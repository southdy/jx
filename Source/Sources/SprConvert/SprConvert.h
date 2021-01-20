#ifndef __SPRCONVERT_H__
#define __SPRCONVERT_H__

struct SprTexInfoHead
{
	BYTE	Comment[4];		// ע������(STI\0)
	WORD	nTexNum;		// ��ͼ��Ŀ
	WORD	nFrameNum;		// �����ɵ�֡��
};

struct TexSize
{
	WORD	nWidth;
	WORD	nHeight;
};

struct FrameTexInfo
{
	WORD	nTexIndex;		// ��һ֡���ڵڼ�����ͼ
	WORD	nTexX;			// ����ͼ�ϵ�λ�õ�x����
	WORD	nTexY;			// ����ͼ�ϵ�λ�õ�y����
};

struct TextureSize
{
	int		nWidth;
	int		nHeight;
	int		nFrameWidth;
	int		nFrameHeight;
	int		nFrameNum;		// �����ɵ�֡��
	WORD	nRealFrameNum;  // ʵ�ʴ�ŵ�֡��
};

class SprConvert
{
private:
	SPRHEAD*	m_pHeader;
	PBYTE		m_pFileData;
	SPROFFS* 	m_pOffset;
	PBYTE		m_pSprite;
	bool		m_bReadOK;
	int			m_nTotleTexNum;
	TextureSize m_TexSize[200];
private:
public:
public:
	SprConvert();
	~SprConvert();
	void ReadFile(char *szSprFile);
	int DoGroup(int nWidth, int nHeight, int nMinTexWidth, int nMinTexHeight, int &nTexNum);
	bool Output();
	void WriteFile(char *szSprFile);
};

#endif