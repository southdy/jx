// -------------------------------------------------------------------------
//	�ļ���		��	UiNewPlayer2.h
//	������		��	����
//	����ʱ��	��	2002-9-10 14:25:21
//	��������	��	��ɫ�½����棭2
// -------------------------------------------------------------------------
#ifndef __UINEWPLAYER2_H__
#define __UINEWPLAYER2_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndShowAnimate.h"
#include "../../Login/Character/SelSavedCharacter.h"

class KUiNewPlayer2 : protected KWndShowAnimate
{
public:
	static KUiNewPlayer2* OpenWindow(const char* pName, int bMale);//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void			CloseWindow(bool bDestroy);		//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��

private:
	KUiNewPlayer2();
	~KUiNewPlayer2();
	void	LoadScheme(const char* pScheme);//������淽��
	void	Initialize();					//��ʼ��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickButton(KWndWindow* pWnd);//��Ӧ�����ť
	void	UpdateProperty();				//��������˵��
	void	OnOk();							//���
	void	OnCancel();						//ȡ��
	void	Breathe();
private:
	static KUiNewPlayer2* m_pSelf;
	
private:
	KWndText256	m_PropertyShow;				// ����˵��
	KWndButton	m_Gold;						// ��
	KWndButton	m_Wood;						// ľ
	KWndButton	m_Water;					// ˮ
	KWndButton	m_Fire;						// ��
	KWndButton	m_Earth;					// ��
	KWndButton	m_OK;						// ȷ��
	KWndButton  m_Cancel;					// ȡ��
	
	KNewCharacterInfo	m_Info;
	bool		m_bCreating;
};

#endif // __UINEWPLAYER2_H__