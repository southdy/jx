/*****************************************************************************************
//	����--ϵͳ��Ϣ����
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-15
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndImage.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../../../core/src/gamedatadef.h"

#define	MAX_SYS_MSG_TYPE			6
#define	POPUPING_MSG_HEAP_INDEX		MAX_SYS_MSG_TYPE
#define	RECYCLE_MSG_HEAP_INDEX		POPUPING_MSG_HEAP_INDEX + 1

class KUiSysMsgCentre : protected KWndWindow
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiSysMsgCentre*	OpenWindow();		//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void				CloseWindow();		//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void				LoadScheme(const char* pScheme);//������淽��	

	static bool				AMessageArrival(KSystemMessage* pMsg, void* pParam);

private:
	KUiSysMsgCentre();
	~KUiSysMsgCentre();
	void	Initialize();
	void	Clear();
	void	LoadScheme(KIniFile* pIni);			//������淽��
	KSystemMessage*	GetAMsgSpace(int nParamSize);
	bool	AddAMsgToHeap(KSystemMessage*pMsg, int nHeapIndex, bool bSort);
	void	DeleteMsgInHeap(int nHeapIndex, int nMsgIndex, bool bImmedDel, bool bTobeConfirm);
	void	ConfirmMsg(KSystemMessage* pMsg, bool bImmedDel);
	void	MovePopupedMsgToHeap();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���

	void	OnConfirmOperFinished(unsigned int uParam, int nSelAction);	//��Ӧ��������������Ѿ�����ѡ��
	void	SetPopupMsgDest();
	void	PaintWindow();							//���ƴ���
	int		PtInWindow(int x, int y);				//�ж�һ�����Ƿ��ڴ��ڷ�Χ��,������Ǿ�������
	void	Breathe();
	bool	FilterSameMsg(KSystemMessage* pMsg, void* pParam);
private:
	static KUiSysMsgCentre* m_pSelf;
private:
	struct	SYS_MSG_HEAP
	{
		KSystemMessage**	pMsgList;						//��Ϣָ����б�
		int					nListSpace;						//��Ϣָ���б�Ĵ�С���������ٸ�ָ��ռ䣩
		int					nNumValid;						//�б�����Ч����Ϣ����Ŀ
	}						m_MsgHeap[MAX_SYS_MSG_TYPE + 2];

	KUiMsgParam				m_SysMsgParam;					//����������Ϣ���ڵĲ���

	int						m_nPopupMsgDestX;				//������Ϣ���յ�λ�ú�������
	int						m_nPopupMsgX;					//������Ϣ�ĵ�ǰλ�ú�������
	int						m_nPopupMsgY;					//������Ϣ�ĵ�ǰ��λ����������
	int						m_nPopupMsgDestIndex;			//������Ϣ���յ�λ�����ĸ�ͼ�갴ť��
	unsigned int			m_uLastMovementTime;
	unsigned int			m_uMoveInterval;
	unsigned int			m_uDisappearInterval;

	KWndText32				m_MsgTextWnd;
	int						m_bShowMsgText;

	KWndButton				m_MsgIconBtn[MAX_SYS_MSG_TYPE];

	KUiImageRef				m_MsgIcon[MAX_SYS_MSG_TYPE];	//��Ϣͼ���ͼ��
	short					m_nMsgIconFrame[MAX_SYS_MSG_TYPE];//��Ϣͼ���ͼ��֡����

	KSystemMessage*			m_pHandlingMsg;
};