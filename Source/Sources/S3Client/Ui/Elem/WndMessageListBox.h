// -------------------------------------------------------------------------
//	�ļ���		��	ShowChatText.h
//	������		��	����
//	����ʱ��	��	2002-9-16 16:22:52
//	��������	��	��ʾ��������
// -------------------------------------------------------------------------
#pragma once

#include "WndWindow.h"
#include "WndScrollbar.h"

struct KOneMsgInfo
{
	int			 nLines;				//������Ϣռ�˶�����
	int			 nCharWidth;			//������Ϣռ���ַ���
	unsigned int uTextBKColor;			//������Ϣ����ʾʱ�����ֳĵ׵���ɫ,0Ϊ��
	int			 nMaxSize;				//Msg�������ɵ������Ϣ����
	int			 nLen;					//��Ϣ����
	char		 Msg[1];				//��Ϣ������
};

class KWndScrollBar;

class KWndMessageListBox : public KWndWindow
{
public:
	KWndMessageListBox();
	virtual ~KWndMessageListBox();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);//��ʼ��

	void	SetFirstShowLine(int nLine);	//���õ�һ������ʾ��������ȫ�����ֵĵڼ���
	int		GetFirstShowLine();				//��ȡ��һ������ʾ��������ȫ�����ֵĵڼ���
	void	SetScrollbar(KWndScrollBar* pScroll);

	int		SetCapability(int nNumMessage);
	int		GetCapability(){ return m_nCapability;}
	int		GetCurSel() { return m_nSelMsgIndex; }
	int		SetCurSel(int nIndex);

	int		AddOneMessage(const char* pText, int nLen, unsigned int uTextBKColor = 0);
	int		GetOneMessage(int nIndex, char* pBuffer, int nLen, bool bExcludeCtrl);
	void	RemoveAMessage(int nIndex);
	int		GetMsgCount() { return m_nNumMessage; }
	int		HitTextAtPoint(int x, int y);

	void	SetSize(int nWidth, int nHeight);//���ô��ڴ�С
	void	Clear();
	void	Clone(KWndMessageListBox* pCopy);
	int		GetMaxShowLine() {return m_nNumMaxShowLine;}
	int		GetItemLineCount(int nIndex);

	//�Ӵ��ڷ��������
	unsigned int	SplitData();
	//��������������
	unsigned int	BindData(unsigned int	hData);
	//�ͷŴ������ݾ��
	static void		FreeData(unsigned int	hData);

	virtual int		PtInWindow(int x, int y);

	void ClearHideLine();
	void HideNextLine();
	void HideAllLine();
	
	int	GetMinHeight();
	
	void SetFontSize(int nFontSize);
	int GetFontSize()	{return m_nFontSize;}

private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	UpdateData();					//����������ɾ���ߴ��ڳߴ�仯������Щ���������Լ���������������
	virtual void	PaintWindow();			//���ƴ���
	void	OnMouseMove(int x, int y);
	void	OnLButtonDown(int x, int y);
	void	OnLButtonDClick(int x, int y);
	int		GetMsgAtPoint(int x, int y);

private:
	struct KMessageListData
	{
		KOneMsgInfo**	pMessages;			//��Ϣ����
		int				nNumMessage;		//��Ϣ����Ŀ
		int				nCapability;		//��Ϣ�����������Ŀ
		int				nStartShowMsg;		//������ʾ����Ϣ
		int				nStartMsgSkipLine;	//������ʾ����Ϣ�Ϸ����Ե�����Ŀ
		int				nSelMsgIndex;		//��ǰѡ�����Ϣ
	};
	
private:
	KWndScrollBar*	m_pScrollbar;			//������
	KOneMsgInfo**	m_pMessages;			//��Ϣ����

	int				m_nNumMessage;			//��Ϣ����Ŀ
	int				m_nCapability;			//��Ϣ�����������Ŀ

	int				m_nNumMaxShowLine;		//������ʾ������Ŀ
	int				m_nNumBytesPerLine;		//ÿһ���ַ�����Ŀ
	int				m_nNumVisibleTextLine;	//���Կ����������ֵ��е���Ŀ
	
	int				m_nHideNumLine;		//��ǿ�����ص�����,׷��/ɾ��/ѡ��/������/�ĳߴ�ʱ����Ч

	int				m_nFontSize;			//�����С
	int				m_nStartShowMsg;		//������ʾ����Ϣ
	int				m_nStartMsgSkipLine;	//������ʾ����Ϣ�Ϸ����Ե�����Ŀ

	int				m_nSelMsgIndex;			//��ǰѡ�����Ϣ
	int				m_nHLMsgIndex;			//�����ָ��Ϣ������
	unsigned int	m_MsgColor;				//Ĭ�ϵ�������ɫ
	unsigned int	m_MsgBorderColor;		//Ĭ�ϵ����ֱ�Ե��ɫ
	unsigned int	m_SelMsgColor;			//��ѡ�����ֵ���ɫ
	unsigned int	m_SelMsgBorderColor;	//��ѡ�����ֵı�Ե��ɫ
		
	unsigned int	m_SelMsgBgColor;		//��ѡ����Ϣ�ı�����ɫ
	unsigned int	m_HLMsgColor;			//�����ָ��Ϣ����ɫ
	unsigned int	m_HLMsgBorderColor;		//�����ָ��Ϣ�ı�Ե��ɫ
	unsigned int	m_uTextLineShadowColor;	//�����ֵ���ӵ�еı�����ɫ�����Ϊ0����ʾ�޴˱���ɫ

	BOOL m_bHitText;

	unsigned int	GetOffsetTextHeight();
};

class KScrollMessageListBox : public KWndWindow
{
public:
	KScrollMessageListBox();

	virtual int		Init(KIniFile* pIniFile, const char* pSection);//��ʼ��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	
	KWndMessageListBox* GetMessageListBox()
	{
		return &m_MsgList;
	}
	KWndScrollBar* GetScrollBar()
	{
		return &m_Scroll;
	}
	int		HeightToLineHeight(int nHeight);
	int		WidthToCharWidth(int nWidth);
	int		GetMinHeight();
protected:
	KWndMessageListBox	m_MsgList;
	KWndScrollBar		m_Scroll;
	int m_nLineHeight;
	int m_nMinLineCount;
	int	m_nMaxLineCount;
	int m_nCurrentLineCount;

	void	SetMsgLineCount(int nCount);
	void	InitMinMaxLineCount(int nMin, int nMax);//��ʼ��
};

bool	MsgListBox_LoadContent(KWndMessageListBox* pBox, KIniFile* pFile, const char* pszSection);