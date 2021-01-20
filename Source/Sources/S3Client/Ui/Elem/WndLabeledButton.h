/*****************************************************************************************
//	���洰����ϵ�ṹ--�����ְ�������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-10-4
------------------------------------------------------------------------------------------
	�����ְ�ť���ڡ�
*****************************************************************************************/
#pragma once
#include "WndButton.h"
#include "WndText.h"

//===========================================
//	�򵥵Ĵ����ְ�ť���ڡ�
//	���ֵ��У����д��������в��������Ʒ���
//===========================================
class KWndLabeledButton : public KWndButton
{
public:
	KWndLabeledButton();
	void			SetLabel(const char* pLabel);		//���ð�ť��������
	int				GetLabel(char* pLabel, int nSize);
	void			SetLabelColor(unsigned int Color);	//���ð�ť����������ɫ
	virtual int		Init(KIniFile* pIniFile, const char* pSection);	//��ʼ��
	virtual void	PaintWindow();									//�������
	void			Clone(KWndLabeledButton* pCopy);
private:
	char	m_Label[32];		//��������
	int		m_nLabelLen;		//�������ִ��ĳ��ȣ���������������
	int		m_nFontSize;		//�������ֵĴ�С
	int		m_nLabelYOffset;	//������������ƫ�Ƶ���
	unsigned int m_FontColor;	//�������ֵ���ɫ
	unsigned int m_FontBorderColor;	//�������ֵı�Ե��ɫ
	unsigned int m_OverColor;	//����ƶ�������ʱ���������ɫ
	unsigned int m_OverBorderColor;	//����ƶ�������ʱ������ֱ�Ե��ɫ
};

//=====================================================
//	���ӵĴ����ְ�ť���ڡ�
//	���ֿ���Ϊ���У�Ҳ����Ϊ���У�����ʱ�����ָ��
//	ˮƽ�Լ���ֱ�Ķ��뷽ʽ�������п��԰������Ʒ���
//=====================================================
class KWndRichLabeledButton : public KWndButton
{
public:
	void			SetLabel(const char* pLabel, int nLen);	//���ð�ť��������
	void			SetLabelColor(unsigned int Color);		//���ð�ť����������ɫ
	virtual int		Init(KIniFile* pIniFile, const char* pSection);	//��ʼ��
	virtual void	PaintWindow();									//�������
private:
	KWndText256		m_Label;	//�������ֿؼ�
};