#ifndef __TEXTURERESMGR_H__
#define __TEXTURERESMGR_H__

#include <vector>
using namespace std;

#include "../../Engine/Src/KCriticalSection.h"

enum IS_BALANCE_PARAM { 
    //## ��չm_pObjectList��ÿ�β������ӵ�Ԫ�ص���Ŀ
    ISBP_EXPAND_SPACE_STEP = 128, 
    //## ÿ���ٴ�����ͼ�ζ������һ��ƽ�����Ĭ��ֵ��
    ISBP_CHECK_POINT_DEF = 24, 
    //## ͼ����Ŀƽ���Ĭ��ֵ
    ISBP_BALANCE_NUM_DEF128 = 16 * 1024 * 1024, 
	ISBP_BALANCE_NUM_DEF256 = 32 * 1024 * 1024, 
	ISBP_BALANCE_NUM_DEF512 = 48 * 1024 * 1024, 
    //## ������Χ���Ȳ��ҵķ�Χ��С��
    ISBP_TRY_RANGE_DEF = 8 };

class TextureRes;

// ��Դ����Ľڵ�
struct ResNode
{
	uint32		m_nLastUsedTime;			// ��һ����Ⱦ��ʱ���ǩ
	bool		m_bCacheable;				// �Ƿ�����Ӳ�̶�Ӧ�ļ�����Դ
	uint32		m_nID;						// ��ԴID������Դ������
	uint32		m_nType;					// ��Դ����
	TextureRes*	m_pTextureRes;				// ָ����Դ��ָ��
};

class TextureResMgr
{
public:
	uint32 m_nLoadCount;
	uint32 m_nReleaseCount;
    uint32 m_nMaxReleaseCount;
public:
	TextureResMgr();
	~TextureResMgr();

	//## ���ͼ��Դ�洢����ƽ��״����
    void CheckBalance();

	//## ����ͼ�ζ�̬����ƽ�������
    void SetBalanceParam(
        //## ����ͼ�ε���Ŀ��ƽ��ֵ��
        int32 nNumImage, 
        //## ÿ���ٴ�����ͼ�ζ������һ��ƽ���顣
        uint32 uCheckPoint = 3000);

	//## ����ڴ�ͼ�ζ���ʧ�ܣ��򷵻�0�����򷵻ظ���ͼ������ת�����ɵ���ֵid��
    uint32 CreateImage(
        //## ͼ�ε����֡�
        const char* pszName, 
        //## ͼ�κ��
        int32 nWidth, 
         //## ͼ���ݿ�
        int32 nHeight, 
        //## ͼ������
        int32 nType);

	//## ָ���ͷ�ĳ��ͼ��Դ��
    void FreeImage(
        //## ͼ���ļ���/ͼ������
        const char* pszImage);

	//## ����ͷ�ȫ����ͼ�ζ��󣬼Ȱ�����̬���صģ�Ҳ����ͨ������CreateImage���ɵġ�
    void Free();

	//## ��ȡĳ��ͼ����Դ�������Ϣ��
    bool GetImageParam(
        //## ָ�򱣴�ͼ����Դ�ļ���/ͼ�����Ļ�����
        const char* pszImage,
        //## ͼ����Ϣ�洢�ṹ��ָ��
        KImageParam* pImageData,
        //## ͼ��Դ����
		int nType);
	
	bool GetImageFrameParam(
        //## ָ�򱣴�ͼ����Դ�ļ���/ͼ�����Ļ�����
        const char* pszImage,
		//ͼ��֡����
		int		nFrame,
        //## ֡ͼ�����������ͼ�ε�ƫ��
		KRPosition2* pOffset,
		//## ֡ͼ�δ�С
		KRPosition2* pSize,
        //## ͼ��Դ����
		int nType);

	//## ͼ�ε����ݶ���ָ�롣
    TextureRes* GetImage(
        //## ͼ��Դ�ļ�����/ͼ������
        const char* pszImage, 
        //## ͼ��id
        unsigned int& uImage, 
        //## ͼ��ԭ����iImageStore�ڵ�λ�á�
        short& nImagePosition, 
        //## Ҫ��ȡͼ�ε�֡��������
        int nFrame,
        //## ͼ��Դ����
        int nType,
		//## �Ƿ�֤��һ֡��ͼ����
		bool bPrepareTex = true);
	
	//## ��ȡͼ��ָ��֡��ĳ������alphaֵ��
    int32 GetImagePixelAlpha(
        //## ͼ����Դ�ļ���/ͼ����
        const char* pszImage, 
        //## ͼ�ε�֡������
        int nFrame, 
        //## �����ͼ�к�����
        int nX, 
        //## �����ͼ��������
        int nY, 
        //## ͼ��Դ����
        int nType);

	// ��ʼ��¼ͳ����Ϣ
	void StartProfile();

	// ������¼ͳ����Ϣ
	void EndProfile();

	// ���ͳ����Ϣ�ַ���
	void GetProfileString(char* str, int BufLen);
	
	bool InvalidateDeviceObjects();
	bool RestoreDeviceObjects();

private:
	//## ��ø���id�ڼ�¼���е�λ�á�
    //## ����ֵ���壺
    //##  ����ֵ >= 0 --> ������id�ڼ�¼���е�λ�ã���0��ʼ������ֵ��
    //##  ����ֵ < 0  --> ����û�и�����id�������Ҫ�����id�Ļ�������λ��Ӧ��Ϊ(-����ֵ-1)
    int FindImage(
        //## ͼ�ε�id
        uint32 uImage, 
        //## ͼ�μ�¼��m_pObjectList�п��ܵ�λ�á�
        int nPossiblePosition);

	//## ����ͼ�ζ�������ָ��ͼ����Դ���������ʧ�ܣ������ٸ����ɵĶ���
    TextureRes* LoadImage(
        //## ͼ���ļ���
        const char* pszImageFile, 
        //## ͼ������
        uint32 nType) const;

private:
	//## ��ͼ�ζ�������ܴ������ۼӼ�¼ֵ��
    DWORD m_tmLastCheckBalance;
	//## �������ڴ���ͼ����Ŀ��ƽ��ֵ
    int32 m_nBalanceNum;
	//## ����ƽ���ʱ�����á�
    uint32 m_uCheckPoint;
	vector<ResNode>	m_TextureResList;	// ��ͼ��Դ����
	
	// ��������ͳ��
	uint32 m_nHitCount;					// ���м���
	uint32 m_nUseCount;					// ʹ�ü���
	bool m_bDoProfile;					// �Ƿ�������ͳ������
	uint32 m_uTexCacheMemUsed;			// ���������ĵ��ڴ�,��λ�ֽ�
	uint32 m_uMemDrawingUsed;			// ��һ֡������Ⱦ���ڴ棨������ͼ�Ŀհ�����,��λ�ֽ�

    KCriticalSection    m_ImageProcessLock;
};

#endif