//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KFileDialog.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Win32 FileDialog Operation Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KMemBase.h"
#include "KStrBase.h"
#include "KWin32Wnd.h"
#include "KFileDialog.h"
//---------------------------------------------------------------------------
// ����:	KFileDialog
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KFileDialog::KFileDialog()
{
	// init OpenFileName members
	m_OpenFileName.lStructSize		= sizeof(OPENFILENAME);
	m_OpenFileName.hwndOwner		= NULL;
	m_OpenFileName.hInstance		= NULL;
	m_OpenFileName.lpstrFilter		= m_szFilter;
	m_OpenFileName.lpstrCustomFilter= NULL;
	m_OpenFileName.nMaxCustFilter	= 0;
	m_OpenFileName.nFilterIndex		= 0;
	m_OpenFileName.lpstrFile		= m_szFileName;
	m_OpenFileName.nMaxFile			= sizeof(m_szFileName);
	m_OpenFileName.lpstrFileTitle	= m_szFileTitle;
	m_OpenFileName.nMaxFileTitle	= sizeof(m_szFileTitle);
	m_OpenFileName.lpstrInitialDir	= m_szFilePath;
	m_OpenFileName.lpstrTitle		= m_szTitle;
	m_OpenFileName.Flags			= 0;
	m_OpenFileName.nFileOffset		= 0;
	m_OpenFileName.nFileExtension	= 0;
	m_OpenFileName.lpstrDefExt		= m_szDefExt;
	m_OpenFileName.lCustData		= 0;

	// init FileDialog members
	m_szFileName[0]		= 0;
	m_szFilePath[0]		= 0;
	m_szFileTitle[0]	= 0;
	m_szTitle[0]		= 0;
	m_szFilter[0]		= 0;
	m_szDefExt[0]		= 0;
}
//---------------------------------------------------------------------------
// ����:	SetTitle
// ����:	�����ļ��Ի����ı���
// ����:	szTitle		����
// ����:	void
//---------------------------------------------------------------------------
void KFileDialog::SetTitle(LPSTR szTitle)
{
	g_StrCpy(m_szTitle, szTitle);
}
//---------------------------------------------------------------------------
// ����:	SetFlags
// ����:	�����ļ��Ի����ı�־
// ����:	Flags	��־
//				OFN_ALLOWMULTISELECT - �����ѡ
//				OFN_OVERWRITEPROMPT - �ļ�����ʱ��ʾ��ʾ
//				OFN_EXPLORER - ��������
// ����:	void
//---------------------------------------------------------------------------
void KFileDialog::SetFlags(DWORD Flags)
{
	m_OpenFileName.Flags = Flags | OFN_EXPLORER;
}
//---------------------------------------------------------------------------
// ����:	SetFilter
// ����:	�����ļ��Ի������ļ���չ������
// ����:	szFilter	������
//			����"Text files (*.txt)|*.TXT";
//				"Text files (*.txt)|*.TXT|Pascal files (*.pas)|*.PAS";
// ����:	void
//---------------------------------------------------------------------------
void KFileDialog::SetFilter(LPSTR szFilter)
{
	int i;

	// copy filter string
	g_StrCpy(m_szFilter, szFilter);

	// convert '|' to '\0'
	for (i = 0; m_szFilter[i]; i++)
	{
		if (m_szFilter[i] == '|')
			m_szFilter[i] = 0;
	}

	// double null terminators
	m_szFilter[i + 1] = 0;
}
//---------------------------------------------------------------------------
// ����:	SetDefExt
// ����:	�����ļ��Ի�����ȱʡ��չ��
// ����:	szDefExt	ȱʡ��չ��
// ����:	void
//---------------------------------------------------------------------------
void KFileDialog::SetDefExt(LPSTR szDefExt)
{
	g_StrCpy(m_szDefExt, szDefExt);
}
//---------------------------------------------------------------------------
// ����:	SetFilePath
// ����:	�����ļ�·��
// ����:	lpstrPath	�ļ�·��
// ����:	void
//---------------------------------------------------------------------------
void KFileDialog::SetFilePath(LPSTR lpstrPath)
{
	if (lpstrPath[0] == '\\')
	{
		g_SetFilePath(lpstrPath);
		g_GetFullPath(m_szFilePath, lpstrPath);
	}
	else
	{
		g_StrCpy(m_szFilePath, lpstrPath);
	}
}
//---------------------------------------------------------------------------
// ����:	SetFileName
// ����:	�����ļ��Ի������ļ���
// ����:	lpstrFile	�ļ���
// ����:	void
//---------------------------------------------------------------------------
void KFileDialog::SetFileName(LPSTR lpstrFile)
{
	g_StrCpy(m_szFileName, lpstrFile);
}
//---------------------------------------------------------------------------
// ����:	SetFileTitle
// ����:	�����ļ��Ի������ļ���
// ����:	lpstrFile	�ļ���
// ����:	void
//---------------------------------------------------------------------------
void KFileDialog::SetFileTitle(LPSTR lpstrFile)
{
	g_StrCpy(m_szFileTitle, lpstrFile);
}
//---------------------------------------------------------------------------
// ����:	Open
// ����:	�ļ��򿪶Ի���
// ����:	void
// ����:	�ɹ�����TRUE��ʧ�ܻ���ȡ������FALSE��
//---------------------------------------------------------------------------
BOOL KFileDialog::Open()
{
	m_OpenFileName.hwndOwner = g_GetMainHWnd();
	if (GetOpenFileName(&m_OpenFileName))
	{
		g_StrCpyLen(m_szFilePath, m_szFileName, m_OpenFileName.nFileOffset);
		return TRUE;
	}
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	Save
// ����:	�ļ�����Ի���
// ����:	void
// ����:	�ɹ�����TRUE��ʧ�ܻ���ȡ������FALSE��
//---------------------------------------------------------------------------
BOOL KFileDialog::Save()
{
	m_OpenFileName.hwndOwner = g_GetMainHWnd();
	if (GetSaveFileName(&m_OpenFileName))
	{
		g_StrCpyLen(m_szFilePath, m_szFileName, m_OpenFileName.nFileOffset);
		return TRUE;
	}
	return FALSE;
}
//---------------------------------------------------------------------------

