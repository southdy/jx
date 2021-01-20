//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KEngine.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Engine Init and Exit
//---------------------------------------------------------------------------
#include "kwin32.h"
#include "KEngine.h"
//---------------------------------------------------------------------------
// ����:	InitEngine
// ����:	��ʼ������
// ����:	void
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL g_InitEngine()
{
	// find debug window
	g_FindDebugWindow("TMainForm", "MyDebug");

	// set root path
	g_SetRootPath(NULL);

	// init ddraw
	if (g_pDirectDraw)
		if (!g_pDirectDraw->Init())
			return FALSE;
	
	// init dinput
	if (g_pDirectInput)
		if (!g_pDirectInput->Init())
			return FALSE;
	
	// init dsound
	if (g_pDirectSound)
		g_pDirectSound->Init();

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	ExitEngine
// ����:	�˳�����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void g_ExitEngine()
{
	if (g_pDirectDraw)
		g_pDirectDraw->Exit();
	if (g_pDirectInput)
		g_pDirectInput->Exit();
	if (g_pDirectSound)
		g_pDirectSound->Exit();
}
//---------------------------------------------------------------------------
