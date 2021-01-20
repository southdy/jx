/*****************************************************************************************
//	��������
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-5-15
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

enum UI_SOUND_INDEX
{
	UI_SI_WND_OPENCLOSE = 0,	//������رս���ʱˢ��һ��
	UI_SI_POPUP_OUTGAME_WND,	//������Ϸ��Ľ��������
	UI_SI_OVER_OUTGAME_WND_BTN,	//����ƶ�����Ϸ��Ľ���İ�ť��
	UI_SI_PICKPUT_ITEM,			//�������õ��ߵ���Ʒ��ʱ������
	UI_SI_PLAYER_ATTRIB,		//ѡ�˼�������������
	UI_SI_FRIEND_ONLINE,		//����������ʾ��

	UI_SOUND_COUNT
};

bool UiSoundLoadSetting();		//�����趨
void UiSoundPlay(UI_SOUND_INDEX eIndex);	//����ָ��������
void UiSoundPlay(char* SoundFileName);	//����ָ��������
