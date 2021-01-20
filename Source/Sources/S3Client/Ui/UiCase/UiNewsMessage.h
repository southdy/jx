/*****************************************************************************************
//	����--������Ϣ����
//	Copyright : Kingsoft 2003
//	Author	:   Fyt(Fan Zhanpeng)
//	CreateTime:	2003-08-01
------------------------------------------------------------------------------------------
*****************************************************************************************/
#ifndef _UINEWSMESSAGE_H
#define _UINEWSMESSAGE_H    1

#include "KEngine.h"
#include "KIniFile.h"
#include "../Elem/WndText.h"
#include "../Elem/WndShadow.h"
#include "../../../Core/Src/GameDataDef.h"

struct KNewsMessageNode : public KNewsMessage
{
	unsigned int		uTime;			//һ����Ϣ-�����̶�Ϊ0����������Ϣ-��ʾ�������ĺ�����
										//��ʱ��Ϣ����ʱʱ���ޣ�(time()��ʱ����)
	union
	{
		unsigned int	uShowTimes;		// ����Ϣ�Ѿ���ʾ�Ĵ���������һ����Ϣ��
		unsigned int	uStartTime;		// ����ʱ��ʼ��ʱ�䣨���ڵ���ʱ��Ϣ��
		unsigned int	uLastShowTime;	// ���һ����ʾ��ʱ�䣨���ڶ�ʱ��Ϣ��
	};
	KNewsMessageNode*	pNext;
};

class KUiNewsMessage : protected KWndShadow
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiNewsMessage*	OpenWindow();						// �򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiNewsMessage*  GetIfVisible();                     //�������������ʾ���򷵻�ʵ��ָ��
	static void				CloseWindow(BOOL bDestory = FALSE);	// �رմ���
	static void				LoadScheme(const char* pszScheme);	// ������淽��
	static void				MessageArrival(KNewsMessage* pMsg, SYSTEMTIME* pTime);
	static void				EnableIdleMsg(BOOL bEnable);

private:
    static KUiNewsMessage *m_pSelf;
	static BOOL				ms_bEnableIdleMsg;
private:
	KUiNewsMessage();
	~KUiNewsMessage() {}
	void	Initialize();
	void	LoadScheme(KIniFile* pIni);			// ������淽��
	virtual void	Breathe();					// ��Ҫ���������Ϣ����-____-|||b��
	virtual int		PtInWindow(int x, int y);	// ��͸������
	virtual void	PaintWindow();

	bool	AddMessage(KNewsMessage* pMsg, unsigned int uTime, unsigned int uTimeParam = 0);
												// ����Ϣ����-__-||b����ǰ������һ����Ϣ-__-||b
	KNewsMessageNode* 	SeparateMsg();			// ��m_pHandling��ָ����Ϣ����������������������
	void	AddToTail(KNewsMessageNode* pNode);	// ��pNode��ָ����Ϣ�ӵ�����ĩ�ˣ�
	bool	PickAMessage();						// �ڶ����У�Ѱ���Ƿ��з�����ʾ��������Ϣ�����Ҹ���ɾ����ʱ��Ϣ
	void	Reset();							// ������Ϣ����ʾ״̬
	bool	ScrollMsg();						// ������ǰ��Ϣ
	void	PickFromIni();						// ��Ini�ļ��������ȡһ����Ϣ�������
	void	Clear();							// ������е���Ϣ
	void	ConvertMsg();						// ת����Ϣ��������ʾ����
	bool	MakeCountingMsg();					// ���ɵ���ʱ��Ϣ����ʾ����

private:

	KNewsMessageNode*	m_pHead;			// ��Ϣ�����е�ͷһ����Ϣ
	KNewsMessageNode*	m_pHandling;		// ������ʾ/�����������Ϣ

	KIniFile		m_IniFile;				// ��¼Ini�ļ���Ҫ�������ʾ��һЩ��Ϣ

	int				m_nIndentH;				// ǰ׺����ʾ��Ϣ���ݷָ�����
	int				m_nIndentV;				// ��������ڴ��ڶ�����������������λ�����ص㣩
	int				m_nFontSize;			// �����С
	unsigned int	m_uTextColor;			// ǰ���ַ�����ɫ
	unsigned int	m_uTextBorderColor;		// ���ֱ�Ե��ɫ
	int				m_nVisionWidth;			// ��ʾ��Ϣ�Ŀռ�����ؿ��
	int				m_nCharasVisibleLimitNum;// ���ɵ��ַ�������
	int				m_nFontHalfWidth[2];	// �ַ�������һ����

	unsigned int	m_uMaxIdleTime;			// �����Ϣ����ʱ��
	unsigned int	m_uShowInterval;		// ͬһ����Ϣ������ʾ֮��ļ��
	unsigned int	m_uScrollInterval;		// �ַ�������ʱ����

	unsigned int	m_uLastShowTime;		// ��һ����ʾ��Ϣ��ʱ��
	KNewsMessage	m_CurrentMsg;			// ��ǰ��ʾ����Ϣ����
	int				m_nLineLen;				// ��ǰ��ʾ��Ϣ������ʾ�ַ�������

	unsigned int	m_uLastScrollTime;		// �ϴι�����ʱ��
	bool	m_bJustIncoming;				// ��Ϣ���ڽ���,(ͷ)��δ��ʼ�뿪��ʾ����
	int		m_nCharIndex;					// ��Ϣ�ַ��Ĺ����±�
	int		m_nTextPosX;					// ��Ϣ��ʾ��������Ͻ�X����(���)
	int		m_nHalfIndex;					// �ַ���������
	int		m_nInsertPlace;					// ֻ�Ե�������Ϣ��Ч
};

#endif
