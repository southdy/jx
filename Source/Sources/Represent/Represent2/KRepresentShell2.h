/*****************************************************************************************
//  ����ģ��Ķ���ӿڵĶ�ά�汾ʵ�֡�
//	Copyright : Kingsoft 2002-2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-11-11
------------------------------------------------------------------------------------------
*****************************************************************************************/

#ifndef KREPRESENTSHELL2_H_HEADER_INCLUDED_C2314D70
#define KREPRESENTSHELL2_H_HEADER_INCLUDED_C2314D70
#include "../iRepresent/iRepresentShell.h"
#include "KImageStore2.h"
#include "../../Engine/Src/KEngine.h"
#include "../iRepresent/Text/TextProcess.h"

struct KImageParam;
struct iFont;

//##ModelId=3DB6678F0243
//##Documentation
//## ����ģ��Ķ���ӿڵĶ�ά�汾ʵ�֡�
class KRepresentShell2 : public iRepresentShell
{
public:
	//##ModelId=3DD20C90004D
	KRepresentShell2();
//=======================================
//=====iRepresentShell����Ľӿں���=====
//=======================================

    //##ModelId=3DCA72E102FE
    //## �ͷ����ٽӿڶ���
    //##Documentation
    //## �ͷ����ٽӿڶ���
    void Release();

//---------��ͼ�豸���---------------

    //##ModelId=3DB6692300B3
    //## ������ͼ�豸�������ͼ������
    //## ֱ�ӵ���Reset������
    bool Create(
        //##Documentation
        //## �豸�����ڣ������λ�����ص㣩
        int nWidth, 
        //##Documentation
        //## �豸�����ڣ��ݿ���λ�����ص㣩
        int nHeight, 
        //##Documentation
        //## �Ƿ��ռȫ��Ļ
        bool bFullScreen);

    //##ModelId=3DB69EC0023A
    //##Documentation
    //## ���û�ͼ�豸
    bool Reset(
        //##Documentation
        //## �豸�����ڣ������λ�����ص㣩
        int nWidth, 
        //##Documentation
        //## �豸�����ڣ��ݿ���λ�����ص㣩
        int nHeight, 
        //##Documentation
        //## �Ƿ��ռȫ��Ļ
        bool bFullScreen);

//---------�������������---------------

    //##ModelId=3DCA0B230317
    //##Documentation
    //## ����һ���������
	bool CreateAFont(
        //##Documentation
        //## �ֿ��ļ�����
        const char* pszFontFile, 
        //##Documentation
        //## �ֿ�ʹ�õ��ַ����뼯��
        CHARACTER_CODE_SET CharaSet, 
        //##Documentation
        //## �������id.
        int nId);

    //##ModelId=3DCA0BAE00E4
    //##Documentation
    //## ������֡�
    void OutputText(
        //##Documentation
        //## ʹ�õ��������id��
        int nFontId, 
        //##Documentation
        //## Ҫ������ַ�����
        const char* psText, 
        //##Documentation
        //## Ҫ������ַ����ĳ���(BYTE)��
        //## ��nCount���ڵ���0ʱ���ַ������Բ���������Ľ�����������ʾ����ַ��Ľ�����
        //## ��nCountС��0ʱ����ʾ���ַ�������'\0'��β�������ݽ����ַ���ȷ������ַ����ĳ��ȡ�
        //## Ĭ��ֵΪ-1��
        int nCount, 
        //##Documentation
        //## �ַ�����ʾ�������X���������ֵΪKF_FOLLOW��
        //## ����ַ����������ϴ��ַ��������λ��֮��
        //## Ĭ��ֵΪKF_FOLLOW��
        int nX, 
        //##Documentation
        //## �ַ�����ʾ�������Y, �������ֵΪKF_FOLLOW��
        //## ���ַ�����ǰһ������ַ�����ͬһ�е�λ�á�
        //## Ĭ��ֵΪKF_FOLLOW��
        int nY, 
        //##Documentation
        //## �ַ�����ʾ��ɫ��Ĭ��Ϊ��ɫ����32bit����ARGB�ĸ�
        //## ʽ��ʾ��ɫ��ÿ������8bit��
        unsigned int Color, 
        //##Documentation
        //## �Զ����е��п����ƣ������ֵС��һ��ȫ���ַ���������Զ����д���Ĭ��ֵΪ0���Ȳ����Զ����д���
        int nLineWidth = 0,
		int nZ = TEXT_IN_SINGLE_PLANE_COORD,
		//�ֵı�Ե��ɫ
		unsigned int BorderColor = 0);

	//##ModelId=3DB655B2000E
    //##Documentation
    //## ������֡�
    int OutputRichText(
		//##Documentation
        //## ʹ�õ��������id��
        int nFontId, 
		KOutputTextParam* pParam,
		//##Documentation
        //## Ҫ������ַ�����
        const char* psText, 
        //##Documentation
        //## Ҫ������ַ����ĳ���(BYTE)��
        //## ��nCount���ڵ���0ʱ���ַ������Բ���������Ľ�����������ʾ����ַ��Ľ�����
        //## ��nCountС��0ʱ����ʾ���ַ�������'\0'��β�����м䲻����'\0'���ַ���
        //## Ĭ��ֵΪ-1
        int nCount = KRF_ZERO_END, 
        //##Documentation
        //## �Զ����е��п����ƣ������ֵС��һ��ȫ���ַ���������Զ����д���Ĭ��ֵΪ0���Ȳ����Զ����д���
        int nLineWidth = 0);

    //## ����ָ���������ַ�����������ַ�ƫ��
    int LocateRichText(
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
        int nLineWidth = 0);

    //##ModelId=3DCA0B8102F3
    //##Documentation
    //## �ͷ�һ���������
    void ReleaseAFont(
        //##Documentation
        //## ��������id
        int nId);

//---------ͼ��Դ���---------------

    //##ModelId=3DCD8DEA01BB
    //##Documentation
    //## ����ڴ�ͼ�ζ���ʧ�ܣ��򷵻�0�����򷵻ظ���ͼ������ת�����ɵ���ֵid��
    unsigned int CreateImage(
        //##Documentation
        //## ͼ�ε����֡�
        const char* pszName, 
        //##Documentation
        //## ͼ�κ��
        int nWidth, 
        //##Documentation
        //## ͼ���ݿ�
        int nHeight,
		//##Documentation
        //## ͼ������
        int nType);

    //##ModelId=3DCD8EF60316
    //##Documentation
    //## ָ���ͷ�ĳ��ͼ��Դ��
    void FreeImage(
		//##Documentation
        //## ͼ���ļ���/ͼ������
        const char* pszImage);

    //##ModelId=3DCD8E9200E8
    //##Documentation
    //## �ͷ�ȫ����ͼ�ζ���
    void FreeAllImage();

    //##ModelId=3DCD8FA900EE
    //##Documentation
    //## ��ȡͼ��������ݻ�����
    void* GetBitmapDataBuffer(
        //##Documentation
        //## ͼ����
        const char* pszImage,
		//���ڻ�ȡͼ�����ݻ������������Ϣ���ݣ���������ָ�룬�������Щ��Ϣ��
		KBitmapDataBuffInfo* pInfo);

	//##�ͷŶ�(ͨ��GetBitmapDataBuffer���û�ȡ�õ�)ͼ��������ݻ������Ŀ���
	void ReleaseBitmapDataBuffer(
		//## ͼ����
		const char* pszImage,
		//ͨ��GetBitmapDataBuffer���û�ȡ�õ�ͼ��������ݻ�����ָ��
		void* pBuffer);

    //##ModelId=3DCA6EBC000F
    //##Documentation
    //## ��ȡĳ��ͼ����Ϣ��
    bool GetImageParam(
        //##Documentation
        //## ͼ�ε���Դ�ļ���/ͼ����
        const char* pszImage,
        //##Documentation
        //## ͼ����Ϣ�洢�ṹ��ָ��
        KImageParam* pImageData,
		//##Documentation
		//## ͼ������
		int nType);

    //##Documentation
    //## ��ȡĳ��ͼ��ĳ֡����Ϣ
	bool GetImageFrameParam(
        //##Documentation
        //## ָ�򱣴�ͼ����Դ�ļ���/ͼ�����Ļ�����
        const char* pszImage,
		//##Documentation
		//ͼ��֡����
		int		nFrame,
        //##Documentation
        //## ֡ͼ�����������ͼ�ε�ƫ��
		KRPosition2* pOffset,
		//## ֡ͼ�δ�С
		KRPosition2* pSize,
        //##Documentation
        //## ͼ��Դ����
		int nType);

    //##ModelId=3DCA72620157
    //##Documentation
    //## ��ȡĳ��ͼ��ĳ֡��ָ������alphaֵ��������β����ڣ������������ͼ�η�Χ֮���򷵻�0��
    int GetImagePixelAlpha(
        //##Documentation
        //## ͼ����Դ�ļ���/ͼ����
        const char* pszImage, 
        //##Documentation
        //## ͼ�ε�֡������
        int nFrame, 
        //##Documentation
        //## �����ͼ�к�����
        int nX, 
        //##Documentation
        //## �����ͼ��������
        int nY,
		//##Documentation
        //## ͼ������
        int nType);

    //##ModelId=3DCD90F30011
    //##Documentation
    //## ����ͼ�ζ�̬����ƽ�������
    void SetImageStoreBalanceParam(
        //##Documentation
        //## ����ͼ�ε���Ŀ��ƽ��ֵ��
        int nNumImage, 
        //##Documentation
        //## ÿ���ٴ�����ͼ�ζ������һ��ƽ���顣
        unsigned int uCheckPoint = 10000);

    //##ModelId=3DCD90910361
    //##Documentation
    //## ����ͼ�ε��ļ��������ļ�ֻ��ISI_T_BITMAP16��ʽ��ͼ����Ч������ָ����ͼ���Ƿ�ɹ����浽�ļ���
    bool SaveImage(
        //##Documentation
        //## �����ļ������֡�
        const char* pszFile, 
        //##Documentation
        //## ͼ����
        const char* pszImage, 
        //##Documentation
        //## Ŀ��ͼ���ļ�������
        int nFileType);
    
	
//---------���Ʋ���---------------
	
	//##ModelId=3DB69FE401DA
    //##Documentation
    //## ����ͼԪ
    void DrawPrimitives(
        //##Documentation
        //## ���Ƶ�ͼԪ����Ŀ
        int nPrimitiveCount,
        //##Documentation
        //## ����ͼԪ�Ľṹ������
        KRepresentUnit* pPrimitives, 
        //##Documentation
        //## Primitive���ͣ�ȡֵ����ö��ֵREPRESENT_UNIT_TYPE
        unsigned int uGenre,
        //##Documentation
        //## ͼԪ���Ʋ����ṩ�������Ƿ�Ϊ��ֱ�����ߵĵ�ƽ�����ꡣ������ǣ���ͼԪ���Ʋ����ṩ����������ά�ռ����ꡣ
        int bSinglePlaneCoord);

	void DrawPrimitivesOnImage(
		//##Documentation
        //## ���Ƶ�ͼԪ����Ŀ
        int nPrimitiveCount,
        //##Documentation
        //## ����ͼԪ�Ľṹ������
        KRepresentUnit* pPrimitives, 
        //##Documentation
        //## Primitive���ͣ�ȡֵ����ö��ֵREPRESENT_UNIT_TYPE
        unsigned int uGenre, 
		//##Documentation
        //## ͼ����
        const char* pszImage, 
        //##Documentation
        //## Ŀ��ͼ�ε�id
        unsigned int uImage,
		short&	nImagePosition);

	//##Documentation
	//## ���ͼ������
	void ClearImageData(
		//##Documentation
        //## ͼ����
        const char* pszImage, 
        //##Documentation
        //## Ŀ��ͼ�ε�id
        unsigned int uImage,
		short nImagePosition);

    //##ModelId=3DC0A08D0085
    //##Documentation
    //## �����Ӿ��۲��Ŀ�Ľ��㡣
    //## (�����������������ʹ����z=0ƽ����)��
    //## �ѵ������������Ϊ��ͼ�豸���ĵ��Ӧ�����꣬��������������ͼ�豸���Ͻǵ��Ӧ�����꣺
    //## m_nLeft = nX - Width(��ͼ�豸) / 2
    //## m_nTop = nY - Height(��ͼ�豸) / 2 - nZ
    void LookAt(int nX, int nY, int nZ);

    //##ModelId=3DD00EEE0149
    //##Documentation
    //## ���ƻ�ͼ�豸�ϵ�ͼ��ͼ�ζ���
    //## Ŀ��ͼ����Դ���������Ŀǰֻ����ISI_T_BITMAP16�����Ŀ��ͼ�ζ��󲻴��ڻ��߸���ʧ���򷵻�false��
    bool CopyDeviceImageToImage(
        //##Documentation
        //## ͼ�ε����֡�
        const char* pszName, 
        //##Documentation
        //## ��ͼ�豸ͼ�θ��Ʒ�Χ�����Ͻǵ������
        int nDeviceX, 
        //##Documentation
        //## ��ͼ�豸ͼ�θ��Ʒ�Χ�����Ͻǵ�������
        int nDeviceY, 
        //##Documentation
        //## Ŀ��ͼ�θ��Ƶ���Χ�����Ͻǵ������
        int nImageX, 
        //##Documentation
        //## Ŀ��ͼ�θ��Ƶ���Χ�����Ͻǵ�������
        int nImageY, 
        //##Documentation
        //## ���Ʒ�Χ�ĺ��
        int nWidth, 
        //##Documentation
        //## ���Ʒ�Χ�ĺ��
        int nHeight);

	//##ModelId=3DD20C45002A
	//##Documentation
	//## ��ʼһ�ֻ��Ʋ���
	bool RepresentBegin(
		//##Documentation
		//## �Ƿ�����豸�ϵ�ǰ��ͼ�Ρ�
		int bClear, 
		//##Documentation
		//## ���bClearΪ��0ֵ����Colorָ����ʲô��ɫֵ������豸ԭ����ͼ�Ρ�
		unsigned int Color);

	//##ModelId=3DD20C450066
	//##Documentation
	//## ����һ�ֻ��Ʋ���
	void RepresentEnd();

	//��ͼ/��ͼ�豸���� ת��Ϊ�ռ�����
	void ViewPortCoordToSpaceCoord(
		int& nX,	//���룺��ͼ/��ͼ�豸�����x�����������ռ������x��
		int& nY,	//���룺��ͼ/��ͼ�豸�����y�����������ռ������y��
		int  nZ		//���������õ��Ŀռ������z��
		);
	
		//## ���ù�����Ϣ
	virtual void SetLightInfo(
		//## ��ǰ9���������Ͻǵ�x����
		int nX,
		//## ��ǰ9���������Ͻǵ�y����
		int nY,
		//## ��ǰ9������Ĺ�����Ϣ��DWORD���飬��ʾ������ڵ��Ϲ�������ֵ
		//## ���pLightingΪNULL����ʾ�������մ���
		unsigned int *pLighting){}
	
	//## ���ñ���ģ��ѡ��
	virtual void SetOption(
		//## ѡ������
		RepresentOption eOption,
		//## ѡ���Ƿ��
		bool bOn){}
	virtual HRESULT AdviseRepresent(IInlinePicEngineSink*);
	virtual HRESULT UnAdviseRepresent(IInlinePicEngineSink*);
	
	virtual bool SaveScreenToFile(const char* pszName, ScreenFileType eType, unsigned int nQuality);
	virtual void SetGamma(int nGamma){}

	//����ƫɫ�б�
	virtual unsigned int SetAdjustColorList(
			unsigned int* puColorList,	//ƫɫ����ɫ�б�ÿ����ɫΪһ��unsigned int����
										//��32bit����0RGB�ĸ�ʽ��ʾ��ɫ��ÿ������8bit��
			unsigned int uCount			//��ɫ�б�����ɫ������Ŀ
			);
private:

    //##ModelId=3DB68CEB0350
    class KRSFontItem
   {
      public:
        //##ModelId=3DB68D04025D
        //##Documentation
        //## ��������id��һ��Ӧ��ʱ����id����������ִ�С��ֵ�����������Ӧ����⡣
        int nId;

        //##ModelId=3DB68D1D0323
        //##Documentation
        //## ��������ָ�롣
        iFont* pFontObj;

    };

    //##ModelId=3DCFECA1036D
    enum RS2_FONT_ITEM_PARAM { 
        //##Documentation
        //## �ӿ�������ά��������������Ŀ���ٹ�����Ŀʱ�������������ʧ�ܡ�
        RS2_MAX_FONT_ITEM_NUM = 5 };

private:

    //##ModelId=3DB66AD30124
    KCanvas m_Canvas;

    //##ModelId=3DB66AEA03BD
    KDirectDraw m_DirectDraw;

    //##ModelId=3DB6A25A0137
    //##Documentation
    //## ͼ����Դά��ģ�����
    KImageStore2 m_ImageStore;

    //##ModelId=3DCA0E4902B3
    int m_nLeft;

    //##ModelId=3DCA0E520312
    int m_nTop;

	//##ModelId=3DB68D8701FE
    //##Documentation
    //## �������ļ��ϡ�
    KRSFontItem m_FontTable[RS2_MAX_FONT_ITEM_NUM];

	KTextProcess	m_TextProcess;

private:

	//##ModelId=3DD20C900089
	~KRepresentShell2();
    //##ModelId=3DCFED410049
    //##Documentation
    //## �ռ����굽��ά(��ʾ��)ƽ������ı任
    void CoordinateTransform(
        //##Documentation
        //## ������ά����x������������ά����x(ˮƽ)����
        int& nX, 
        //##Documentation
        //## ������ά����y������������ά����y(��ֱ)����
        int& nY, 
        //##Documentation
        //## ������ά����z����
        int nZ);
};


#endif /* KREPRESENTSHELL2_H_HEADER_INCLUDED_C2314D70 */
