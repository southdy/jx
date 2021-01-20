/*****************************************************************************************
//	����--ѡ������ش���
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-6-7
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndList.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndShowAnimate.h"
#include "../../Login/LoginDef.h"
#include "../../Login/Login.h"

struct KNativePlace
{
	char			szName[32];
	char			szImage[128];
	unsigned short	nId;
	unsigned short	nDescLen;
	char			sDesc[256];
};

class KUiSelNativePlace : protected KWndShowAnimate
{
public:
	static KUiSelNativePlace* OpenWindow(int nPlaceId = -1);//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void				 CloseWindow(bool bDestroy);	//�رմ���
private:
	KUiSelNativePlace();
	~KUiSelNativePlace();
	void	Initialize();						//��ʼ��
	void	LoadScheme(const char* pScheme);	//������淽��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	//���ں���
	void	OnOk();
	void	OnCancel();
private:
	void	LoadList(int nPlaceId);
	void	UpdateData();
	static KUiSelNativePlace* m_pSelf;
private:
	KNativePlace*	m_pPlaceList;
	KWndList		m_List;
	KWndImage		m_PlaceImg;
	KWndButton		m_Ok;
	KWndButton		m_Cancel;
	KWndText256		m_PlaceDesc;
	char			m_szLoginBg[32];
	int				m_nLastSelPlace;
};
