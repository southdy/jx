/*****************************************************************************************
//	����--login���̱�������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-16
------------------------------------------------------------------------------------------
*****************************************************************************************/

#pragma once
#include "../Elem/WndImage.h"
#include "../Elem/WndText.h"

class KUiLoginBackGround : protected KWndImage
{
public:
	static KUiLoginBackGround*	OpenWindow(const char* pszConfig);	//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void					CloseWindow(bool bDestroy);			//�رմ���
	static void					SetConfig(const char* pszConfig);	//����ͼ��
private:
	KUiLoginBackGround() {}
	~KUiLoginBackGround() {}
	void	Initialize();					//��ʼ��
	void	LoadScheme(KIniFile* pIni);		//������淽��
	void	Breathe();						//�����
private:
	static 	KUiLoginBackGround* m_pSelf;
#define	MAX_NUM_BUTTERFLY	3
	KWndImage		m_Butterflys[MAX_NUM_BUTTERFLY];
	unsigned int	m_uAdviceIntervalMin[MAX_NUM_BUTTERFLY];
	unsigned int	m_uAdviceIntervalMax[MAX_NUM_BUTTERFLY];
	unsigned int	m_uInterval[MAX_NUM_BUTTERFLY];
	unsigned int	m_uLastSleepTime[MAX_NUM_BUTTERFLY];
	char			m_szConfig[32];
	KWndText80		m_VersionWnd;
};
