/*****************************************************************************************
//	����--С��ͼ
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-21
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndImage.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndShadow.h"
#include "../Elem/PopupMenu.h"
#include "../../Core/Src/GameDataDef.h"

//С��ͼ����ʾ����ģʽ
enum MINIMAP_MODE
{
	MINIMAP_M_BRIEF_PIC = 0,			//����ͼ
	MINIMAP_M_BRIEF_PIC_BROWSE,			//����ͼ���/�Ŵ�ģʽ
	MINIMAP_M_WORLD_MAP,				//�����ͼ

	//MINIMAP_M_OBSTACLE,				//�ϰ�ͼģʽ
	MINIMAP_M_NONE,					//û����ʾ��ͼ

	MINIMAP_M_COUNT,
};

struct KUiSceneTimeInfo;

class KUiMiniMap : public KWndImage
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiMiniMap*	OpenWindow();					//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiMiniMap*	GetIfVisible();					//�������������ʾ���򷵻�ʵ��ָ��
	static void			CloseWindow();					//�رմ���

	static void			SetMode(MINIMAP_MODE eMode);	//������ʾ����ģʽ
	static void			LoadScheme(const char* pScheme);//������淽��
	static void			UpdateSceneTimeInfo(KUiSceneTimeInfo* pInfo);

	static KUiMiniMap*	GetSelf() {return ms_pSelf;}

	void	Show();										//��ʾ����
	void	Hide();										//���ش���

private:
	KUiMiniMap();
	~KUiMiniMap() {}
	void	Initialize();
	void	PaintWindow();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	LoadScheme(KIniFile* pIni);		//������淽��
	void	SetStatus(bool bSmall);			//�л�״̬
	void    MapScroll(int nbScrollScene);	//��ͼ��
	void	StopScrollMap();				//ֹͣ�϶���ͼ
	void	MapMoveBack();					//��ͼ��ԭԭʼƫ��
	virtual void	Breathe();

private:
	static KUiMiniMap*	ms_pSelf;
private:
	unsigned int	m_uShowElem;
	POINT			m_MapPos;
	SIZE			m_MapSize;
	KWndButton		m_SwitchBtn;
	KWndText32		m_SceneName;
	KWndText32		m_ScenePos;
	KWndShadow		m_Shadow;
	KWndButton		m_WorldMapBtn;

	POINT           m_OldPos;
	unsigned int	m_uLastScrollTime;             // �����һ�ι�����ʱ��(��)
	int             m_nOffsetX;                    // ÿ��Loop����⵽���ϴ����λ�õ�ƫ��X
	int             m_nOffsetY;                    // ÿ��Loop����⵽���ϴ����λ�õ�ƫ��Y
};

void	MapToggleStatus();
void	MapSetMode(MINIMAP_MODE eMode);	//������ʾ����ģʽ