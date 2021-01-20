/*****************************************************************************************
//  �ı���������ȡ�ı����е�һ���ʵ�ͬһ�е��Ӵ����Լ��ı����ܵĺ���Ŀ��
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-29
------------------------------------------------------------------------------------------
//	Comment :  ������ı����а������Ʒ��ţ�����ԭʼ�ı�ǩ���Ʒ����Ǿ�������ġ��ı����в�
//	�ܳ�����Ч�ַ��������ģ����ܻ�����ڴ�Υ�����ʴ��󡣲ο�Engine�е�Text.h����ö���
//	�����еĿ��Ʒ����б����Լ�������Ч�ַ��Ĺ��ܡ�
//		�����ڹ��촦�����ʵ��ʱ����Ҫ������ַ�����������ݣ�Ҳ���Ե���SetContent������
//	Ҫ������ַ�����
//		��ε���GetSimplexText�������ȡ�ַ�����ͬ�еĵ�һ���ʵ��Ӵ������ַ������������
//	֮�������õ��ò���TailCtrl�е���ĩ������Ϊ�ı������������롣�´��ٵ����ֽ����´���
//	������ʼ����ȡ�Ӵ���
*****************************************************************************************/

struct iFont;
#include "TextProcessDef.h"

class KTextProcess
{
private:
	//�����ı����ƵĽṹ
#pragma pack(push, 1)
	struct KTP_CTRL
	{
		unsigned char cCtrl;	//�˳�ԱȡֵΪText.h��÷��KTEXT_CTRL_CODE�е�һ��ֵ
		union{
			struct {
				unsigned char cParam0;	//�˼�����������Ա��ȡֵ�뺬������cCtrl��ȡֵ����
				unsigned char cParam1;
				unsigned char cParam2;
			};
			unsigned short wParam;
		};
	};
#pragma pack(pop)
public:
	//��ָ����λ�û����ַ�����ָ��������
	int		DrawTextLine(iFont* pFont, int nFontSize, KOutputTextParam* pParam);

	//�ж�ĳһ�����꣬��ָ����λ��������ַ����У���ռ��ƫ����
	//ע�⣺��������ַ���ռ�ľ����򷵻ص�ƫ��Ϊ��ָ�����������һ���ַ���ƫ��
	int		TransXYPosToCharOffset(int nX, int nY, iFont* pFont, int nFontSize, KOutputTextParam* pParam);

	//����ָ���п�ʼ��
	int		SeekToSpecialLine(int nLineIndex, KTP_CTRL& Ctrl1, KTP_CTRL& Ctrl2,
							  int& nSkipedHalfLines, int nFontSize, int bPicPackInSingleLine);
	//����Ҫ������ı�������
	void	SetContent(const char* pBuffer, int nCount, int nLineLen);

	void SetEncodePlugin(fnTextProcessEncodePlugin fnPlugin);

	void SetDrawPlugin(fnTextProcessEncodePlugin fnPlugin);

	//����һ�еĸ߶� [wxb 2003-6-20]
	void GetCurLineHeight(int& nSpanLines, int& nHeight, int nFontSize, int bPicPackInSingleLine);
	//����ͻָ��ڲ����� [wxb 2003-6-20]
	void Backup()
	{
		//_ASSERT(!m_bBackuped);
		m_nCountB			= m_nCount;
		m_nCurrentLineLenB	= m_nCurrentLineLen;
		m_fCurrentLineLenB	= m_fCurrentLineLen;
		m_nMaxLineLenB		= m_nMaxLineLen;
		m_nReadPosB			= m_nReadPos;

		m_bBackuped = 1;	//true
	}
	void Restore()
	{
		//_ASSERT(m_bBackuped);
		m_nCount			= m_nCountB;
		m_nCurrentLineLen	= m_nCurrentLineLenB;
		m_fCurrentLineLen	= m_fCurrentLineLenB;
		m_nMaxLineLen		= m_nMaxLineLenB;
		m_nReadPos			= m_nReadPosB;

		m_bBackuped = 0;	//false
	}

	//���캯��
	KTextProcess();
	KTextProcess(const char* pBuffer, int nCount, int nLineLen);

private:
	//��ȡ�ַ����ﵥһ����ͬ�е�һ���Ӵ�
	int		GetSimplexText(int &nSimplexStartPos, KTP_CTRL& HeadCtrl, KTP_CTRL& TailCtrl, int nFontSize);

private:
	const char*	m_pBuffer;	//�ı���������ָ��
	int			m_nCount;	//�ı����������ݵĳ���(��BYTEΪ��λ)
	int			m_nCurrentLineLen;
	float		m_fCurrentLineLen;
	int			m_nMaxLineLen;	//ÿ�п������ɵģ���ǣ��ַ�����Ŀ
	int			m_nReadPos;	//��ǰ�ȶ�ȡ��λ��

	//for backup
	int			m_bBackuped;

	int			m_nCountB;
	int			m_nCurrentLineLenB;
	float		m_fCurrentLineLenB;
	int			m_nMaxLineLenB;
	int			m_nReadPosB;
};
