#ifndef S3ImeH
#define	S3ImeH

#pragma comment ( lib, "imm32.lib" )
#include <windows.h>
#include <imm.h>
#include "kEngine.h"
#include "KStrBase.h"

const MAX_BUFFER=1024;

class CIme{
private:
	bool m_bIme;//ime�����־

	char m_szText[MAX_BUFFER] ;
    char* m_pszText;

	int  m_nTextCursor;
    int  m_nStrLen;

	char m_szCompStr[ MAX_PATH ];//�洢ת����Ĵ�
	char m_szCompReadStr[ MAX_PATH ];//�洢����Ĵ�


	char m_szCandList[ MAX_PATH ];//�洢������ַ���ѡ�ֱ�
	int  m_nImeCursor;//�洢ת����Ĵ��еĹ��λ��
	

	CANDIDATELIST *m_lpCandList;//�洢��׼��ѡ�ֱ�
	char m_szImeName[ 64 ];//�洢���뷨������

	bool m_bImeSharp;//ȫ�Ǳ�־
	bool m_bImeSymbol;//���ı���־

	bool m_bInsert;//��??��д״̬


	void ConvertCandList( CANDIDATELIST *pCandList, char *pszCandList );//��ѡ�ֱ�����ɴ�

	bool OnWM_INPUTLANGCHANGEREQUEST();
	bool OnWM_INPUTLANGCHANGE( HWND hWnd );
	bool OnWM_IME_SETCONTEXT(){ return true; }
	bool OnWM_IME_STARTCOMPOSITION(){ return true; }
	bool OnWM_IME_ENDCOMPOSITION(){ return true; }
	bool OnWM_IME_NOTIFY( HWND hWnd, WPARAM wParam );
	bool OnWM_IME_COMPOSITION( HWND hWnd, LPARAM lParam );
	bool onWM_CHAR(WPARAM wParam);
	bool onWM_KEYDOWN(WPARAM wParam);


public:
	CIme();
	~CIme();
	//���ƺ���
	void DisableIme();//�رղ���ֹ���뷨����ime�Ѿ�����رգ��˺���Ҳ������ȼ�����ime
	void EnableIme();//�������뷨���˺���ҿ������ȼ�����ime
	void NextIme();//�л�����һ�����뷨������EnableIme�����Ч
	void SharpIme( HWND hWnd );//�л�ȫ��/���
	void SymbolIme( HWND hWnd );//�л���/Ӣ�ı��
	//״̬����
	char* GetImeName();//�õ����뷨���֣������ǰ��Ӣ���򷵻�NULL

//  void GetTextInfo(char **pszBufferStr,int *pnTextCursor);//ȡ�õ�ǰ�������ִ�
//	void SetTextInfo(char *pszBufferStr,int pnTextCursor);//�����Ѿ�������ַ���
    bool GetText(LPSTR pszBuffer,int * nMaxCount);
	int	 GetCursorPos();

	bool SetText(LPSTR pszBuffer);
	void SetCursorPos(int nCursorPos);

    bool IfInsert();//�Ƿ����״̬

	bool IfImeSharp();//�Ƿ�ȫ��
	bool IfImeSymbol();//�Ƿ����ı��

	void GetImeInput( char **pszCompStr, char **pszCompReadStr, int *pnImeCursor, char **pszCandList );
		//�õ����뷨״̬���ĸ�ָ�������ΪNULL���״̬���ط���
		//��pszCompStr�з���ת����Ĵ�

	//��Ӧ���봰����Ϣ�����������true���򴰿ں���Ӧֱ�ӷ���0������Ӧ���ݸ�DefWindowProc
	int	WndMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	
};
#endif







