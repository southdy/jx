//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KDInput.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Direct Input Related Functions
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KWin32Wnd.h"
#include "KDError.h"
#include "KDInput.h"
//---------------------------------------------------------------------------
ENGINE_API KDirectInput* g_pDirectInput = NULL;
//---------------------------------------------------------------------------
// ����:	KDirectInput
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KDirectInput::KDirectInput()
{
	g_pDirectInput		= this;
	m_pDirectInput		= NULL;
	m_pMouseDevice		= NULL;
	m_pKeyboardDevice	= NULL;
}
//---------------------------------------------------------------------------
// ����:	~KDirectInput
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KDirectInput::~KDirectInput()
{
	Exit();
}
//---------------------------------------------------------------------------
// ����:	Init
// ����:	��ʼ��DirectInput
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KDirectInput::Init()
{
	// free prior obj
	if (m_pDirectInput != NULL)
		Exit();

	if (!CreateDirectInput())
	{
		g_MessageBox("DirectInput : DirectInputCreate() Failed");
		return FALSE;
	}

	// create mouse device
	if (!CreateMouseDevice())
	{
		g_MessageBox("DirectInput : CreateMouseDevice() Failed");
		return FALSE;
	}

	// create keyboard device
	if (!CreateKeyboardDevice())
	{
		g_MessageBox("DirectInput : CreateKeyboardDevice() Failed");
		return FALSE;
	}

	g_DebugLog("DirectInput init ok");
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Exit
// ����:	�ͷ�DirectInput
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KDirectInput::Exit()
{
	if (m_pMouseDevice)
	{
		m_pMouseDevice->Unacquire();
		m_pMouseDevice->Release();
		m_pMouseDevice = NULL;
	}
	if (m_pKeyboardDevice)
	{
		m_pKeyboardDevice->Unacquire();
		m_pKeyboardDevice->Release();
		m_pKeyboardDevice = NULL;
	}
	if (m_pDirectInput)
	{
		m_pDirectInput->Release();
		m_pDirectInput = NULL;
	}
	g_DebugLog("DirectInput release ok");
}
//---------------------------------------------------------------------------
// ����:	CreateDirectInput
// ����:	����DirectInput
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KDirectInput::CreateDirectInput()
{
	HRESULT  hres;

	hres = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pDirectInput,
		NULL);
#ifndef _DEBUG		
	if (hres != DI_OK)
	{
		g_DIError(hres);
		return FALSE;
	}
#endif
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	CreateMouseDevice
// ����:	���� Mouse Device
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KDirectInput::CreateMouseDevice()
{
	DWORD    Flags;
	HRESULT  hres;

	// check direct input
	if (m_pDirectInput == NULL)
		return FALSE;

	// creating the DirectInput Mouse Device
	hres = m_pDirectInput->CreateDevice(
		GUID_SysMouse,
		&m_pMouseDevice,
		NULL);
	if (hres != DI_OK)
	{
		g_DIError(hres);
		return FALSE;
	}

	// setting the Mouse Data Format
	hres = m_pMouseDevice->SetDataFormat(&c_dfDIMouse);
	if (hres != DI_OK)
	{
		g_DIError(hres);
		return FALSE;
	}

	// setting the Mouse Behavior flag
	Flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
	hres = m_pMouseDevice->SetCooperativeLevel(g_GetMainHWnd(), Flags);

#ifndef _DEBUG	
	if (hres != DI_OK)
	{
		g_DIError(hres);
		return FALSE;
	}
#endif
	// acquire obtains access to the input device
	hres = m_pMouseDevice->Acquire();
//Question ��֪Ϊʲô��_DEBUGϵͳΪδ����_DEBUG�����ڴ���һ��

#ifndef _DEBUG		
	if (hres != DI_OK)
	{
		g_DIError(hres);
		return FALSE;
	}
#endif
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	CreateKeyboardDevice
// ����:	���������豸
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KDirectInput::CreateKeyboardDevice()
{
	DWORD   Flags;
	HRESULT hres;

	// check direct input
	if (m_pDirectInput == NULL)
		return FALSE;

	// creating the DirectInput Keyboard Device
	hres = m_pDirectInput->CreateDevice(
		GUID_SysKeyboard,
        &m_pKeyboardDevice,
		NULL);
	if (hres != DI_OK)
	{
		g_DIError(hres);
		return FALSE;
	}

	// setting the Keyboard Data Format
	hres = m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (hres != DI_OK)
	{
		g_DIError(hres);
		return FALSE;
	}

	// setting the Keyboard Behavior
	Flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
	hres = m_pKeyboardDevice->SetCooperativeLevel(g_GetMainHWnd(), Flags);

#ifndef _DEBUG		
	if (hres != DI_OK)
	{
		g_DIError(hres);
		return FALSE;
	}
#endif

	// acquire obtains access to the input device
	hres = m_pKeyboardDevice->Acquire();
#ifndef _DEBUG	
	if (hres != DI_OK)
	{
		g_DIError(hres);
		return FALSE;
	}
#endif

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	SetMouseBehavior
// ����:	���� Mouse Э��ģʽ
// ����:	bExclusive �� TRUE Ϊ��ռģʽ
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KDirectInput::SetMouseBehavior(BOOL bExclusive)
{
	DWORD    Flags;
	HRESULT  hres;

	if (bExclusive)
	{
		Flags = DISCL_FOREGROUND | DISCL_EXCLUSIVE;
	}
	else
	{
		Flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
	}
	hres = m_pMouseDevice->SetCooperativeLevel(g_GetMainHWnd(), Flags);
	if (hres != DI_OK)
	{
		g_DIError(hres);
		return FALSE;
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	GetMouseState
// ����:	ȡ�����״̬
// ����:	pDx		ˮƽƫ��
//			pDy		��ֱƫ��
//			pLb		���״̬
//			pRb		�Ҽ�״̬
// ����:	void
//---------------------------------------------------------------------------
BOOL KDirectInput::GetMouseState(PINT pDx, PINT pDy, PBYTE pLb, PBYTE pRb)
{
	DIMOUSESTATE  dims;
	HRESULT       hres;

	if (m_pMouseDevice == NULL)
		return FALSE;
	hres = m_pMouseDevice->GetDeviceState(sizeof(dims), &dims);
	if (hres == DI_OK)
	{
		*pDx = dims.lX;
		*pDy = dims.lY;
		*pLb = dims.rgbButtons[0];
		*pRb = dims.rgbButtons[1];
	}
	else
	{
		m_pMouseDevice->Acquire();
		return FALSE;
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	GetKeyboardState
// ����:	ȡ�ü���״̬
// ����:	KeyBuf	״̬����
// ����:	void
//---------------------------------------------------------------------------
BOOL KDirectInput::GetKeyboardState(PBYTE KeyBuffer)
{
	if (m_pKeyboardDevice == NULL)
		return FALSE;
	if (m_pKeyboardDevice->GetDeviceState(256, KeyBuffer) != DI_OK)
	{
		m_pKeyboardDevice->Acquire();
		return FALSE;
	}
	return TRUE;
}
//---------------------------------------------------------------------------

