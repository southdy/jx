/*****************************************************************************************
//	���洰����ϵ�ṹ--�����ְ�������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-10-4
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "WndLabeledButton.h"
#include "Wnds.h"
#include "../../../Engine/Src/Text.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

//===========================================
//	�򵥵Ĵ����ְ�ť���ڡ�
//	���ֵ��У����д��������в��������Ʒ���
//===========================================

//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
KWndLabeledButton::KWndLabeledButton()
{
	m_Label[0]  = 0;
	m_Label[31] = 0;
	m_nLabelLen = 0;
	m_nFontSize = 16;
	m_nLabelYOffset = 0;
}

void KWndLabeledButton::Clone(KWndLabeledButton* pCopy)
{
	if (pCopy)
	{
		KWndButton::Clone(pCopy);
		pCopy->m_nFontSize = m_nFontSize;
		pCopy->m_FontColor = m_FontColor;
		pCopy->m_OverColor = m_OverColor;
		pCopy->m_nLabelYOffset = m_nLabelYOffset;
		pCopy->m_FontBorderColor = m_FontBorderColor;
		pCopy->m_OverBorderColor = m_OverBorderColor;
		strcpy(pCopy->m_Label, m_Label);
	}
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ������
//--------------------------------------------------------------------------
int KWndLabeledButton::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndButton::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "Font", 16, &m_nFontSize);
		pIniFile->GetInteger(pSection, "LabelYOffset", 0, &m_nLabelYOffset);
		if (m_nFontSize < 12)
			m_nFontSize = 16;
		//===��ȡ����====
		char	Buff[32];
		pIniFile->GetString(pSection, "Color", "", Buff, sizeof(Buff));
		m_FontColor = GetColor(Buff);
		pIniFile->GetString(pSection, "BorderColor", "", Buff, sizeof(Buff));
		m_FontBorderColor = GetColor(Buff);
		pIniFile->GetString(pSection, "OverColor", "", Buff, sizeof(Buff));
		m_OverColor = GetColor(Buff);
		pIniFile->GetString(pSection, "OverBorderColor", "", Buff, sizeof(Buff));
		m_OverBorderColor = GetColor(Buff);
		
		pIniFile->GetString(pSection, "Label", "", Buff, sizeof(Buff));
		SetLabel(Buff);
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
//	���ܣ����ð�ť��������
//--------------------------------------------------------------------------
void KWndLabeledButton::SetLabel(const char* pLabel)
{
	if (pLabel)
	{
		m_nLabelLen = strlen(pLabel);
		if (m_nLabelLen > 31)
			m_nLabelLen = 31;
		memcpy(m_Label, pLabel, m_nLabelLen);
		m_Label[m_nLabelLen] = 0;
	}
}

int KWndLabeledButton::GetLabel(char* pLabel, int nSize)
{
	int nRet = 0;
	if (pLabel)
	{		
		if (m_nLabelLen < nSize)
		{			
			memcpy(pLabel, m_Label, m_nLabelLen + 1);
			nRet = m_nLabelLen;
		}
		else
		{
			pLabel[0] = 0;
			nRet = 0;
		}
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	���ܣ����ð�ť����������ɫ
//--------------------------------------------------------------------------
void KWndLabeledButton::SetLabelColor(unsigned int Color)
{
	m_FontColor = Color;
}

//--------------------------------------------------------------------------
//	���ܣ��������
//--------------------------------------------------------------------------
void KWndLabeledButton::PaintWindow()
{
	KWndButton::PaintWindow();
	if (g_pRepresentShell)
	{
		char	Buffer[32];
		int nMaxLen = m_Width * 2 / m_nFontSize;
		const char* pShowString = TGetLimitLenString(m_Label, -1, Buffer, nMaxLen);
		if (pShowString)
		{
			int nLen = strlen(pShowString);
			unsigned int uColor = m_FontColor;
			unsigned int uBorderColor = m_FontBorderColor;
			if (m_Flag & WNDBTN_F_OVER)
			{
				uColor = m_OverColor;
				uBorderColor = m_OverBorderColor;
			}
			g_pRepresentShell->OutputText(m_nFontSize, pShowString, nLen,
				m_nAbsoluteLeft + (m_Width - nLen * m_nFontSize / 2) / 2,
				m_nAbsoluteTop + m_nLabelYOffset,
				uColor , 0,
				TEXT_IN_SINGLE_PLANE_COORD, uBorderColor);
		}
	}
}

//=====================================================
//	���ӵĴ����ְ�ť���ڡ�
//	���ֿ���Ϊ���У�Ҳ����Ϊ���У�����ʱ�����ָ��
//	ˮƽ�Լ���ֱ�Ķ��뷽ʽ�������п��԰������Ʒ���
//=====================================================

//--------------------------------------------------------------------------
//	���ܣ���ʼ������
//--------------------------------------------------------------------------
int KWndRichLabeledButton::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndButton::Init(pIniFile, pSection))
	{
		m_Label.Init(pIniFile, pSection);
		m_Label.SetPosition(m_nAbsoluteLeft, m_nAbsoluteTop);
	}
	return false;
}

//--------------------------------------------------------------------------
//	���ܣ����ð�ť��������
//--------------------------------------------------------------------------
void KWndRichLabeledButton::SetLabel(const char* pLabel, int nLen)
{
	m_Label.SetText(pLabel, nLen);
}

//--------------------------------------------------------------------------
//	���ܣ����ð�ť����������ɫ
//--------------------------------------------------------------------------
void KWndRichLabeledButton::SetLabelColor(unsigned int Color)
{
	m_Label.SetTextColor(Color);
}

//--------------------------------------------------------------------------
//	���ܣ��������
//--------------------------------------------------------------------------
void KWndRichLabeledButton::PaintWindow()
{
	KWndButton::PaintWindow();
	m_Label.SetPosition(m_nAbsoluteLeft, m_nAbsoluteTop);
	m_Label.PaintWindow();
}
