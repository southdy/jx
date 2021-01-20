//#include "KWin32.h"
//#include "StdAfx.h"
#include "ZPackFile.h"
#include "ucl/ucl.h"


unsigned long hash(const char *file_name) {
	unsigned long id = 0;
	const char *ptr = file_name;
	int index = 0;
	while(*ptr) {
		if(*ptr >= 'A' && *ptr <= 'Z') id = (id + (++index) * (*ptr + 'a' - 'A')) % 0x8000000b * 0xffffffef;
		else id = (id + (++index) * (*ptr)) % 0x8000000b * 0xffffffef;
		ptr++;
	}
	return (id ^ 0x12345678);
}

ZMapFile::ZMapFile(const char *file_name) {
	m_Ptr = 0;
	old_offset = 0;
	m_hMap = 0;
	m_hFile = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_hFile != INVALID_HANDLE_VALUE) {
		m_Size = GetFileSize(m_hFile, NULL);
		if(m_Size) {
			m_hMap = CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, m_Size, NULL);
			if(m_hMap) return;
		}
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

char *ZMapFile::map(unsigned long offset, unsigned long size) {
	unsigned long align = offset % 0x10000;									//64K�߽�
	if(m_Ptr) {
		if(offset > old_offset && (offset + size < old_offset + MINIMIZE_MAP_SIZE)) {			//�Ѿ�ӳ����
			return m_Ptr + offset - old_offset;
		}
		UnmapViewOfFile(m_Ptr);
	}
	old_offset = offset - align;
	size = MINIMIZE_MAP_SIZE;
	if(old_offset + size > m_Size) size = m_Size - old_offset;
	m_Ptr = (char *)MapViewOfFile(m_hMap, FILE_MAP_READ, 0, old_offset, size);		
	if(m_Ptr) return m_Ptr + align;					//�ɹ��ˣ�
	else return NULL;
}

void ZMapFile::unmap() {
	if(!m_Ptr) return;
	UnmapViewOfFile(m_Ptr);
	m_Ptr = 0;
}

ZMapFile::~ZMapFile() {
	if(m_Ptr) UnmapViewOfFile(m_Ptr);
	if(m_hMap) CloseHandle(m_hMap);
	if(m_hFile != INVALID_HANDLE_VALUE) CloseHandle(m_hFile);
}


ZCache::ZCache(long size) {
	long align = size % 0x10000;				//������64k����ģ���Ȼ�ή��Ч��
	if(align) size += 0x10000 - align;
	buffer = new char[size];
	cache_size = size;
	free_items = (item_info *)buffer;
	free_items->offset = 0;
	free_items->size = cache_size;
	free_items->index_high = 0;
	memset((void *)&last_item[0], 0, LAST_ITEM * sizeof(item_info));
	last = 0;
}

ZCache::~ZCache() {
	if(buffer) delete[] buffer;
}

//�����Ż�,��¼���ʹ�õ�5��

char *ZCache::getNode(unsigned long index_high, unsigned long index_low, long size) {			//�õ�һ�����л��������ݿ�
	size += sizeof(item_info);
	unsigned long align = size % MINIMIZE_BLOCK_SIZE;
	if(align) size += MINIMIZE_BLOCK_SIZE - align;
	
	if(size > cache_size) return 0;
///	m_Mutex.Lock();
	item_info *next;
	while(free_items->size < size) {										//ɾ���������Ŀֱ�����㹻�Ŀռ�Ϊֹ
		next = (item_info *)((char *)free_items + free_items->size);		//��һ���ڵ�
		if((char *)next - buffer >= cache_size) {							//�Ѿ����˽�����λ�ã��ص�һ��
			free_items = (item_info *)buffer;
			free_items->index_high = 0;
		}
		else free_items->size += next->size;
	}

	free_items->index_high = index_high;
	free_items->index_low = index_low;
	next = (item_info *)((char *)free_items + size);
	if((char *)next - buffer == cache_size) next = (item_info *)buffer;
	else if(free_items->size > size) {
		next->index_high = 0;
		next->size = free_items->size - size;
		next->offset = free_items->offset + size;
	}

	free_items->size = size;
	last_item[last].index_high = index_high;
	last_item[last].index_low = index_low;
	last_item[last].size = size;
	last_item[last].offset = free_items->offset;
	if(++last == LAST_ITEM) last = 0;
	char *result = buffer + free_items->offset + sizeof(item_info);
	free_items = next;
//	m_Mutex.Unlock();
	return result;
}

char *ZCache::searchNode(unsigned long index_high, unsigned long index_low) {
//	m_Mutex.Lock();
	for(int index = 0; index < LAST_ITEM; index++) {
		if(last_item[index].index_high == index_high && last_item[index].index_low == index_low) {
//			m_Mutex.Unlock();
			return buffer + last_item[index].offset + sizeof(item_info);
		}
	}
	item_info *current = (item_info *)buffer;
	while((char *)current - buffer < cache_size) {
		if(current->index_high == index_high && current->index_low == index_low) {
			last_item[last].index_high = index_high;
			last_item[last].index_low = index_low;
			last_item[last].size = current->size;
			last_item[last].offset = current->offset;
			if(++last == LAST_ITEM) last = 0;
//			m_Mutex.Unlock();
			return buffer + current->offset + sizeof(item_info);
		}
		current = (item_info *)((char *)current + current->size);
	}
//	m_Mutex.Unlock();
	return 0;
}


int ZPackFile::getNodeIndex(unsigned long id) {								//���ַ��ҵ�ָ��������
	int i;
	for(i=0;i<header.count;++i)
	{
		if (id == index_list[i].id) break;
	}
	if(id != index_list[i].id) return -1;
	return i;
/*
		int nBegin, nEnd, nMid;
		nBegin = 0;
		nEnd = header.count;
		while (nBegin <= nEnd) {
			nMid = (nBegin + nEnd) / 2;
			if (id == index_list[nMid].id) break;
			if (id < index_list[nMid].id) nEnd = nMid - 1;
			else nBegin = nMid + 1;
		}
		if(id != index_list[nMid].id) return -1;								//�����ļ�����Ҳû��
		return nMid;*/
	
}

unsigned long ZPackFile::getSize(unsigned long index) {
	return index_list[index].size;
}

bool ZPackFile::_readData(int node_index, char *node) {
	char *source = data_map->map(index_list[node_index].offset, index_list[node_index].size);
	if(!source) {
		return false;
	}
	unsigned int dest_length;
	unsigned long compress_type = index_list[node_index].compress_size >> 24;
	int r = 1;
	if((compress_type == TYPE_UCL) | (compress_type == TYPE_BZIP2)) {									//����ѹ��
		if(compress_type == TYPE_UCL) {
			r = ucl_nrv2b_decompress_8((BYTE *)source, index_list[node_index].compress_size & 0x00FFFFFF, (BYTE *)node, &dest_length, NULL);
		}
		else if(compress_type == TYPE_BZIP2) {
//			dest_length = index_list[node_index].size;
//			r = BZ2_bzBuffToBuffDecompress(node + sizeof(id), &dest_length, source, index_list[node_index].compress_size & 0x00FFFFFF, 0, 0);
		}
	}
	if(!r) return true;
	return false;
}

ZPackFile::ZPackFile(const char *file_name, ZCache *the_cache) {
	data_map = 0;
	index_list = 0;
	opened = false;
	cache = the_cache;
	data_map = new ZMapFile(file_name);
	char *header_ptr = data_map->map(0, sizeof(header));
	if(!header_ptr) return;
	memcpy(&header, header_ptr, sizeof(header));
	index_list = new index_info[header.count];
	char *index_ptr = data_map->map(header.index_offset, header.count * sizeof(index_info));
	memcpy(&index_list[0], index_ptr, header.count * sizeof(index_info));
	opened = true;
}

ZPackFile::~ZPackFile() {
	if(index_list) delete[] index_list;
	if(data_map) delete data_map;
}

char *ZPackFile::getData(unsigned long id) {
	char * node = cache->searchNode(id);
	if(node) return node;
	int node_index = getNodeIndex(id);
	if(node_index == -1) return 0;
	node = cache->getNode(id, -1, index_list[node_index].size);
	if(_readData(node_index, node)) return node;
	else return 0;
}

char *ZPackFile::getData(const char *name) {											//��ȡָ�����ļ�����
	unsigned long id = hash(name);
	return getData(id);
}

char* ZPackFile::getOrigData(unsigned long id)
{//ȡ��δ��ѹ��ԭʼ����
	char * node = cache->searchNode(id);
	if(node) return node;
	int node_index = getNodeIndex(id);
	if(node_index == -1) return 0;
	node = cache->getNode(id, -1, index_list[node_index].size);
	return data_map->map(index_list[node_index].offset, index_list[node_index].size);
}


int ZPackFileNT::getNodeIndex(unsigned long id) {								//���ַ��ҵ�ָ��������
	int i;
	for(i=0;i<header.count;++i)
	{
		if (id == index_list[i].id) break;
	}
	if(id != index_list[i].id) return -1;
	return i;
/*
		int nBegin, nEnd, nMid;
		nBegin = 0;
		nEnd = header.count;
		while (nBegin <= nEnd) {
			nMid = (nBegin + nEnd) / 2;
			if (id == index_list[nMid].id) break;
			if (id < index_list[nMid].id) nEnd = nMid - 1;
			else nBegin = nMid + 1;
		}
		if(id != index_list[nMid].id) return -1;								//�����ļ�����Ҳû��
		return nMid;*/
	
}

unsigned long ZPackFileNT::getSize(unsigned long index) {
	return index_list[index].size;
}

bool ZPackFileNT::_readData(int node_index, char *node) {
	char *source = data_map->map(index_list[node_index].offset, index_list[node_index].size);
	if(!source) {
		return false;
	}
	unsigned int dest_length;
	unsigned long compress_type = index_list[node_index].compress_size >> 24;
	int r = 1;
	if((compress_type == TYPE_UCL) | (compress_type == TYPE_BZIP2)) {									//����ѹ��
		if(compress_type == TYPE_UCL) {
			r = ucl_nrv2b_decompress_8((BYTE *)source, index_list[node_index].compress_size & 0x00FFFFFF, (BYTE *)node, &dest_length, NULL);
		}
		else if(compress_type == TYPE_BZIP2) {
//			dest_length = index_list[node_index].size;
//			r = BZ2_bzBuffToBuffDecompress(node + sizeof(id), &dest_length, source, index_list[node_index].compress_size & 0x00FFFFFF, 0, 0);
		}
	}
	if(!r) return true;
	return false;
}

ZPackFileNT::ZPackFileNT(const char *file_name, ZCache *the_cache) {
	data_map = 0;
	index_list = 0;
	opened = false;
	cache = the_cache;
	data_map = new ZMapFile(file_name);
	char *header_ptr = data_map->map(0, sizeof(header));
	if(!header_ptr) return;
	memcpy(&header, header_ptr, sizeof(header));
	index_list = new index_info_nt[header.count];
	char *index_ptr = data_map->map(header.SubHeader[0].index_offset, header.count * sizeof(index_info_nt));
	memcpy(&index_list[0], index_ptr, header.count * sizeof(index_info_nt));
	opened = true;
}

ZPackFileNT::~ZPackFileNT() {
	if(index_list) delete[] index_list;
	if(data_map) delete data_map;
}

char *ZPackFileNT::getData(unsigned long id) {
	char * node = cache->searchNode(id);
	if(node) return node;
	int node_index = getNodeIndex(id);
	if(node_index == -1) return 0;
	node = cache->getNode(id, -1, index_list[node_index].size);
	if(_readData(node_index, node)) return node;
	else return 0;
}

char *ZPackFileNT::getData(const char *name) {											//��ȡָ�����ļ�����
	unsigned long id = hash(name);
	return getData(id);
}

char* ZPackFileNT::getOrigData(unsigned long id)
{//ȡ��δ��ѹ��ԭʼ����
	char * node = cache->searchNode(id);
	if(node) return node;
	int node_index = getNodeIndex(id);
	if(node_index == -1) return 0;
	node = cache->getNode(id, -1, index_list[node_index].size);
	return data_map->map(index_list[node_index].offset, index_list[node_index].size);
}
