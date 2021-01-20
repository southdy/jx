//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KWin32Wnd.h.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Window Functions
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KDDraw.h"
#include "KWin32Wnd.h"
//---------------------------------------------------------------------------
static HWND	g_hWndMain = NULL;
static HWND	g_hWndDraw = NULL;
//---------------------------------------------------------------------------
// ����:	GetMainHWnd
// ����:	ȡ�������ھ��
// ����:	void
// ����:	�����ھ��(�п���ΪNULL)
//---------------------------------------------------------------------------
HWND g_GetMainHWnd(void)
{
	return g_hWndMain;
}
//---------------------------------------------------------------------------
// ����:	SetMainHWnd
// ����:	���������ھ��
// ����:	hWnd	�����ھ��
// ����:	void
//---------------------------------------------------------------------------
void g_SetMainHWnd(HWND hWnd)
{
	g_hWndMain = hWnd;
}
//---------------------------------------------------------------------------
// ����:	GetDrawHWnd
// ����:	ȡ�û�ͼ���ھ��
// ����:	void
// ����:	���ھ��(�п���ΪNULL)
//---------------------------------------------------------------------------
HWND g_GetDrawHWnd(void)
{
	return g_hWndDraw;
}
//---------------------------------------------------------------------------
// ����:	SetDrawHWnd
// ����:	���û�ͼ���ھ��
// ����:	hWnd	���ھ��
// ����:	void
//---------------------------------------------------------------------------
void g_SetDrawHWnd(HWND hWnd)
{
	g_hWndDraw = hWnd;
}
//---------------------------------------------------------------------------
// ����:	Get Client Rect
// ����:	ȡ�ô��ڿͻ��������
// ����:	lpRect	��������
// ����:	void
//---------------------------------------------------------------------------
void g_GetClientRect(LPRECT lpRect)
{
	if (g_pDirectDraw->GetScreenMode() == FULLSCREEN)
	{
		lpRect->left = 0;
		lpRect->top = 0;
		lpRect->right = g_pDirectDraw->GetScreenWidth();
		lpRect->bottom = g_pDirectDraw->GetScreenHeight();
	}
	else
	{
		GetClientRect(g_hWndDraw, lpRect);
	}
}
//---------------------------------------------------------------------------
// ����:	Client To Screen
// ����:	�ͻ����꣭��Ļ����
// ����:	lpRect
// ����:	void
//---------------------------------------------------------------------------
void g_ClientToScreen(LPRECT lpRect)
{
	if (g_pDirectDraw->GetScreenMode() == WINDOWMODE)
	{
		ClientToScreen(g_hWndDraw, (LPPOINT)lpRect);
		ClientToScreen(g_hWndDraw, (LPPOINT)lpRect + 1);
	}
}
//---------------------------------------------------------------------------
// ����:	Screen To Client
// ����:	��Ļ���꣭�ͻ�����
// ����:	lpRect
// ����:	void
//---------------------------------------------------------------------------
void g_ScreenToClient(LPRECT lpRect)
{
	if (g_pDirectDraw->GetScreenMode() == WINDOWMODE)
	{
		ScreenToClient(g_hWndDraw, (LPPOINT)lpRect);
		ScreenToClient(g_hWndDraw, (LPPOINT)lpRect + 1);
	}
}
//---------------------------------------------------------------------------

void g_ScreenToClient(LPPOINT lpPoint)
{
	if (g_pDirectDraw->GetScreenMode() == WINDOWMODE)
		ScreenToClient(g_hWndDraw, lpPoint);
}