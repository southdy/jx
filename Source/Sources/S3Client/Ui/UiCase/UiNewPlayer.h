// -------------------------------------------------------------------------
//	�ļ���		��	UiNewPlayer.h
//	������		��	����
//	����ʱ��	��	2002-9-10 14:25:21
//	��������	��	��ɫ�½����棭1
//
// -------------------------------------------------------------------------
#ifndef __UINEWPLAYER_H__
#define __UINEWPLAYER_H__

#include "../Elem/WndImage.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndShadow.h"
#include "../../Login/Login.h"

class KUiNewPlayer : protected KWndShowAnimate
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiNewPlayer* OpenWindow(int nNativePlaceId);//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void			CloseWindow(bool bDestroy);		//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��

private:
	KUiNewPlayer();
	~KUiNewPlayer();
	void	Initialize();						//��ʼ��
	void	LoadScheme(const char* pScheme);	//������淽��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickButton(KWndWindow* pWnd);	//��Ӧ�����ť
	int		GetInputInfo();
	void	OnOk();								//��ɽ�ɫѡ�����
	void	OnCancel();							//���ء���ɫѡ����桱
	void	SelGender();
private:
	static KUiNewPlayer* m_pSelf;
	void	UpdateProperty();				//��������˵��
	void	Breathe();
private:
	KWndEdit32	m_Name;						// ����
	KWndButton	m_OK;						// ȷ��
	KWndButton  m_Cancel;					// ȡ��
	KWndButton	m_Male, m_Female;			// �Ա�ť
	
	KWndText256	m_PropertyShow;				// ����˵��
	KWndButton	m_Gold;						// ��
	KWndButton	m_Wood;						// ľ
	KWndButton	m_Water;					// ˮ
	KWndButton	m_Fire;						// ��
	KWndButton	m_Earth;					// ��
	
	KRoleChiefInfo		m_Info;
	char				m_szLoginBg[32];
	char				m_szPlayerImgPrefix[128];
	int					m_bJustClicked;
	struct PROPTYPEINFO
	{
		KWndWindow*			pBtn;
		int					nShowTextLen;
		char				propertyShow[256];		//����˵��
		char				szMaleImg[128];
		char				szFemaleImg[128];
		char				szMaleSound[128];
		char				szFemaleSound[128];
	}					m_propTypeInfoTable[series_num];
};

const char*		PROP_GetTitleString(OBJ_ATTRIBYTE_TYPE eType);

#endif // __UINEWPLAYER_H__