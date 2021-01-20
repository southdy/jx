#include "Ime.h"

CIme::CIme():m_lpCandList(NULL){
	DisableIme(); ;//ͨ��DisableIme��ʼ��һЩ����
	m_pszText=m_szText;
    memset(m_pszText,0,MAX_BUFFER);
	m_nTextCursor=0;
	m_nStrLen=0;
}
CIme::~CIme(){
	DisableIme();
	if( m_lpCandList ){
		GlobalFree( (HANDLE)m_lpCandList );
		m_lpCandList = NULL;
	}
}

void CIme::SetCursorPos(int nCursorPos){
	if (nCursorPos<0) 
		m_nTextCursor=0;
	else if (nCursorPos>m_nStrLen) 
	    m_nTextCursor=m_nStrLen;
	else
		m_nTextCursor=nCursorPos;
}

bool CIme::SetText(LPSTR pszBuffer)
{
	if(pszBuffer)
	{
		m_nStrLen=g_StrLen(pszBuffer);
		if (m_nStrLen <= MAX_BUFFER)
		{
			memcpy(m_pszText,pszBuffer,m_nStrLen);
			m_nTextCursor=m_nStrLen;
			return true;
		}
	};
	return false;
} 

bool CIme::GetText(LPSTR pszBuffer,int * MaxCount)
{
	if (*MaxCount > m_nStrLen && pszBuffer)
	{
		memcpy(pszBuffer, m_pszText, m_nStrLen);
		pszBuffer[m_nStrLen] = 0;
		return true;
	}
	else
	{
		*MaxCount = m_nStrLen + 1;
		return false;
	}
};

int CIme::GetCursorPos()
{
	return m_nTextCursor;
}

void CIme::DisableIme()
{
	while( ImmIsIME( GetKeyboardLayout( 0 )))
		ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );//���ime��ͨ��ѭ���л�����һ���ر�
	m_bIme = false;
	m_szImeName[ 0 ] = 0;
	m_szCompStr[ 0 ] = 0;
	m_szCompReadStr[ 0 ] = 0;
	m_szCandList[ 0 ] = 0;
	m_nImeCursor = 0;
}

void CIme::EnableIme()
{
	m_bIme = true;
}

void CIme::NextIme(){
	if( m_bIme )ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );
}
 
void CIme::SharpIme( HWND hWnd ){
	ImmSimulateHotKey( hWnd, IME_CHOTKEY_SHAPE_TOGGLE );
}

void CIme::SymbolIme( HWND hWnd ){
	ImmSimulateHotKey( hWnd, IME_CHOTKEY_SYMBOL_TOGGLE );
}



void CIme::ConvertCandList( CANDIDATELIST *pCandList, char *pszCandList ){//ת��CandidateList��һ������\t�ָ�ÿһ��
	unsigned int i;
	if( pCandList->dwCount < pCandList->dwSelection ){
		pszCandList[ 0 ] = 0;
		return;
	}
		//��ѡ����ų���������΢��ƴ���ڶ��ε�ѡ�ֱ����һҳ���ٰ�PageDown�����������������һ��˳�ѡ��״̬����ʼһ���µ�����
		//��΢��ƴ���Լ���ime���ڿ��Խ��������⣬����΢��ƴ��ʵ���˸���Ľӿڣ�����ʹ�������ֲ�̫��׼������
		//�������޷����������⣬����ʵ��ʹ����Ҳ�������������£�����������׼���뷨������������bug
		//�Ǳ�׼���뷨����ʵ�ֵĽӿڱȽ��٣�����Ӧ��Ҳ������������bug
	for( i = 0; ( i < pCandList->dwCount - pCandList->dwSelection )&&( i < pCandList->dwPageSize ); i++ ){
		*pszCandList++ = ( i % 10 != 9 )? i % 10 + '1' : '0';//ÿ���Ӧ�����ּ�
		*pszCandList++ = '.';//��'.'�ָ�
		strcpy( pszCandList, (char*)pCandList
			+ pCandList->dwOffset[ pCandList->dwSelection + i ] );//ÿ��ʵ�ʵ�����
		pszCandList += strlen( pszCandList );
		*pszCandList++ = '\t';//��֮����'\t'�ָ�
	}
	*( pszCandList - 1 )= 0;//��β�����������һ��'\t'
}
bool CIme::onWM_KEYDOWN(WPARAM wParam){
	switch(wParam){
		case VK_LEFT:
		{
			m_nTextCursor--;
			if (m_nTextCursor<0) m_nTextCursor=0;
			if (*(BYTE*)(m_pszText+m_nTextCursor)>=0xA1){
			m_nTextCursor--;
			if (m_nTextCursor<0) m_nTextCursor=0;
			}
			break;
		}
		case VK_RIGHT:
		{
			m_nTextCursor++;
			if (m_nTextCursor>m_nStrLen) m_nTextCursor=m_nStrLen;
			if (*(BYTE*)(m_pszText+m_nTextCursor)>=0xA1){
			m_nTextCursor++;
			if (m_nTextCursor>m_nStrLen) m_nTextCursor=m_nStrLen;
			}
			break;
		}
		case VK_DELETE:
		{
			
			break;
		}
		case VK_INSERT:
		{
			m_bInsert=!m_bInsert;
			break;
		}
	};
	return true;
}

bool CIme::onWM_CHAR(WPARAM wParam){

    switch(wParam){
	case VK_BACK:
		{
            if (m_nTextCursor==0) break;
			int OldCursor;
			OldCursor=m_nTextCursor;
			m_nStrLen--;
			if (m_nStrLen<0) m_nStrLen=0;
			m_nTextCursor--;
			if (m_nTextCursor<0) m_nTextCursor=0;

			if (*(BYTE*)(m_pszText+m_nTextCursor)>=0xA1)
			{
				m_nTextCursor--;
				if (m_nTextCursor<0) m_nTextCursor=0;
				m_nStrLen--;
				if (m_nStrLen<0) m_nStrLen=0;
			};
			if((m_nStrLen-m_nTextCursor)==0)
			{
			 *(m_pszText+m_nTextCursor)=0;
			}
			else
			{
			 memcpy(m_pszText+m_nTextCursor,m_pszText+OldCursor,m_nStrLen-m_nTextCursor);	
             *(m_pszText+m_nStrLen)=0 ;
			};
		break;
		};
	default:
		{
		if (m_bInsert){
		if ((m_nStrLen-m_nTextCursor)>0){
			memcpy(m_pszText+m_nTextCursor+1,m_pszText+m_nTextCursor,m_nStrLen-m_nTextCursor);
			};
		};
		*(m_pszText+m_nTextCursor++) = (char)wParam;
		m_nStrLen++;
		}
    }
    return TRUE;
}


int	CIme::WndMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int	nRet = 0;
	switch(uMsg)
	{
	case WM_KEYDOWN:
		nRet=onWM_KEYDOWN(wParam);
		break;
	case WM_CHAR:
		nRet = onWM_CHAR(wParam);
		break;
   	case WM_IME_SETCONTEXT:
		nRet = OnWM_IME_SETCONTEXT();
		break;
	case WM_INPUTLANGCHANGEREQUEST:
		nRet = OnWM_INPUTLANGCHANGEREQUEST();
		break;
	case WM_INPUTLANGCHANGE:
		nRet = OnWM_INPUTLANGCHANGE(hWnd);
		break;
	case WM_IME_STARTCOMPOSITION:
		nRet = OnWM_IME_STARTCOMPOSITION();
		break;
	case WM_IME_ENDCOMPOSITION:
		nRet =OnWM_IME_ENDCOMPOSITION();
		break;
	case WM_IME_NOTIFY:
		nRet = OnWM_IME_NOTIFY( hWnd, wParam );
		break;
	case WM_IME_COMPOSITION:
		nRet = OnWM_IME_COMPOSITION( hWnd, lParam );
		break;
	}
	return nRet;
}

bool CIme::OnWM_INPUTLANGCHANGEREQUEST(){
	return !m_bIme;//�����ֹime�򷵻�false����ʱ���ں���Ӧ����0������DefWindowProc������뷨		HIMC hIMC = ImmGetContext( hWnd );
}

bool CIme::OnWM_INPUTLANGCHANGE( HWND hWnd ){
	//ime�ı�
	HKL hKL = GetKeyboardLayout( 0 );
	if( ImmIsIME( hKL )){
		HIMC hIMC = ImmGetContext( hWnd );
	ImmEscape( hKL, hIMC, IME_ESC_IME_NAME, m_szImeName );//ȡ�������뷨����
		DWORD dwConversion, dwSentence;
		ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
		m_bImeSharp = ( dwConversion & IME_CMODE_FULLSHAPE )? true : false;//ȡ��ȫ�Ǳ�־
		m_bImeSymbol = ( dwConversion & IME_CMODE_SYMBOL )? true : false;//ȡ�����ı���־
		ImmReleaseContext( hWnd, hIMC );
	}
	else//Ӣ������
		m_szImeName[ 0 ] = 0;
	return false;//���Ƿ���false����Ϊ��Ҫ���ں�������DefWindowProc��������
}
bool CIme::OnWM_IME_NOTIFY( HWND hWnd, WPARAM wParam ){
	HIMC hIMC;
	DWORD dwSize;
	DWORD dwConversion, dwSentence;
	switch( wParam ){
		case IMN_SETCONVERSIONMODE://ȫ��/��ǣ���/Ӣ�ı��ı�
			hIMC = ImmGetContext( hWnd );
			ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
			m_bImeSharp = ( dwConversion & IME_CMODE_FULLSHAPE )? true : false;
			m_bImeSymbol = ( dwConversion & IME_CMODE_SYMBOL )? true : false;
			ImmReleaseContext( hWnd, hIMC );
			break;
		case IMN_OPENCANDIDATE://����ѡ��״̬
		case IMN_CHANGECANDIDATE://ѡ�ֱ�ҳ
			hIMC = ImmGetContext( hWnd );
			if( m_lpCandList ){
				GlobalFree( (HANDLE)m_lpCandList );
				m_lpCandList = NULL;
			}//�ͷ���ǰ��ѡ�ֱ�
			if( dwSize = ImmGetCandidateList( hIMC, 0, NULL, 0 )){
				m_lpCandList = (LPCANDIDATELIST)GlobalAlloc( GPTR, dwSize );
				if( m_lpCandList )
					ImmGetCandidateList( hIMC, 0, m_lpCandList, dwSize );
			}//�õ��µ�ѡ�ֱ�
			ImmReleaseContext( hWnd, hIMC );
			if( m_lpCandList )ConvertCandList( m_lpCandList, m_szCandList );//ѡ�ֱ�����ɴ�
			break;
		case IMN_CLOSECANDIDATE://�ر�ѡ�ֱ�
			if( m_lpCandList ){
				GlobalFree( (HANDLE)m_lpCandList );
				m_lpCandList = NULL;
			}//�ͷ�
			m_szCandList[ 0 ] = 0;
			break;
	}
	return true;//���Ƿ���true����ֹime���ڴ�
}
bool CIme::OnWM_IME_COMPOSITION( HWND hWnd, LPARAM lParam ){//����ı�
	HIMC hIMC;
	DWORD dwSize;
	hIMC = ImmGetContext( hWnd );
	if( lParam & GCS_COMPSTR ){
		dwSize = ImmGetCompositionString( hIMC, GCS_COMPSTR, (void*)m_szCompStr, sizeof( m_szCompStr ));
		m_szCompStr[ dwSize ] = 0;
	}//ȡ��szCompStr
	if( lParam & GCS_COMPREADSTR ){
		dwSize = ImmGetCompositionString( hIMC, GCS_COMPREADSTR, (void*)m_szCompReadStr, sizeof( m_szCompReadStr ));
		m_szCompReadStr[ dwSize ] = 0;
	}//ȡ��szCompReadStr
	if( lParam & GCS_CURSORPOS ){
		m_nImeCursor = 0xffff & ImmGetCompositionString( hIMC, GCS_CURSORPOS, NULL, 0 );
	}//?��?.nImeCursor
	if( lParam & GCS_RESULTSTR ){
		unsigned char str[ MAX_PATH ];
		dwSize = ImmGetCompositionString( hIMC, GCS_RESULTSTR, (void*)str, sizeof( str ));//ȡ�ú������봮
		str[ dwSize ] = 0;
		unsigned char *p = str;
		while( *p )PostMessage( hWnd, WM_CHAR, (WPARAM)(*p++), 1 );//ת��WM_CHAR��Ϣ
	}
	ImmReleaseContext( hWnd, hIMC );
	return true;//���Ƿ���true����ֹime���ڴ�
}

char* CIme::GetImeName(){
	return m_szImeName[ 0 ]? m_szImeName : NULL;
}


bool CIme::IfImeSharp(){//�Ƿ�ȫ��
	return m_bImeSharp;
}

bool CIme::IfImeSymbol(){//�Ƿ����ı��
	return m_bImeSymbol;
}

bool CIme::IfInsert(){
	return m_bInsert;
}

void CIme::GetImeInput( char **pszCompStr, char **pszCompReadStr, int *pnImeCursor, char **pszCandList ){
	if( pszCompStr )*pszCompStr = m_szCompStr;
	if( pszCompReadStr )*pszCompReadStr = m_szCompReadStr;
	if( pnImeCursor )*pnImeCursor = m_nImeCursor;
	if( pszCandList )*pszCandList = m_szCandList;
}