#ifndef __TEXTURERES_H__
#define __TEXTURERES_H__

struct TextureInfo
{
	uint32	nWidth;						// ��ͼ���
	uint32	nHeight;					// ��ͼ�߶�
	LPDIRECT3DTEXTURE9 pTexture;		// ��ͼָ��
	uint32	nFrameX;					// ����һ֡ͼ���ϵ����Ͻ�λ��
	uint32	nFrameY;					// ����һ֡ͼ���ϵ����Ͻ�λ��
	int32	nFrameWidth;				// ��ͼ���������ݿ��
	int32	nFrameHeight;				// ��ͼ���������ݸ߶�
};

struct FrameToTexture
{
	int32	nTexNum;					// ��һ֡���Ϊ������ͼ
	int32	nWidth;						// ��һ֡�Ŀ��
	int32	nHeight;					// ��һ֡�ĸ߶�
	int32	nOffX;						// ���������spr��X����ƫ��
	int32	nOffY;						// ���������spr��Y����ƫ��
	TextureInfo texInfo[4];				// ��ͼ��Ϣ���飬һ����֣����4����ͼ
	int32	nRawDataLen;				// ԭʼ���ݳ���
	BYTE	*pRawData;					// ��һ֡��ԭʼ����
	void	*pFrame;					// ����SprGetFrame���ص�ָ�룬��SprReleaseFrameʹ��
};

class TextureRes
{
public:

public:
	TextureRes();
	virtual ~TextureRes();

	// ����Ա������Ϊ��ʼֵ
	virtual void ResetVar() = 0;

	// �����ڴ���Դ
	virtual bool CreateImage(const char* szImage, int nWidth, int nHeight, uint32 nType) = 0;
	// ���ļ�������Դ
	virtual bool LoadImage(char* szImage, uint32 nType) = 0;

	// �ͷ��ڴ�
	virtual void Release() = 0;
	
	virtual bool InvalidateDeviceObjects(){return true;}
	virtual bool RestoreDeviceObjects(){return true;}

	virtual bool ReleaseAFrameData() = 0;

	int32 GetWidth(){ return (int32)m_nWidth; }
	int32 GetHeight(){ return (int32)m_nHeight; }

//private:
public:
	uint32		m_nType;					// ��Դ����

	D3DFORMAT	m_eTextureFormat;			// ��ͼ��ʽ
	int32		m_nWidth;					// ��Դ���
	int32		m_nHeight;					// ��Դ�߶�
	
	// ��������ͳ��
	unsigned long m_nTexMemUsed;			// ��ͼ�ڴ�ռ������,��λ�ֽ�
	unsigned long m_nSprMemUsed;			// ��ͼ�ڴ�ռ������,��λ�ֽ�
	bool		m_bLastFrameUsed;			// ��һ֡�Ƿ�ʹ��

private:
	
	friend class TextureResMgr;
};

class TextureResBmp : public TextureRes
{
public:

public:
	TextureResBmp();
	~TextureResBmp();

	// ����Ա������Ϊ��ʼֵ
	virtual void ResetVar();

	// �����ڴ���Դ
	virtual bool CreateImage(const char* szImage, int nWidth, int nHeight, uint32 nType);
	// ���ļ�������Դ
	virtual bool LoadImage(char* szImage, uint32 nType);

	// �ͷ��ڴ�
	virtual void Release();

	virtual bool InvalidateDeviceObjects();
	virtual bool RestoreDeviceObjects();

	virtual bool ReleaseAFrameData()
	{
		Release();
		return false;
	}

	// �������ݻ�����
	bool LockData(void** pData, int32& nPitch);
	// �������ݻ�����
	void UnLockData();

//private:
public:
	FrameToTexture		m_FrameInfo;		// ֡����ͼӳ����Ϣ
	LPDIRECT3DTEXTURE9	m_pSysMemTexture;	// ϵͳ�ڴ��еĶ�Ӧ��ͼ�����ڸ��º�����Դ��е���ͼ

private:
	bool LoadJpegFile(char* szImage);
	
};

class TextureResSpr : public TextureRes
{
public:

public:
	TextureResSpr();
	~TextureResSpr();

	// ����Ա������Ϊ��ʼֵ
	virtual void ResetVar();

	// �����ڴ���Դ
	virtual bool CreateImage(const char* szImage, int nWidth, int nHeight, uint32 nType);
	// ���ļ�������Դ
	virtual bool LoadImage(char* szImage, uint32 nType);

	// �ͷ��ڴ�
	virtual void Release();

	virtual bool ReleaseAFrameData();

	bool PrepareFrameData(const char* szImage, int32 nFrame, bool bPrepareTex);
	// ȡ��nFrame֡�ĵ�nIdx����ͼ�����ݲ�ͬ���nIdxȡֵΪ1-4
	LPDIRECT3DTEXTURE9 GetTexture(int32 nFrame, int nIdx);

	uint32 GetCenterX(){ return m_nCenterX; }
	uint32 GetCenterY(){ return m_nCenterY; }
	uint32 GetFrameNum(){ return m_nFrameNum; }
	uint32 GetDirections(){ return m_nDirections; };
	uint32 GetInterval(){ return m_nInterval; };
	
	// ȡ�õ�nFrame֡ͼ��x��y�����ϵ����ص�alphaֵ
	int32 GetPixelAlpha(int32 nFrame, int32 x, int32 y);

//private:
public:
	uint32		m_nCenterX;
	uint32		m_nCenterY;
	uint32		m_nColors;
	uint32		m_nDirections;
	uint32		m_nInterval;

	KPAL24*		m_pPal24;					// ��ɫ��
	WORD*		m_pPal16;					// 4444��ɫ��
	int32		m_nFrameNum;				// ��Դ֡��

	FrameToTexture*		m_pFrameInfo;		// ֡����ͼӳ����Ϣ����ָ��
	char*		m_pHeader;					// sprͷ����Ϣ�������Ϊ����ζ�ŷ�֡ѹ��������Ϊ����ѹ��
	SPROFFS*	m_pOffset;					// ֡ƫ������Ϣ

private:
	bool LoadSprFile(char* szImage);
	void CreateTexture16Bit(const char* szImage, int32 nFrame);
	void CreateTexture32Bit(uint32 nFrame);

	// �����ͼ������ͼ���Ϊ1��4���Գ��������ͼ�ռ�
	int SplitTexture(uint32 nFrame);
	
};

#endif