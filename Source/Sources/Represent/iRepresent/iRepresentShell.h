/*****************************************************************************************
//  ����ģ��Ķ���ӿڡ�
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-11-11
------------------------------------------------------------------------------------------
*****************************************************************************************/

#ifndef IREPRESENTSHELL_H_HEADER_INCLUDED_C2311ED3
#define IREPRESENTSHELL_H_HEADER_INCLUDED_C2311ED3

struct KRepresentUnit;
struct KImageParam;
#include "Font/FontDef.h"
#include "Text/TextProcessDef.h"

struct KRPosition2;
#define TEXT_IN_SINGLE_PLANE_COORD -32767
#define LIGHTING_GRID_SIZEX			32							// ���������Ϣ���ӵĴ�С������������512
#define LIGHTING_GRID_SIZEY			32							// ���������Ϣ���ӵĴ�С������������512
//RWPP_AREGION_WIDTH��RWPP_AREGION_HEIGHTҪ��KScenePlaceRegionC.h�е�ֵ����һ��
#define AREGION_WIDTH	512
#define AREGION_HEIGHT	1024
#define LIGHTING_GRID_WIDTH	(AREGION_WIDTH / LIGHTING_GRID_SIZEX * 3)	// �Ÿ��������򰲸��Ӽ���Ŀ��
#define LIGHTING_GRID_HEIGHT (AREGION_HEIGHT / LIGHTING_GRID_SIZEY * 3)	// �Ÿ��������򰲸��Ӽ���ĸ߶�

//��Ļ��ͼ�ļ�����
enum ScreenFileType
{
	SCRFILETYPE_BMP,
	SCRFILETYPE_JPG,
};

enum RepresentOption
{
	DYNAMICLIGHT = 0,
	PERSPECTIVE,
};

//-----���ݻ����������ݸ�ʽö��------
enum	BITMAP_DATA_BUFFER_FORMAT
{
	BDBF_16BIT_555	= 1,
	BDBF_16BIT_565,
	BDBF_16BIT_4444,
	DBDF_24BIT,
	DBDF_32BIT,
};

struct KBitmapDataBuffInfo
{
	int		nWidth;			//ͼ�ο��
	int		nHeight;		//ͼ�θ߶�
	int		nPitch;			//ͼ�λ�����ÿ�������ݵ����ļ��
	BITMAP_DATA_BUFFER_FORMAT	eFormat;	//���ݸ�ʽ��ȡֵ����ö��BITMAP_DATA_BUFFER_FORMAT
	void*	pData;			//������ָ��
};

struct IInlinePicEngineSink;

//## ����ģ��Ķ���ӿڡ�
struct iRepresentShell
{
  public:
    //## �ͷ����ٽӿڶ���
    virtual void Release() = 0;

//---------��ͼ�豸���---------------

    //## ������ͼ�豸�������ͼ������
    virtual bool Create(
        //## �豸�����ڣ������λ�����ص㣩
        int nWidth, 
        //## �豸�����ڣ��ݿ���λ�����ص㣩
        int nHeight, 
        //## �Ƿ��ռȫ��Ļ
        bool bFullScreen) = 0;

    //## ���û�ͼ�豸
    virtual bool Reset(
        //## �豸�����ڣ������λ�����ص㣩
        int nWidth, 
        //## �豸�����ڣ��ݿ���λ�����ص㣩
        int nHeight, 
        //## �Ƿ��ռȫ��Ļ
        bool bFullScreen) = 0;

//---------�������������---------------

    //## ����һ���������
    virtual bool CreateAFont(
        //## �ֿ��ļ�����
        const char* pszFontFile, 
        //## �ֿ�ʹ�õ��ַ����뼯��
        CHARACTER_CODE_SET CharaSet, 
        //## �������id.
        int nId) = 0;

    //## ������֡�
    virtual void OutputText(
        //## ʹ�õ��������id��
        int nFontId, 
        //## Ҫ������ַ�����
        const char* psText, 
        //## Ҫ������ַ����ĳ���(BYTE)��
        //## ��nCount���ڵ���0ʱ���ַ������Բ���������Ľ�����������ʾ����ַ��Ľ�����
        //## ��nCountС��0ʱ����ʾ���ַ�������'\0'��β�������ݽ����ַ���ȷ������ַ����ĳ��ȡ�
        //## Ĭ��ֵΪ-1��
        int nCount = KRF_ZERO_END, 
        //## �ַ�����ʾ�������X���������ֵΪKF_FOLLOW��
        //## ����ַ����������ϴ��ַ��������λ��֮��
        //## Ĭ��ֵΪKRF_FOLLOW��
        int nX = KRF_FOLLOW, 
        //## �ַ�����ʾ�������Y, �������ֵΪKF_FOLLOW��
        //## ���ַ�����ǰһ������ַ�����ͬһ�е�λ�á�
        //## Ĭ��ֵΪKRF_FOLLOW��
        int nY = KRF_FOLLOW, 
        //## �ַ�����ʾ��ɫ��Ĭ��Ϊ��ɫ����32bit����ARGB�ĸ�
        //## ʽ��ʾ��ɫ��ÿ������8bit��
        unsigned int Color = 0xFF000000, 
        //## �Զ����е��п����ƣ������ֵС��һ��ȫ���ַ���������Զ����д���Ĭ��ֵΪ0���Ȳ����Զ����д���
        int nLineWidth = 0,
		int nZ = TEXT_IN_SINGLE_PLANE_COORD,
		//�ֵı�Ե��ɫ
		unsigned int BorderColor = 0) = 0;

    //## ������֡�
    virtual int OutputRichText(
        //## ʹ�õ��������id��
        int nFontId, 
		KOutputTextParam* pParam,
        //## Ҫ������ַ�����
        const char* psText, 
        //## Ҫ������ַ����ĳ���(BYTE)��
        //## ��nCount���ڵ���0ʱ���ַ������Բ���������Ľ�����������ʾ����ַ��Ľ�����
        //## ��nCountС��0ʱ����ʾ���ַ�������'\0'��β�����м䲻����'\0'���ַ���
        //## Ĭ��ֵΪ-1��		
        int nCount = KRF_ZERO_END, 
        //##Documentation
        //## �Զ����е��п����ƣ������ֵС��һ��ȫ���ַ���������Զ����д���Ĭ��ֵΪ0���Ȳ����Զ����д���
        int nLineWidth = 0) = 0;

    //## ����ָ���������ַ�����������ַ�ƫ��(-1Ϊʧ��)
    virtual int LocateRichText(
		//## ָ��������
		int nX, int nY,

        //## ʹ�õ��������id��
        int nFontId, 
		KOutputTextParam* pParam,
        //## Ҫ������ַ�����
        const char* psText, 
        //## Ҫ������ַ����ĳ���(BYTE)��
        //## ��nCount���ڵ���0ʱ���ַ������Բ���������Ľ�����������ʾ����ַ��Ľ�����
        //## ��nCountС��0ʱ����ʾ���ַ�������'\0'��β�����м䲻����'\0'���ַ���
        //## Ĭ��ֵΪ-1��		
        int nCount = KRF_ZERO_END, 
        //##Documentation
        //## �Զ����е��п����ƣ������ֵС��һ��ȫ���ַ���������Զ����д���Ĭ��ֵΪ0���Ȳ����Զ����д���
        int nLineWidth = 0) = 0;

    //## �ͷ�һ���������
    virtual void ReleaseAFont(
        //## ��������id
        int nId) = 0;
	
//---------ͼ��Դ���---------------

    //## ����ڴ�ͼ�ζ���ʧ�ܣ��򷵻�0�����򷵻ظ���ͼ������ת�����ɵ���ֵid��
    virtual unsigned int CreateImage(
        //## ͼ�ε����֡�
        const char* pszName, 
        //## ͼ�κ��
        int nWidth, 
        //## ͼ���ݿ�
        int nHeight, 
        //## ͼ������
        int nType) = 0;

    //## ָ���ͷ�ĳ��ͼ��Դ��
    virtual void FreeImage(
        //## ͼ���ļ���/ͼ������
        const char* pszImage 
        ) = 0;

    //## �ͷ�ȫ����ͼ�ζ���
    virtual void FreeAllImage() = 0;

    //## ��ȡͼ��������ݻ����������ؿգ����ʾʧ��
    virtual void* GetBitmapDataBuffer(
        //## ͼ����
        const char* pszImage,
		//���ڻ�ȡͼ�����ݻ������������Ϣ���ݣ���������ָ�룬�������Щ��Ϣ��
		KBitmapDataBuffInfo* pInfo) = 0;

	//##�ͷŶ�(ͨ��GetBitmapDataBuffer���û�ȡ�õ�)ͼ��������ݻ������Ŀ���
	virtual void ReleaseBitmapDataBuffer(
		//## ͼ����
		const char* pszImage,
		//ͨ��GetBitmapDataBuffer���û�ȡ�õ�ͼ��������ݻ�����ָ��
		void* pBuffer) = 0;

    //## ��ȡĳ��ͼ����Ϣ��
    virtual bool GetImageParam(
        //##Documentation
        //## ͼ�ε���Դ�ļ���/ͼ����
        const char* pszImage,
        //##Documentation
        //## ͼ����Ϣ�洢�ṹ��ָ��
        KImageParam* pImageData,
		//##Documentation
        //## ͼ������
        int nType) = 0;
	
    //## ��ȡĳ��ͼ��ĳ֡����Ϣ
	virtual bool GetImageFrameParam(
        //## ָ�򱣴�ͼ����Դ�ļ���/ͼ�����Ļ�����
        const char* pszImage,
		//ͼ��֡����
		int		nFrame,
        //## ֡ͼ�����������ͼ�ε�ƫ��
		KRPosition2* pOffset,
		//## ֡ͼ�δ�С
		KRPosition2* pSize,
        //## ͼ��Դ����
		int nType) = 0;

    //## ��ȡĳ��ͼ��ĳ֡��ָ������alphaֵ��������β����ڣ������������ͼ�η�Χ֮���򷵻�0��
    virtual int GetImagePixelAlpha(
        //## ͼ����Դ�ļ���/ͼ����
        const char* pszImage, 
        //## ͼ�ε�֡������
        int nFrame, 
        //## �����ͼ�к�����
        int nX, 
        //## �����ͼ��������
        int nY,
        //## ͼ������
        int nType) = 0;

    //## ����ͼ�ζ�̬����ƽ�������
    virtual void SetImageStoreBalanceParam(
        //## ����ͼ�ε���Ŀ��ƽ��ֵ��
        int nNumImage, 
        //## ÿ���ٴ�����ͼ�ζ������һ��ƽ���顣
        unsigned int uCheckPoint = 1000) = 0;

    //## ����ͼ�ε��ļ��������ļ�ֻ��ISI_T_BITMAP16��ʽ��ͼ����Ч������ָ����ͼ���Ƿ�ɹ����浽�ļ���
    virtual bool SaveImage(
        //## �����ļ������֡�
        const char* pszFile, 
        //## ͼ����
        const char* pszImage, 
        //## Ŀ��ͼ���ļ�������
        int nFileType) = 0;


//---------���Ʋ���---------------

    //## ����ͼԪ
    virtual void DrawPrimitives(
        //## ���Ƶ�ͼԪ����Ŀ
        int nPrimitiveCount,
        //## ����ͼԪ�Ľṹ������
        KRepresentUnit* pPrimitives, 
        //## Primitive���ͣ�ȡֵ����ö��ֵREPRESENT_UNIT_TYPE
        unsigned int uGenre, 
        //## ͼԪ���Ʋ����ṩ�������Ƿ�Ϊ��ֱ�����ߵĵ�ƽ�����ꡣ������ǣ���ͼԪ���Ʋ����ṩ����������ά�ռ����ꡣ
        int bSinglePlaneCoord) = 0;

    //## ����ͼԪ��ָ����ͼ��֮��
	virtual void DrawPrimitivesOnImage(
        //## ���Ƶ�ͼԪ����Ŀ
        int nPrimitiveCount,
        //## ����ͼԪ�Ľṹ������
        KRepresentUnit* pPrimitives, 
        //## Primitive���ͣ�ȡֵ����ö��ֵREPRESENT_UNIT_TYPE
        unsigned int uGenre, 
        //## ͼ����
        const char* pszImage, 
        //## Ŀ��ͼ�ε�id
        unsigned int uImage,
		short& nImagePosition)	= 0;

	//## ���ͼ������
	virtual void ClearImageData(
        //## ͼ����
        const char* pszImage, 
        //## Ŀ��ͼ�ε�id
        unsigned int uImage,
		short nImagePosition)	= 0;

    //## �����Ӿ��۲��Ŀ�Ľ��㡣
    virtual void LookAt(int nX, int nY, int nZ) = 0;

    //## ���ƻ�ͼ�豸�ϵ�ͼ��ͼ�ζ���
    //## Ŀ��ͼ����Դ���������Ŀǰֻ����ISI_T_BITMAP16�����Ŀ��ͼ�ζ��󲻴��ڻ��߸���ʧ���򷵻�false��
    virtual bool CopyDeviceImageToImage(
        //## ͼ�ε����֡�
        const char* pszName, 
        //## ��ͼ�豸ͼ�θ��Ʒ�Χ�����Ͻǵ������
        int nDeviceX, 
        //## ��ͼ�豸ͼ�θ��Ʒ�Χ�����Ͻǵ�������
        int nDeviceY, 
        //## Ŀ��ͼ�θ��Ƶ���Χ�����Ͻǵ������
        int nImageX, 
        //## Ŀ��ͼ�θ��Ƶ���Χ�����Ͻǵ�������
        int nImageY, 
        //## ���Ʒ�Χ�ĺ��
        int nWidth, 
        //## ���Ʒ�Χ�ĺ��
        int nHeight) = 0;

	//## ��ʼһ�ֻ��Ʋ���
	virtual bool RepresentBegin(
		//## �Ƿ�����豸�ϵ�ǰ��ͼ�Ρ�
		int bClear, 
		//## ���bClearΪ��0ֵ����Colorָ����ʲô��ɫֵ������豸ԭ����ͼ�Ρ�
		unsigned int Color) = 0;

	//## ����һ�ֻ��Ʋ���
	virtual void RepresentEnd() = 0;

	//��ͼ/��ͼ�豸���� ת��Ϊ�ռ�����
	virtual void ViewPortCoordToSpaceCoord(
		int& nX,	//���룺��ͼ/��ͼ�豸�����x�����������ռ������x��
		int& nY,	//���룺��ͼ/��ͼ�豸�����y�����������ռ������y��
		int  nZ		//���������õ��Ŀռ������z��
		) = 0;
	
	//## ���ù�����Ϣ
	virtual void SetLightInfo(
		//## ��ǰ9���������Ͻǵ�x����
		int nX,
		//## ��ǰ9���������Ͻǵ�y����
		int nY,
		//## ��ǰ9������Ĺ�����Ϣ��DWORD���飬��ʾ������ڵ��Ϲ�������ֵ
		//## ���pLightingΪNULL����ʾ�������մ���
		unsigned int *pLighting) = 0;
	
	//## ���ñ���ģ��ѡ��
	virtual void SetOption(
		//## ѡ������
		RepresentOption eOption,
		//## ѡ���Ƿ��
		bool bOn) = 0;

	//## �ҽ�Ƕ�����ֵ�ͼƬ��ʾ [wxb 2003-6-20]
	virtual long AdviseRepresent(IInlinePicEngineSink*) = 0;
	virtual long UnAdviseRepresent(IInlinePicEngineSink*) = 0;
	
	virtual bool SaveScreenToFile(const char* pszName, ScreenFileType eType, unsigned int nQuality) = 0;
	
	// nGammaȡֵ��0��100֮��
	virtual void SetGamma(int nGamma) = 0;

	//����ƫɫ�б�
	virtual unsigned int SetAdjustColorList(
			unsigned int* puColorList,	//ƫɫ����ɫ�б�ÿ����ɫΪһ��unsigned int����
										//��32bit����0RGB�ĸ�ʽ��ʾ��ɫ��ÿ������8bit��
			unsigned int uCount			//��ɫ�б�����ɫ������Ŀ
			) = 0;
};



#ifndef _REPRESENT_INTERNAL_SIGNATURE_
	//----�ӿں����ĵ���----
	//=========����һ��iRepresentShell�ӿڵ�ʵ��===============
	extern "C" __declspec(dllimport) iRepresentShell* CreateRepresentShell();
	//=========����ģ�����ܣ��ж��Ƿ��Ƽ�ʹ��===============
	//��ѡ���ڵĽӿں���
	//extern "C" __declspec(dllimport) bool RepresentIsModuleRecommended();
#endif

//------ģ��Ľӿں�����ԭ�͵Ķ���------
typedef iRepresentShell*					(*fnCreateRepresentShell)();
typedef bool								(*fnRepresentIsModuleRecommended)();
//------�ӿں����ĺ������ַ���----------
#define	FN_CREATE_REPRESENT_SHELL			"CreateRepresentShell"
#define	FN_REPRESENT_IS_MODULE_RECOMMENDED	"RepresentIsModuleRecommended"


#endif /* IREPRESENTSHELL_H_HEADER_INCLUDED_C2311ED3 */
