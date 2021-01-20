//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KKeyboard.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Keyboard Functions
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KDInput.h"
#include "KMessage.h"
#include "KKeyboard.h"
//---------------------------------------------------------------------------
ENGINE_API KKeyboard* g_pKeyboard = NULL;
//---------------------------------------------------------------------------
// ����:	KKeyboard
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KKeyboard::KKeyboard()
{
	g_pKeyboard = this;
	g_MemZero(m_NewBuf, sizeof(m_NewBuf));
	g_MemZero(m_OldBuf, sizeof(m_OldBuf));
}
//---------------------------------------------------------------------------
// ����:	UpdateState
// ����:	ˢ�¼���״̬
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KKeyboard::UpdateState()
{
	int nKey;

	if(g_pDirectInput->GetKeyboardState(m_NewBuf)==FALSE)
	{
//		memset(m_NewBuf,0,256);
		return;
    }
	MakeFuncKey();

	for (nKey = 0x01; nKey <= 0x58; nKey++)
		MakeMessage(nKey);

	for (nKey = 0x9C; nKey <= 0xDC; nKey++)
		MakeMessage(nKey);
}
//---------------------------------------------------------------------------
// ����:	IsDown
// ����:	�ж�һ�����Ƿ񱻰���
// ����:	byKey	����
// ����:	TRUE������ FALSE��̧��
//---------------------------------------------------------------------------
BOOL KKeyboard::IsDown(BYTE byKey)
{
	if(m_NewBuf[byKey]) 
     return TRUE;
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	MakeFuncKey
// ����:	ˢ�¹��ܼ�״̬
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KKeyboard::MakeFuncKey()
{
	m_FuncKey = 0;

	if (m_NewBuf[DIK_LSHIFT] | m_NewBuf[DIK_RSHIFT])
	{
		m_FuncKey |= KEY_SHIFT;
	}

	if (m_NewBuf[DIK_LCONTROL] | m_NewBuf[DIK_RCONTROL])
	{
		m_FuncKey |= KEY_CTRL;
	}

	if (m_NewBuf[DIK_LALT] | m_NewBuf[DIK_RALT])
	{
		m_FuncKey |= KEY_ALT;
	}
}
//---------------------------------------------------------------------------
// ����:	MakeMessage
// ����:	���ͼ�����Ϣ
// ����:	nKey	����
// ����:	void
//---------------------------------------------------------------------------
void KKeyboard::MakeMessage(int nKey)
{
	if (m_OldBuf[nKey] != m_NewBuf[nKey])
	{
		if (m_NewBuf[nKey] & 0x80)
		{
			g_SendMessage(TM_KEY_DOWN, nKey, m_FuncKey);
		}
		else
		{
			g_SendMessage(TM_KEY_UP, nKey, m_FuncKey);
		}
		m_OldBuf[nKey] = m_NewBuf[nKey];
	}
	else
	{
		if (m_NewBuf[nKey] & 0x80)
		{
			g_SendMessage(TM_KEY_HOLD, nKey, m_FuncKey);
		}
	}
}
//---------------------------------------------------------------------------
