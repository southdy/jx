/*****************************************************************************************
//	����--login������Ϣ����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-16
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndImage.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"
#include "../../Login/Login.h"

enum LOGIN_BG_INFO_MSG_INDEX
{
	CI_MI_CONNECTING = 1,		//1=������
	CI_MI_CONNECT_FAILED,		//2=����ʧ��
	CI_MI_CONNECT_SERV_BUSY,	//3=������æ
	CI_MI_CONNECT_TIMEOUT,		//4=���ӳ�ʱδ�����Ӧ
	CI_MI_ACCOUNT_PWD_ERROR,	//5=�˺�/�������
	CI_MI_ACCOUNT_LOCKED,		//6=�˺ű�����
	CI_MI_ERROR_ROLE_NAME,		//7=�½���ɫ�����ֲ��Ϸ�
	CI_MI_CREATING_ROLE,		//8=�����½���ɫ
	CI_MI_DELETING_ROLE,		//9=����ɾ����ɫ
	CI_MI_GETTING_ROLE_DATA,	//10=ȡ��ɫ������
	CI_MI_ENTERING_GAME,		//11=������Ϸ��
	CI_MI_SVRDOWN,				//12=��Ϸ����������������ά����

	CI_MI_TO_DEL_ROLE,			//13=Ҫɾ����ɫ
    CI_MI_INVALID_PROTOCOLVERSION,  //14=�汾�ŽϾɣ���Ҫ�������µĿͻ���

	CI_MI_ERROR_LOGIN_INPUT,	//15=��½����������˺�������������������
	CI_MI_ERROR_CONFIRM_INPUT,	//16=ɾ����ɫʱ�������������������������
	CI_MI_INVALID_LOGIN_INPUT1,	//17=�����в����԰����ո��Ʊ����ַ���
	CI_MI_INVALID_LOGIN_INPUT2,	//18=�������ȱ�����2��8���������ڣ���������Ӣ���ַ�������������������
	CI_MI_NOT_ENOUGH_ACCOUNT_POINT,	//19=�㿨����
	CI_MI_INVALID_PASSWORD,		//20=��������벻��ȷ��
	CI_MI_ACCOUNT_FREEZE = 30,	//30=�˺Ŷ���
};

enum LOGIN_BG_NEXT_STEP
{
	CI_NS_INIT_WND	= 1000,		//ת����ʼ����
	CI_NS_SEL_SERVER,			//ת��ѡ����������
	CI_NS_LOGIN_WND,			//ת����½����
	CI_NS_SEL_ROLE_WND,			//ת���½���ɫ����
	CI_NS_NEW_ROLE_WND,			//ת���½���ɫ����
	CI_NS_CONFIRM_DEL_ROLE,		//ת��ȷ��ɾ����ɫ����
	CI_NS_EXIT_PROGRAM,			//�˳�����
};

class KUiConnectInfo : protected KWndImage
{
public:
	//eDesireLoginStatusΪû��LOGIN_LOGIC_STATUS��LOGIN_BG_NEXT_STEP��ֵ֮һ
	static KUiConnectInfo*	OpenWindow(LOGIN_BG_INFO_MSG_INDEX eIndex,
									int eDesireLoginStatus,
									int nParam = 0);	//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void				CloseWindow(bool bDestroy);				//�رմ���

private:
	KUiConnectInfo();
	~KUiConnectInfo() {}
	void	Initialize();							//��ʼ��
	void	LoadScheme(const char* pScheme);		//������淽��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	//���ں���
	void	PaintWindow();
	void	SetInfoMsg(LOGIN_BG_INFO_MSG_INDEX eIndex);
	void	Breathe();								//�����
	bool	OnDelRole();
	void	OnCancelDelRole();
	void	OnClickConfirmBtn();
	void	GotoNextStep();
private:
	static 	KUiConnectInfo* m_pSelf;
private:
	KWndImage		m_DelRoleBgImg;
	KWndButton		m_ConfirmBtn;
	KWndButton		m_DelRoleBtn;
	KWndButton		m_CancelDelRoleBtn;
	KWndEdit32		m_InputPwdWnd;

	int				m_nDesireLoginStatus;

	char			m_szMessage[64];
	int				m_nMsgLen;
	int				m_nFont;
	int				m_nColor2CharacterStartIndex;
	int				m_nColor2CharacterEndIndex;
	int				m_nTextCentreX, m_nTextCentreY;

	unsigned int	m_uMsgColor;
	unsigned int	m_uMsgBorderColor;
	unsigned int	m_uMsgColor2;
	unsigned int	m_uMsgBorderColor2;
	unsigned int	m_uMsgColorChangeInterval;
	unsigned int	m_uLastMsgColorChanageTime;

	char			m_szLoginBg[32];
	int				m_nParam;
};
