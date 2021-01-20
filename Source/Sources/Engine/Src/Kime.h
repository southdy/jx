//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2002 by Kingsoft
//
// File:	KIme.h
// Date:	2002.07.18
// Code:	��ѧ��
// Desc:	KIme Class
//---------------------------------------------------------------------------

#ifndef S3ImeH
#define	S3ImeH

#pragma comment ( lib, "imm32.lib" )
#include <windows.h>
#include <imm.h>

#define  MAX_IME_BUFFER  128

class ENGINE_API KIme{
private:
	bool  m_bNoLanguageChange;
	bool  m_bIsWorking;
    bool  m_bCloseFlag;
    bool  m_bCloseFlagForCanChange;
    DWORD m_dwConversion;
    DWORD m_dwSentence;
    bool  m_bCloseIMESimHotKey;
    int   m_nCaretX;
    int   m_nCaretY;
//	char  m_InputLength[MAX_IME_BUFFER];
public:
//    BYTE  szInputBuf[MAX_IME_BUFFER]; 

	KIme();
	~KIme();

	void ResetIme(void);
//	BOOL InsertWord(int nWord);
	
	void EnableLanguageChange();
	void DisableLanguageChange();
	void CloseIME();
    void OpenIME();
	BOOL IsIme();
	void TurnOff();
	void TurnOn();

    void SetCaretPos(int X, int Y);     // Ϊ��֧�ֹ�����

	//��Ӧ���봰����Ϣ�����������true���򴰿ں���Ӧֱ�ӷ���0������Ӧ���ݸ�DefWindowProc
	int	WndMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	
};
extern ENGINE_API KIme* g_pIme;

#endif







