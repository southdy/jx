/*****************************************************************************************
//	���ļ���ȡ
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-9-16
------------------------------------------------------------------------------------------
	XPackFile֧�ֶ��̵߳�ͬʱ���ʡ�
	XPackFile�ⲿͨ��XPackElemFileRef���������õĶ԰������ļ������á�
	�ⲿ����������Ķ��������ṹ�ڵ����ݣ��Ի�������İ����ʹ��ܡ�
*****************************************************************************************/
#ifndef _XPACK_FILE_H_
#define	_XPACK_FILE_H_
#include "KSprite.h"

//--------------------------------------------------
//ʹ��XPackFile�����ļ����з��ʲ������õ��ĸ����ṹ
//--------------------------------------------------
struct	XPackElemFileRef
{
	unsigned long	uId;			//�ļ�id
	int				nPackIndex;		//������
	int				nElemIndex;		//���ڵ����ļ�����
	int				nCacheIndex;	//��������
	int				nOffset;		//���ļ��Ĳ������ƶ�
	int				nSize;			//���ļ��Ĵ�С
};

class XPackFile
{
public:
	XPackFile();
	~XPackFile();
	//�򿪰��ļ�
	bool		Open(const char* pszPackFileName, int nSelfIndex);
	//�رհ��ļ�
	void		Close();
	//���Ұ��ڵ����ļ�
	bool		FindElemFile(unsigned long uId, XPackElemFileRef& ElemRef);
	//��ȡ���ڵ����ļ�
	int			ElemFileRead(XPackElemFileRef& ElemRef, void* pBuffer, unsigned uSize);

	//��ȡspr�ļ�ͷ��������spr
	SPRHEAD*	GetSprHeader(XPackElemFileRef& ElemRef, SPROFFS*& pOffsetTable);
	//��ȡ��֡ѹ����spr��һ֡������
	SPRFRAME*	GetSprFrame(SPRHEAD* pSprHeader, int nFrame);

private:
	//ֱ�Ӷ�ȡ���ļ������е����ݵ�������
	bool		DirectRead(void* pBuffer, unsigned int uOffset, unsigned int uSize) const;
	//����ѹ�ض�ȡ���ļ���������
	bool		ExtractRead(void* pBuffer, unsigned int uExtractSize,
						long lCompressType, unsigned int uOffset, unsigned int uSize) const;
	//���������в������ļ���
	int			FindElemFile(unsigned long ulId) const;
	//��cache��������ļ�
	int			FindElemFileInCache(unsigned int uId, int nDesireIndex);
	//�����ļ�������ӵ�cache
	int			AddElemFileToCache(void* pBuffer, int nElemIndex);
	//����һ��������������ָ�������ļ����ݶ�������
	void*		ReadElemFile(int nElemIndex) const;
	//�ͷ�һ��cache��������
	static void	FreeElemCache(int nCacheIndex);

private:
	HANDLE					m_hFile;			//���ļ����
	unsigned int			m_uFileSize;		//���ļ���С
	int						m_nElemFileCount;	//���ļ��ĸ���
	int						m_nSelfIndex;		//���ļ��Լ��ڰ������е�����
	struct XPackIndexInfo*	m_pIndexList;		//���ļ������б�
	CRITICAL_SECTION		m_ReadCritical;		//�������ļ�ʱ���ٽ�������

	//----���ļ�����cache----
	struct XPackElemFileCache
	{
		void*			pBuffer;			//�������ļ����ݵĻ�����
		unsigned long	uId;				//���ļ�id
		long			lSize;				//���ļ���С
		int				nPackIndex;			//�����ĸ����ļ�
		int				nElemIndex;			//���ļ��������б��е�λ��
		unsigned int	uRefFlag;			//�������ñ��
	};

	#define	MAX_XPACKFILE_CACHE			10
	//���ļ���cache����
	static	XPackElemFileCache	ms_ElemFileCache[MAX_XPACKFILE_CACHE];
	//���ļ���cache����Ŀ
	static	int					ms_nNumElemFileCache;
};

#endif