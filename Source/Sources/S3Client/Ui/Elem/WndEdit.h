/*****************************************************************************************
//	���洰����ϵ�ṹ--���봰��
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-22
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "WndText.h"

#define	WNDEDIT_ES_FILTER				0x00ff
#define	WNDEDIT_ES_ENGLISH_AVAILABLE	0x0001		//�������������ַ������š�����
#define	WNDEDIT_ES_IME_AVAILABLE		0x0002		//��������ime�ַ��������ַ�����
#define	WNDEDIT_ES_CHAR_TYPE_FILTER		(WNDEDIT_ES_ENGLISH_AVAILABLE | WNDEDIT_ES_IME_AVAILABLE)
#define	WNDEDIT_ES_MASK_CHARACTER		0x0010		//��ʾʱ����������滻Ҫ��ʾ���ַ�
#define	WNDEDIT_ES_MULTI_LINE			0x0020		//���б༭
#define	WNDEDIT_F_HAVE_FOCUS			0x0100		//Ϊ��ǰӵ�����뽹��Ĵ���

class KWndEdit : public KWndWindow
{
public:
	KWndEdit();
	~KWndEdit();
	virtual int	Init(KIniFile* pIniFile, const char* pSection);//��ʼ��
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	virtual void	PaintWindow();							//���ƴ���
	void			SetColor(unsigned int uColor);
	void			SetText(const char* pText);				//�����ı�����
	int				GetText(char* pBuffer, int nSize, bool bExcludeSpace);//��ȡ�ַ�������
	void			SetIntText(int nNumber);				//�����ı���������Ϊ��������������ַ���
	int				GetIntNumber();
	void			Enable(int bEnable);					//��ֹ��������ʹ���ڱ�����
	void			Clone(KWndEdit* pCopy);
	void			ClearText(bool bSafe = false);
	int				InsertString(const char* pString, int nLen);
	void			GetCaretPosition(int* nx, int* ny);
	unsigned int	GetFocusBkColor()	{return m_FocusBkColor;}
	void	SetFocusBkColor(unsigned int nC)	{m_FocusBkColor = nC;}

protected:
	void			SetTextPtr(char* pText, int nMaxLen);	//�����ı�������ָ��
	int				InsertChar(int nChar);					//
	int				InsertChar(char cLow, char cHigh);		//
	int				OnKeyDown(int nKeyCode, int nModifiers);//��Ӧ���̰�������

private:
	int				CharFromPos(int x, int y);				//retrieves information about the character closest to a specified point in the client area of an edit control
	void			UpdateData();
	void			FmtForShow();
	int				Paste();
private:
	unsigned int	m_Flag;
	int			m_nFontSize;				//�����С
	char*		m_pText;					//�ı�������ָ��
	int			m_nBufferSize;				//�ı�������ָ��ĳ���
	int			m_nLimitText;				//�ַ������ȣ�BYTE�����ƣ��������ַ����������š�
	int			m_nTextLen;					//�ַ���ʵ�ʳ���

	int			m_nSkipAhead;				//��ʾʱ����ǰ�˳������ַ���ǰ�˱������ַ�������
	int			m_nSkipBehind;				//��ʾʱ���Ժ�˳������ַ�����˱������ַ�������

	int			m_nCaretLineIndex;			//��������ڵ���λ��
	int			m_nCaretPos;				//�������λ��
	int			m_nCaretTime;				//���Ʋ������ʾ��ʱ��

    int         m_nCaretX;
    int         m_nCaretY;

    //HWND        m_hEdit;


	unsigned int m_TextColor;				//������ɫ
	unsigned int m_TextBorderColor;			//�����Ե��ɫ
	unsigned int m_FocusBkColor;			//�н���ʱ�ı�����ɫ
};

//=====================================
//	�ı�����������Ϊ32���ı����봰����
//=====================================
class KWndEdit32 : public KWndEdit
{
private:
	char			m_Text[32];
public:
	KWndEdit32();
};

//=====================================
//	�ı�����������Ϊ32���ı����봰����
//=====================================
class KWndEdit512 : public KWndEdit
{
private:
	char			m_Text[512];
public:
	KWndEdit512();
};
