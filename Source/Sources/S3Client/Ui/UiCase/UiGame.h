/*****************************************************************************************
//	�[�򴰿ڵĽӿ�
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-18
------------------------------------------------------------------------------------------
	������ȴ��9-18���ܣ����ǹ�ȥ��ζ�ű��ѣ�
*****************************************************************************************/
#pragma once

#include "../Elem/WndGameSpace.h"
#include "../Elem/WndWindow.h"

class KUiGameSpace : public KWndWindow
{
public:
	//���봦����Ϣ�Լ�һЩ�ض�������Ϣ����Ӧ
	int	WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	//������Ϸ����
	void PaintWindow();

	void OnMouseMoveCursor(int x,int y);

	KUiPlayerItem m_SelectPlayer;
};

extern KUiGameSpace	g_WndGameSpace;

enum
{
	ACTION_CHAT = 0,					//Ҫ��Name
	ACTION_MAKEFRIEND,					//Ҫ��Name
	ACTION_TRADE,						//pDest->nData == PLAYER_MENU_STATE_TRADEOPEN && pDest->nIndex != -1
	ACTION_JOINTEAM,					//pDest->nData == PLAYER_MENU_STATE_TEAMOPEN && pDest->nIndex != -1
	ACTION_INVITETEAM,					//pDest->uId != 0
	ACTION_FOLLOW,						//pDest->nIndex != -1
	ACTION_REVENGE,						//pDest->uId != 0
	ACTION_VIEWITEM,					//pDest->uId != 0
	ACTION_BLACKLIST,					//Ҫ��Name
	ACTION_TONG,                        //Ҫ��Name
};

//pDest��Name������, Ȼ���������ݾ��������д
void ProcessPeople(KUiPlayerItem* pDest, int nAction);
void ProcessEmote(char* szDest, char *szDestChannel = "", int nEmoteIndex = 0);

void PopUpContextPeopleMenu(const KUiPlayerItem& SelectPlayer, int x, int y);
void PopUpContextEmoteMenu(char* szDest, int x, int y);