/*****************************************************************************************
//	���洰����ϵ�ṹ--��������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-22
------------------------------------------------------------------------------------------
	��ť���ڡ�
	��ť���԰����������ԣ���ť������δ��������ʾ��ͬ��ͼ�Σ�ָ���ƶ�����ť�Ϸ�ʱ��ʾ������
	ÿ���һ�ΰ�ť�л�һ�ΰ�ť���Ƿ���״̬(CheckBox)��
*****************************************************************************************/
#pragma once
#include "WndImage.h"

#define	WNDBTN_ES_FILTER		0x00ff
#define	WNDBTN_ES_ANIMATION		0x0001		//���ָ��ͣ���ڴ����Ϸ�ʱ��ʾ����
#define	WNDBTN_ES_CHECKBOX		0x0002		//�л�״̬�İ�Ť
#define	WNDBTN_ES_SEND_HOLD_MSG	0x0004		//������걻��ס���ŵ���Ϣ
#define	WNDBTN_ES_NO_OVERSOUND	0x0008		//���ָ���Ƶ��Ϸ�ʱ������������Ч��

#define	WNDBTN_F_DOWN		0x0200			//��Ŧ������
#define	WNDBTN_F_CHECKED	WNDBTN_F_DOWN	//��Ŧ��ѡ��
#define	WNDBTN_F_OVER		0x0400			//���ͣ���ڴ����Ϸ�

class KWndButton : public KWndImage
{
protected:
	unsigned short	m_Flag;
private:
	short			m_nUpFrame;
	short			m_nDownFrame;
	short			m_nCheckOverFrame;
	short			m_nOverStartFrame;
	short			m_nDisableFrame;
	static	KWndButton* m_pPressedDownBtn;	//���������û�б��ͷŵİ�ť
	char			m_szTip[64];
	int				m_nTipLen;
public:
	KWndButton();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);//��ʼ��
	virtual const char*	GetShortKey() {return NULL;}	//ȡ��ݼ�,����Tip��ʾ
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	virtual void	PaintWindow();							//�������
	int				IsButtonChecked();						//��ť�Ƿ���ڰ���״̬
	int				IsButtonActive();
	void			CheckButton(int bChecked);				//���ð�ť�İ���״̬
	void			Enable(int bEnable);					//��ֹ��������ʹ���ڱ�����
	void			Clone(KWndButton* pCopy);
	static void		SetAllButtonTipTextColor(unsigned int uColor);	//�������а�ť��ʾ�������ֵ���ɫ
	static void		EnableAllButtonTip(int bEnable);					//��ֹ/�������а�ť����ʾ����
	virtual void	OnButtonClick(){}
	virtual int		GetToolTipInfo(char* szTip, int nMax);
	int		SetToolTipInfo(char* szTip, int nMax);

private:
	void			OnLBtnDown(bool bDoubleClick);	//��Ӧ�������ڴ˰���
	void			OnLBtnUp();						//��Ӧ�������ڴ˷ſ�
	void			OnLBtnDownMove();				//��Ӧ����������ʱ���ƶ�
private:
	static unsigned int	ms_uBtnTipTextColor;	//��ť��ʾ�������ֵ���ɫ
	static int			ms_nDisableBtnTip;		//�Ƿ��ֹ��ť����ʾ����
};

#include "WndText.h"
#include "WndMovingImage.h"
#include "WndImagePart.h"

class KWndImageTextButton : public KWndButton
{
protected:
	KWndMovingImage m_Image;
	KWndImagePart m_ImagePart;
	KWndText32 m_Text;
	BOOL bPart;
public:
	KWndImageTextButton();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);//��ʼ��
	void			Set2IntText(int nNumber1, int nNumber2, char Separator);
	void			SetIntText(int nNumber, char Separator);
	void			Set2IntValue(int nNumber1, int nNumber2);
	void			SetText(const char* pText, int nLen = -1);	//�����ı�����
	int				GetText(char* pBuffer, int nSize);		//��ȡ�ַ�������
};
