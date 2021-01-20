//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
// 
// File:	KEicScript.cpp
// Date:	2001-11-6
// Code:	Daphnis
// Desc:	Eic script engine
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KStrBase.h"
#include "KFilePath.h"
#include "KEicScript.h"
//---------------------------------------------------------------------------
typedef	void (*EiC_init_EiC)();
typedef	int  (*EiC_run)(int argc, char **argv);
typedef	void (*EiC_stdCLib)();
//---------------------------------------------------------------------------
static HMODULE			m_hDll = NULL;
static EiC_init_EiC		EicInit = NULL;
static EiC_run			EicRun = NULL;
static EiC_stdCLib		EicStdLib = NULL;
static BOOL				LoadEicLib();
static void				FreeEicLib();
//---------------------------------------------------------------------------
// ����:	KEicScript::LoadEicLib
// ����:	
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL LoadEicLib()
{
	if (m_hDll)
		return TRUE;
	char szPathName[256];
	g_GetFullPath(szPathName, "\\eiclib.dll");
	m_hDll = LoadLibrary(szPathName);
	if (m_hDll == NULL)
	{
		g_DebugLog("Can't load Eiclib.dll!");
		return FALSE;
	}
	EicInit   = (EiC_init_EiC)GetProcAddress(m_hDll, "EiC_init_EiC");
	EicRun    = (EiC_run)GetProcAddress(m_hDll, "EiC_run");
	EicStdLib = (EiC_stdCLib)GetProcAddress(m_hDll, "EiC_stdCLib");
	if ((!EicInit)||(!EicRun)||(!EicStdLib))
	{
		g_DebugLog("There are errors in Eiclib.dll!");
		FreeEicLib();
		return FALSE;
	}
	g_DebugLog("Eiclib.dll load Ok!");
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	KEicScript::FreeEicLib
// ����:	
// ����:	void
//---------------------------------------------------------------------------
void FreeEicLib()
{
	if (m_hDll)
		FreeLibrary(m_hDll);
	m_hDll = NULL;
}
//---------------------------------------------------------------------------
// ����:	KEicScript::Init
// ����:	
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL KEicScript::Init()
{
	// load eiclib.dll
	if (!LoadEicLib())
		return FALSE;
	// init eic lib
	EicInit();
	// add std function lib
	EicStdLib();
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	KEicScript::Exit
// ����:	
// ����:	void
//---------------------------------------------------------------------------
void KEicScript::Exit()
{
	FreeEicLib();
}
//---------------------------------------------------------------------------
// ����:	KEicScript::Load
// ����:	
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL KEicScript::Load(char* FileName)
{
	if (!m_hDll)
		return FALSE;
	g_StrCpy(m_FileName, FileName);
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	KEicScript::Compile
// ����:	
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL KEicScript::Compile(char* FileName)
{
	if (!m_hDll)
		return FALSE;
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	KEicScript::Execute
// ����:	
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL KEicScript::Execute(int argc, char** argv)
{
	if (!m_hDll)
		return FALSE;
	char* ppFile = m_FileName;
	EicRun(1, &ppFile);
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	KEicScript::RegisterFunction
// ����:	
// ����:	
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KEicScript::RegisterFunction(LPSTR FuncName, void* Func)
{
	if (!m_hDll)
		return FALSE;
	return TRUE;
}
//---------------------------------------------------------------------------
