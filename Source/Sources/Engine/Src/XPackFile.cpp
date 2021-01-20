/*****************************************************************************************
//	��ȡ����ļ�
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-9-16
*****************************************************************************************/
#include "KWin32.h"
#include "XPackFile.h"
#include "ucl/ucl.h"
#include <crtdbg.h>

//һ��Pack�ļ����е�ͷ�ṹ:
struct XPackFileHeader
{
	unsigned char cSignature[4];		//�ĸ��ֽڵ��ļ���ͷ��־���̶�Ϊ�ַ���'PACK'
	unsigned long uCount;				//���ݵ���Ŀ��
	unsigned long uIndexTableOffset;	//������ƫ����
	unsigned long uDataOffset;			//���ݵ�ƫ����
	unsigned long uCrc32;				//У���
	unsigned char cReserved[12];		//�������ֽ�
};

#define	XPACKFILE_SIGNATURE_FLAG		0x4b434150	//'PACK'

//Pack�ж�Ӧÿ�����ļ���������Ϣ��
struct XPackIndexInfo
{
	unsigned long	uId;				//���ļ�id
	unsigned long	uOffset;			//���ļ��ڰ��е�ƫ��λ��
	long			lSize;				//���ļ���ԭʼ��С
	long			lCompressSizeFlag;	//���ļ�ѹ����Ĵ�С��ѹ������
										//����ֽڱ�ʾѹ����������XPACK_METHOD
										//�͵������ֽڱ�ʾ���ļ�ѹ����Ĵ�С
};

//���ļ���ѹ����ʽ
enum XPACK_METHOD
{
	TYPE_NONE	= 0x00000000,			//û��ѹ��
	TYPE_UCL	= 0x01000000,			//UCLѹ��
	TYPE_BZIP2	= 0x02000000,			//bzip2ѹ��
	TYPE_FRAME	= 0x10000000,			//ʹ���˶���֡ѹ��,���ļ�Ϊspr����ʱ�ſ����õ�

	TYPE_METHOD_FILTER = 0x0f000000,	//���˱��
	TYPE_FILTER = 0xff000000,			//���˱��
};

//pak���б����spr֡��Ϣ��
struct XPackSprFrameInfo
{
	long lCompressSize;
	long lSize;
} ;

XPackFile::XPackElemFileCache	XPackFile::ms_ElemFileCache[MAX_XPACKFILE_CACHE];
int								XPackFile::ms_nNumElemFileCache = 0;

XPackFile::XPackFile()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_uFileSize = 0;
	m_pIndexList = NULL;
	m_nElemFileCount = 0;
	InitializeCriticalSection(&m_ReadCritical);
}

XPackFile::~XPackFile()
{
	Close();
	DeleteCriticalSection(&m_ReadCritical);
}

//-------------------------------------------------
//���ܣ��򿪰��ļ�
//���أ��ɹ����
//-------------------------------------------------
bool XPackFile::Open(const char* pszPackFileName, int nSelfIndex)
{
	bool bResult = false;
	Close();
	EnterCriticalSection(&m_ReadCritical);
	m_nSelfIndex = nSelfIndex;
	m_hFile = ::CreateFile(pszPackFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	while (m_hFile != INVALID_HANDLE_VALUE)
	{
		m_uFileSize = ::GetFileSize(m_hFile, NULL);
		if (m_uFileSize == 0 || m_uFileSize == INVALID_FILE_SIZE ||
			m_uFileSize <= sizeof(XPackFileHeader))
		{
			break;
		}
		XPackFileHeader	Header;
		DWORD			dwListSize, dwReaded;
		//--��ȡ���ļ�ͷ--
		if (::ReadFile(m_hFile, &Header, sizeof(Header), &dwReaded, NULL) == FALSE)
			break;
		//--���ļ���������ݵĺϷ����ж�--
		if (dwReaded != sizeof(Header) ||
			*(int*)(&Header.cSignature) != XPACKFILE_SIGNATURE_FLAG ||
			Header.uCount == 0 ||
			Header.uIndexTableOffset < sizeof(XPackFileHeader) ||
			Header.uIndexTableOffset >= m_uFileSize ||
			Header.uDataOffset < sizeof(XPackFileHeader) ||
			Header.uDataOffset >= m_uFileSize)
		{
			break;
		}

		//--��ȡ������Ϣ��--
		dwListSize = sizeof(XPackIndexInfo) * Header.uCount;
		m_pIndexList = (XPackIndexInfo*)malloc(dwListSize);
		if (m_pIndexList == NULL ||
			::SetFilePointer(m_hFile, Header.uIndexTableOffset, NULL, FILE_BEGIN) != Header.uIndexTableOffset)
		{
			break;
		}
		if (::ReadFile(m_hFile, m_pIndexList, dwListSize, &dwReaded, NULL) == FALSE)
			break;
		if (dwReaded != dwListSize)
			break;
		m_nElemFileCount = Header.uCount;
		bResult = true;
		break;
	};
	if (bResult == false)
		Close();
	LeaveCriticalSection(&m_ReadCritical);
	return bResult;
}

//-------------------------------------------------
//���ܣ��رհ��ļ�
//-------------------------------------------------
void XPackFile::Close()
{
	EnterCriticalSection(&m_ReadCritical);

	if (m_pIndexList)
	{
		//----���cache�л��浽�ģ����ܣ��Ǵ˰��е����ļ�----
		for (int i = ms_nNumElemFileCache - 1; i >=0; i--)
		{
			if (ms_ElemFileCache[i].nPackIndex == m_nSelfIndex)
			{
				FreeElemCache(i);
				ms_nNumElemFileCache --;
				for (int j = i; j < ms_nNumElemFileCache; j++)
					ms_ElemFileCache[j] = ms_ElemFileCache[j + 1];
			}
		}
		free (m_pIndexList);
		m_pIndexList = NULL;
	}
	m_nElemFileCount = 0;

	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
	m_uFileSize = 0;

	LeaveCriticalSection(&m_ReadCritical);
}

//-------------------------------------------------
//���ܣ��ͷ�һ��cache��������
//���أ��ɹ����
//-------------------------------------------------
void XPackFile::FreeElemCache(int nCacheIndex)
{
	_ASSERT(nCacheIndex >= 0 && nCacheIndex < ms_nNumElemFileCache);
	if (ms_ElemFileCache[nCacheIndex].pBuffer)
	{
		free(ms_ElemFileCache[nCacheIndex].pBuffer);
		ms_ElemFileCache[nCacheIndex].pBuffer = NULL;
	}
	ms_ElemFileCache[nCacheIndex].uId = 0;
	ms_ElemFileCache[nCacheIndex].lSize = 0;
	ms_ElemFileCache[nCacheIndex].uRefFlag = 0;
	ms_ElemFileCache[nCacheIndex].nPackIndex = -1;
}

//-------------------------------------------------
//���ܣ�ֱ�Ӷ�ȡ���ļ������е����ݵ�������
//���أ��ɹ����
//-------------------------------------------------
bool XPackFile::DirectRead(void* pBuffer, unsigned int uOffset, unsigned int uSize) const
{
	bool bResult = false;
	DWORD dwReaded;
	_ASSERT(pBuffer && m_hFile != INVALID_HANDLE_VALUE);
	if (uOffset + uSize <= m_uFileSize &&
		::SetFilePointer(m_hFile, uOffset, 0, FILE_BEGIN) == uOffset)
	{
		if (ReadFile(m_hFile, pBuffer, uSize, &dwReaded, NULL))
		{
			if (dwReaded == uSize)
				bResult = true;
		}
	}
	return bResult;
}

//-------------------------------------------------
//���ܣ�����ѹ�ض�ȡ���ļ���������
//������pBuffer --> ������ָ��
//		uExtractSize  --> ���ݣ���������ѹ��Ĵ�С��pBuffer�������Ĵ�С��С�ڴ���
//		lCompressType --> ֱ�ӴӰ��ж�ȡ��ԭʼ��/ѹ������С
//		uOffset  --> �Ӱ��еĴ�ƫ��λ�ÿ�ʼ��ȡ
//		uSize    --> �Ӱ���ֱ�Ӷ�ȡ�ã�ѹ�������ݵĴ�С
//���أ��ɹ����
//-------------------------------------------------
bool XPackFile::ExtractRead(void* pBuffer, unsigned int uExtractSize,
							long lCompressType, unsigned int uOffset, unsigned int uSize) const
{
	_ASSERT(pBuffer);
	bool bResult = false;
	if (lCompressType == TYPE_NONE)
	{
		if (uExtractSize == uSize)
			bResult = DirectRead(pBuffer, uOffset, uSize);
	}
	else
	{
		void*	pReadBuffer = malloc(uSize);
		if (pReadBuffer)
		{
		    if (lCompressType == TYPE_UCL && DirectRead(pReadBuffer, uOffset, uSize))
			{
				unsigned int uDestLength;
				ucl_nrv2b_decompress_8((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength, NULL);
				bResult =  (uDestLength == uExtractSize);
			}
			free (pReadBuffer);
		}
	}
	return bResult;
}

//-------------------------------------------------
//���ܣ����������в������ļ���(���ַ���)
//���أ����ҵ��������������е�λ��(>=0)����δ�ҵ�����-1
//-------------------------------------------------
int XPackFile::FindElemFile(unsigned long ulId) const
{
	int nBegin, nEnd, nMid;
	nBegin = 0;
	nEnd = m_nElemFileCount - 1;
	while (nBegin <= nEnd)
	{
		nMid = (nBegin + nEnd) / 2;
		if (ulId < m_pIndexList[nMid].uId)
			nEnd = nMid - 1;
		else if (ulId > m_pIndexList[nMid].uId)
			nBegin = nMid + 1;
		else
			break;
	}
	return ((nBegin <= nEnd) ? nMid : -1);
}

//-------------------------------------------------
//���ܣ����Ұ��ڵ����ļ�
//������uId --> ���ļ���id
//		ElemRef -->����ҵ����ڴ˽ṹ���������ļ��������Ϣ
//���أ��Ƿ��ҵ�
//-------------------------------------------------
bool XPackFile::FindElemFile(unsigned long uId, XPackElemFileRef& ElemRef)
{
	ElemRef.nElemIndex = -1;
	if (uId)
	{
		EnterCriticalSection(&m_ReadCritical);
		ElemRef.nCacheIndex = FindElemFileInCache(uId, -1);
		if (ElemRef.nCacheIndex >= 0)
		{
			ElemRef.uId = uId;
			ElemRef.nPackIndex = ms_ElemFileCache[ElemRef.nCacheIndex].nPackIndex;
			ElemRef.nElemIndex = ms_ElemFileCache[ElemRef.nCacheIndex].nElemIndex;
			ElemRef.nSize = ms_ElemFileCache[ElemRef.nCacheIndex].lSize;
			ElemRef.nOffset = 0;
		}
		else
		{
			ElemRef.nElemIndex = FindElemFile(uId);
			if (ElemRef.nElemIndex >= 0)
			{
				ElemRef.uId = uId;
				ElemRef.nPackIndex = m_nSelfIndex;
				ElemRef.nOffset = 0;
				ElemRef.nSize = m_pIndexList[ElemRef.nElemIndex].lSize;
			}
		}
		LeaveCriticalSection(&m_ReadCritical);
	}
	return (ElemRef.nElemIndex >= 0);
}

//-------------------------------------------------
//���ܣ����仺�������������ڵ����ļ������ݵ�����
//���������ļ��ڰ��ڵ�����
//���أ��ɹ��򷵻ػ�������ָ�룬���򷵻ؿ�ָ��
//-------------------------------------------------
void* XPackFile::ReadElemFile(int nElemIndex) const
{
	_ASSERT(nElemIndex >= 0 && nElemIndex < m_nElemFileCount);
	void*	pDataBuffer = malloc(m_pIndexList[nElemIndex].lSize);
	if (pDataBuffer)
	{
		if (ExtractRead(pDataBuffer,
				m_pIndexList[nElemIndex].lSize,
				(m_pIndexList[nElemIndex].lCompressSizeFlag & TYPE_FILTER),
				m_pIndexList[nElemIndex].uOffset,
				(m_pIndexList[nElemIndex].lCompressSizeFlag & (~TYPE_FILTER))) == false)
		{
			free (pDataBuffer);
			pDataBuffer = NULL;
		}
	}
	return pDataBuffer;
}

//-------------------------------------------------
//���ܣ���cache��������ļ�
//������uId --> ���ļ�id
//		nDesireIndex --> ��cache�еĿ���λ��
//���أ��ɹ��򷵻�cache�ڵ�����(>=0),ʧ���򷵻�-1
//-------------------------------------------------
int XPackFile::FindElemFileInCache(unsigned int uId, int nDesireIndex)
{
	if (nDesireIndex >= 0 && nDesireIndex < ms_nNumElemFileCache &&
		uId == ms_ElemFileCache[nDesireIndex].uId)
	{
		ms_ElemFileCache[nDesireIndex].uRefFlag = 0xffffffff;
		return nDesireIndex;
	}

	nDesireIndex = -1;
	for (int i = 0; i < ms_nNumElemFileCache; i++)
	{
		if (uId == ms_ElemFileCache[i].uId)
		{
			ms_ElemFileCache[i].uRefFlag = 0xffffffff;
			nDesireIndex = i;
			break;
		}
	}
	return nDesireIndex;
}

//-------------------------------------------------
//���ܣ������ļ�������ӵ�cache
//������pBuffer --> �������ļ����ݵĻ�����
//		nElemIndex --> ���ļ��ڰ��е�����
//���أ���ӵ�cache������λ��
//-------------------------------------------------
int XPackFile::AddElemFileToCache(void* pBuffer, int nElemIndex)
{
	_ASSERT(pBuffer && nElemIndex >= 0 && nElemIndex < m_nElemFileCount);
	int nCacheIndex;
	if (ms_nNumElemFileCache < MAX_XPACKFILE_CACHE)
	{	//�ҵ�һ����λ��
		nCacheIndex = ms_nNumElemFileCache++;
	}
	else
	{	//�ͷ�һ���ɵ�cache�ڵ�
		nCacheIndex = 0;
		if (ms_ElemFileCache[0].uRefFlag)
			ms_ElemFileCache[0].uRefFlag --;
		for (int i = 1; i < MAX_XPACKFILE_CACHE; i++)
		{
			if (ms_ElemFileCache[i].uRefFlag)
				ms_ElemFileCache[i].uRefFlag --;
			if (ms_ElemFileCache[i].uRefFlag < ms_ElemFileCache[nCacheIndex].uRefFlag)
				nCacheIndex = i;

		}
		FreeElemCache(nCacheIndex);
	}
	ms_ElemFileCache[nCacheIndex].pBuffer = pBuffer;
	ms_ElemFileCache[nCacheIndex].uId = m_pIndexList[nElemIndex].uId;
	ms_ElemFileCache[nCacheIndex].lSize = m_pIndexList[nElemIndex].lSize;
	ms_ElemFileCache[nCacheIndex].nPackIndex = m_nSelfIndex;
	ms_ElemFileCache[nCacheIndex].nElemIndex = nElemIndex;
	ms_ElemFileCache[nCacheIndex].uRefFlag = 0xffffffff;
	return nCacheIndex;
}

//-------------------------------------------------
//���ܣ���ȡ���ļ�һ�����ȵ����ݵ�������
//������pBuffer --> ������ȡ���ݵĻ�����
//		uSize --> Ҫ��ȡ�����ݵĳ���
//���أ��ɹ���ȡ���ֽ���
//-------------------------------------------------
int XPackFile::ElemFileRead(XPackElemFileRef& ElemRef, void* pBuffer, unsigned uSize)
{
	int nResult = 0;
	if (pBuffer && ElemRef.uId &&ElemRef.nElemIndex >= 0)
	{
		EnterCriticalSection(&m_ReadCritical);

		//--�ȿ��Ƿ��Ѿ���cache����---
		ElemRef.nCacheIndex = FindElemFileInCache(ElemRef.uId, ElemRef.nCacheIndex);

		if (ElemRef.nCacheIndex < 0 &&								//��cache��δ�ҵ�
			ElemRef.nElemIndex < m_nElemFileCount &&
			m_pIndexList[ElemRef.nElemIndex].uId == ElemRef.uId)
		{
			void*	pDataBuffer = ReadElemFile(ElemRef.nElemIndex);
			if (pDataBuffer)
				ElemRef.nCacheIndex = AddElemFileToCache(pDataBuffer, ElemRef.nElemIndex);
		}

		if (ElemRef.nCacheIndex >= 0 &&
			//����������Ӧ��չ����飬��ֹ��ģ���ⲿ�ı䣬�������
			//ΪЧ�ʿɿ���ʡ�ԣ������ⲿ���չ������ı�ElemRef�����ݡ�
			ElemRef.nPackIndex == ms_ElemFileCache[ElemRef.nCacheIndex].nPackIndex &&
			ElemRef.nElemIndex == ms_ElemFileCache[ElemRef.nCacheIndex].nElemIndex &&
			ElemRef.nSize == ms_ElemFileCache[ElemRef.nCacheIndex].lSize
			)
		{
			//_ASSERT(ElemRef.nPackIndex == ms_ElemFileCache[ElemRef.nCacheIndex].nPackIndex);
			//_ASSERT(ElemRef.nElemIndex == ms_ElemFileCache[ElemRef.nCacheIndex].nElemIndex);
			//_ASSERT(ElemRef.nSize == ms_ElemFileCache[ElemRef.nCacheIndex].lSize);

			if (ElemRef.nOffset < 0)
				ElemRef.nOffset = 0;
			if (ElemRef.nOffset < ElemRef.nSize)
			{
				if (ElemRef.nOffset + (int)uSize <= ElemRef.nSize)
					nResult = uSize;
				else
					nResult = ElemRef.nSize - ElemRef.nOffset;
				memcpy(pBuffer, (char*)ms_ElemFileCache[ElemRef.nCacheIndex].pBuffer + ElemRef.nOffset, nResult);
				ElemRef.nOffset += nResult;
			}
			else
			{
				ElemRef.nOffset = ElemRef.nSize;
			}
		}
		LeaveCriticalSection(&m_ReadCritical);
	}
	return nResult;
}

#define	NODE_INDEX_STORE_IN_RESERVED	2

SPRHEAD* XPackFile::GetSprHeader(XPackElemFileRef& ElemRef, SPROFFS*& pOffsetTable)
{
	SPRHEAD*		pSpr = NULL;
	bool			bOk = false;

	pOffsetTable = NULL;
	if (ElemRef.uId == 0 || ElemRef.nElemIndex < 0)
		return NULL;

	EnterCriticalSection(&m_ReadCritical);
	if(ElemRef.nElemIndex < m_nElemFileCount &&
		m_pIndexList[ElemRef.nElemIndex].uId == ElemRef.uId)
	{
        //���ȼ�����id��ʲô����ѹ����ʽ
		if ((m_pIndexList[ElemRef.nElemIndex].lCompressSizeFlag & TYPE_FRAME) == 0)
		{
			pSpr = (SPRHEAD*)ReadElemFile(ElemRef.nElemIndex);
			if (pSpr)
			{
				if ((*(int*)&pSpr->Comment[0]) == SPR_COMMENT_FLAG)
				{
					pOffsetTable = (SPROFFS*)(((char*)pSpr) + sizeof(SPRHEAD) + pSpr->Colors * 3);
					bOk = true;
				}
			}
		}
		else
		{
			SPRHEAD Header;
			if (DirectRead(&Header, m_pIndexList[ElemRef.nElemIndex].uOffset, sizeof(SPRHEAD)))
			{
				if (*(int*)&(Header.Comment[0]) == SPR_COMMENT_FLAG)
				{
					unsigned int	u2ListSize = Header.Colors * 3 + Header.Frames * sizeof(XPackSprFrameInfo);
					pSpr = (SPRHEAD*)malloc(sizeof(SPRHEAD) + u2ListSize);
					if (pSpr)
					{
						if (DirectRead((char*)(&pSpr[1]),
							m_pIndexList[ElemRef.nElemIndex].uOffset + sizeof(SPRHEAD),	u2ListSize))
						{
							memcpy(pSpr, &Header, sizeof(SPRHEAD));
							bOk = true;
						}
					}
				}
			}
		}

		if (pSpr)
		{
			if (bOk)
			{
				*((int*)&pSpr->Reserved[NODE_INDEX_STORE_IN_RESERVED]) = ElemRef.nElemIndex;
			}
			else
			{
				free (pSpr);
				pSpr = NULL;
			}
		}
	}
	LeaveCriticalSection(&m_ReadCritical);
    return pSpr;
}

SPRFRAME* XPackFile::GetSprFrame(SPRHEAD* pSprHeader, int nFrame)
{
	SPRFRAME*	pFrame = NULL;
	if (pSprHeader && nFrame >= 0 && nFrame < pSprHeader->Frames)
	{
		EnterCriticalSection(&m_ReadCritical);
		int nNodeIndex = *((int*)&pSprHeader->Reserved[NODE_INDEX_STORE_IN_RESERVED]);
		if (nNodeIndex >= 0 && nNodeIndex < m_nElemFileCount)
		{
			long lCompressType = m_pIndexList[nNodeIndex].lCompressSizeFlag;
			if ((lCompressType & TYPE_FRAME) != 0)
			{
				bool bOk = false;
				lCompressType &= TYPE_METHOD_FILTER;
				long	lTempValue = sizeof(SPRHEAD) + pSprHeader->Colors * 3;
				//����ָ��֡����Ϣ
				XPackSprFrameInfo* pFrameList = (XPackSprFrameInfo *)((char*)pSprHeader + lTempValue);
				unsigned long	uSrcOffset = m_pIndexList[nNodeIndex].uOffset + lTempValue +pSprHeader->Frames * sizeof(XPackSprFrameInfo);
			    while(nFrame > 0)
				{
					uSrcOffset += (pFrameList++)->lCompressSize;
					nFrame--;
				};
				lTempValue = pFrameList->lSize;
				if (lTempValue < 0)
				{
					lTempValue = -lTempValue;
					if (pFrame = (SPRFRAME*)malloc(lTempValue))
						bOk = DirectRead(pFrame, uSrcOffset, lTempValue);
				}
				else
				{
					if (pFrame = (SPRFRAME*)malloc(lTempValue))
					{
						bOk = ExtractRead(pFrame, lTempValue, lCompressType, uSrcOffset, pFrameList->lCompressSize);
					}
				}
				if (bOk == false && pFrame != NULL)
				{
					free(pFrame);
					pFrame = NULL;
				}
			}
		}
		LeaveCriticalSection(&m_ReadCritical);
	}
	return pFrame;	
}
