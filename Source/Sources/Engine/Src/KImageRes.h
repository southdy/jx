#ifndef __KIMAGERES_H__
#define __KIMAGERES_H__

#include "KSprite.h"

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

//##ModelId=3DC0F495038B
//##Documentation
//## ͼ�����Ͷ���
enum KIS_IMAGE_TYPE
{
    //##Documentation
    //## 16λ��֡λͼ��
    //## ISI_T_BITMAP16�ĸ�ʽΪ D3DFMT_R5G6B5 ���� D3DFMT_X1R5G5B5
    //## ������Ϊ���ָ�ʽ��iImageStore�ڲ�ȷ��������ͨ������iImageStore::IsBitmapFormat565��֪���������ָ�ʽ��
    ISI_T_BITMAP16, 
    //##Documentation
    //##  spr��ʽ�Ĵ�alphaѹ��ͼ�Σ�����֡
    ISI_T_SPR,
	//##Documentation
    //##  �ɱ��޸ĵģ�spr��ʽͼ�Σ�������̬���ش���
	ISI_T_SPR_CUSTOM,
};

class KImageRes
{
public:

public:
	KImageRes();
	virtual ~KImageRes();

	// ����Ա������Ϊ��ʼֵ
	virtual void ResetVar() = 0;

	// �����ڴ���Դ
	virtual bool CreateImage(const char* szImage, int nWidth, int nHeight, unsigned int nType) = 0;
	// ���ļ�������Դ
	virtual bool LoadImage(char* szImage, unsigned int nType) = 0;

	// �ͷ��ڴ�
	virtual void Release() = 0;
	
	int GetWidth(){ return (int)m_nWidth; }
	int GetHeight(){ return (int)m_nHeight; }

//private:
public:
	unsigned int		m_nType;					// ��Դ����
	unsigned int		m_nWidth;					// ��Դ���
	unsigned int		m_nHeight;					// ��Դ�߶�
	
	unsigned long m_nMemUsed;				// �ڴ�ռ������,��λ�ֽ�
	bool		m_bLastFrameUsed;			// ��һ֡�Ƿ�ʹ��
};

class ImageResBmp : public KImageRes
{
public:

public:
	ImageResBmp();
	~ImageResBmp();

	// ����Ա������Ϊ��ʼֵ
	virtual void ResetVar();

	// �����ڴ���Դ
	virtual bool CreateImage(const char* szImage, int nWidth, int nHeight, unsigned int nType);
	// ���ļ�������Դ
	virtual bool LoadImage(char* szImage, unsigned int nType);

	// �ͷ��ڴ�
	virtual void Release();

//private:
public:
	BYTE *m_pData;		// ͼ������

private:
	bool LoadJpegFile(char* szImage);
	
};

class ImageResSpr : public KImageRes
{
public:

public:
	ImageResSpr();
	~ImageResSpr();

	// ����Ա������Ϊ��ʼֵ
	virtual void ResetVar();

	// �����ڴ���Դ
	virtual bool CreateImage(const char* szImage, int nWidth, int nHeight, unsigned int nType);
	// ���ļ�������Դ
	virtual bool LoadImage(char* szImage, unsigned int nType);

	// �ͷ��ڴ�
	virtual void Release();

	unsigned int GetCenterX(){ return m_Header.CenterX; }
	unsigned int GetCenterY(){ return m_Header.CenterY; }
	int GetFrameNum(){ return m_Header.Frames; }
	unsigned int GetDirections(){ return m_Header.Directions; };
	unsigned int GetInterval(){ return m_Header.Interval; };
	
	// ȡ�õ�nFrame֡ͼ��x��y�����ϵ����ص�alphaֵ
	int GetPixelAlpha(int nFrame, int x, int y);

	// ׼��һ֡����
	bool PrepareFrameData(char *pszImage, int nFrame);

//private:
public:
	bool		m_bInPackage;				// ���spr�Ƿ��ڰ���
	KPAL24*		m_pPal24;					// ��ɫ��
	WORD*		m_pPal16;					// 4444��ɫ��

	SPRHEAD		m_Header;					// sprͷ����Ϣ
	SPRFRAME**	m_pFrameInfo;				// ֡����ͼӳ����Ϣ����ָ��

private:
	bool LoadSprFile(char* szImage);
};

#endif