/*****************************************************************************************
//	����Ӧ�ý���ϵͳ�Ĺ����ӿ�
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-17
------------------------------------------------------------------------------------------
	�������Ļ��࣬�˴�����弰�������࣬ÿ���඼ֻ�ܴ���һ��ʵ����
*****************************************************************************************/
#pragma once


void		UiPostQuitMsg();						//�����˳���Ϣ
bool		UiIsAlreadyQuit();						//�Ƿ��Ѿ�ѡ���˳�����
int			UiInit();								//����ϵͳ��ʼ��
int			UiStart();								//�_ʼ�����������
void		UiExit();								//����ϵͳ�˳�
void		UiPaint(int nGameLoop);					//���ƽ���
void		UiSetScreenSize(int nWidth, int nHeight);//������Ļ��Χ��С
int			UiHeartBeat();							//����
void		UiProcessInput(unsigned int uMsg, unsigned int uParam, int nParam);//��������
void		UiSetGraphicDevice(void* pDevice);		//���û�ͼ�豸

void		UiStartGame();							//������Ϸ����ʱ
void		UiOnGameServerConnected();				//��һ���µ�GameServer�����������
void		UiOnGameServerStartSyncEnd();			//��һ���µ�GameServer�������ӣ�����ͬ�����
void		UiResumeGame();							//��Ϸ�����ߺ󣩼���
void		UiEndGame();							//�뿪��Ϸ����ʱ
void		UiSetSwitchSceneStatus(int bSwitching);//�л����ص�ͼ״̬


#include "Elem/ComWindow.h"
#include "Elem/WndButton.h"

//����
class Player_Life : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Life)
	void UpdateData();
	void OnButtonClick();
	static bool m_bText;
};

//ħ��
class Player_Mana : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Mana)
	void UpdateData();
	void OnButtonClick();
	static bool m_bText;
};

//����
class Player_Stamina : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Stamina)
	void UpdateData();
	void OnButtonClick();
	static bool m_bText;
};

//����
class Player_Exp : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Exp)
	void UpdateData();
	void OnButtonClick();
	int  GetToolTipInfo(char* szTip, int nMax);
	static bool m_bText;
};

//�ȼ�
class Player_Level : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Level)
	void UpdateData();
};

//��������
class Player_WorldSort : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_WorldSort)
	void OnButtonClick();
	void UpdateData();
};

//״̬
class Player_Status : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Status)
	void OnButtonClick();
	const char*	GetShortKey();
};

//��Ʒ
class Player_Items : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Items)
	void OnButtonClick();
	const char*	GetShortKey();
};

//�书
class Player_Skills : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Skills)
	void OnButtonClick();
	const char*	GetShortKey();
};

//����
class Player_Team : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Team)
	void OnButtonClick();
	const char*	GetShortKey();
};

//����
class Player_Faction : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Faction)
	void OnButtonClick();
	const char*	GetShortKey();
};


//����
class Player_Sit : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Sit)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};

//�ܲ�
class Player_Run : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Run)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};

//����
class Player_Horse : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Horse)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};

//����
class Player_Exchange : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Exchange)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};


//PK����
class Player_PK : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_PK)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};
