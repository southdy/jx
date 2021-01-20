//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNetClient.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Simple double linked node class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KWin32Wnd.h"
#include "KDebug.h"
#include "KStrBase.h"
#include "KHashtable.h"
#include "KMemManager.h"
#include "KNetClient.h"
#include "KCodecLha.h"
#include "KMemBase.h"
#include "crtdbg.h"

//---------------------------------------------------------------------------

enum    //s2cϵͳЭ�飬���31
{
	s2c_passwordchange = 1,
	s2c_replyping,
	s2c_shakehand,
};
enum  //c2sϵͳЭ�飬���31
{
	c2s_replyshakehand = 1,
	c2s_sendping,
	c2s_passwordrecieved,	
};


#define N_SOCKET_MIN_PORTNUM   (7000)
#define N_SOCKET_MAX_PORTNUM   (7999)
#define N_SOCKET_PORTRANGE     (N_SOCKET_MAX_PORTNUM-N_SOCKET_MIN_PORTNUM)
#define	NET_DEBUG

KNetClient* g_pNetClient=NULL;

//---------------------------------------------------------------------------
// ����:	KNetClient
// ����:	constructor
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KNetClient::KNetClient()
{
	g_pNetClient=this;
	m_Socket = 0;
	m_ServerPort = 0;
    pCodec=NULL;	
    g_InitCodec((KCodec**)&pCodec, CODEC_LHA);
	
	memset(&(m_ServerAddr),0,sizeof(m_ServerAddr));
	memset(&(m_ServerHostName),0,sizeof(m_ServerHostName));
	memset(&(m_ServerPortName),0,sizeof(m_ServerPortName));
	
	Ping=0;
    memset(&RecvBuffer,0,sizeof(RECV_BUFFER)); 
    memset(&SysCommand,0,sizeof(COMMAND)); 
    memset(&CoreCommand,0,sizeof(COMMAND)); 
    memset(&RecvData,0,sizeof(RECV_DATA));
    memset(&BackRecvData,0,sizeof(RECV_DATA));
    BackOffset=0;
	
    DataNum = 0;
	dwDataSize=0;
    memset(SendBuffer,0,sizeof(SendBuffer));

    memset(Cipher,0,16);
    memset(LastCipher,0,16);
	CipherVerify=0;
	RecvDataSize=0;
}
//---------------------------------------------------------------------------
// ����:	~KNetClient
// ����:	destructor
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KNetClient::~KNetClient()
{
	Close();
	g_FreeCodec((KCodec**)&pCodec, CODEC_LHA);
	g_pNetClient=NULL;	
}
//---------------------------------------------------------------------------
// ����:	Open
// ����:	
// ����:	void
// ����:	bool
//---------------------------------------------------------------------------
bool KNetClient::Open()
{
	WORD version = MAKEWORD(2, 0);//ver2.0
	struct WSAData wsa_data;
	int error = WSAStartup(version, &wsa_data);
	if (error)
		g_DebugLog("KNetClient::Open(): WSAStartup() failed");
	return (error == 0);
}
//---------------------------------------------------------------------------
// ����:	close
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KNetClient::Close()
{
	if (m_Socket)
	{
		shutdown(m_Socket, 2);
		closesocket(m_Socket);
		m_Socket = 0;
	}
	WSACleanup();
}
//---------------------------------------------------------------------------
// ����:	FillServerAddr()
// ����:	set node name
// ����:	char*
// ����:	void
//---------------------------------------------------------------------------
bool KNetClient::FillServerAddr(char *name)
{
	char name_buf[128];
	char *hostname = NULL;
	char *portname = NULL;
	char *tmp;
	struct hostent *lphost;
	
	strncpy(name_buf, name, sizeof(name_buf));
	name_buf[sizeof(name_buf) - 1] = 0;
	
	if ((tmp = strchr(name_buf,':')))
	{
		*tmp = 0;
		hostname = name_buf;
		portname = tmp + 1;
	}
	else
	{
		hostname = "localhost";
		portname = name_buf;
	}
	
	strncpy(m_ServerHostName, hostname, sizeof(m_ServerHostName));
	strncpy(m_ServerPortName, portname, sizeof(m_ServerPortName));
	
	m_ServerHostName[sizeof(m_ServerHostName) - 1] = 0;
	m_ServerPortName[sizeof(m_ServerPortName) - 1] = 0;
	
	if (strlen(hostname) == 0)
	{
		g_DebugLog("KNetClient(): hostname broken before ':')");
		return FALSE;
	}
	if (strlen(portname)==0)
	{
		g_DebugLog("KNetClient(): portname broken or no portname");
		return FALSE;
	}
	
	lphost = gethostbyname(hostname);
	if (!lphost)
	{
		g_DebugLog("KNetClient(): unknown host!");
		return FALSE;
	}
	
	m_ServerPort = GetPortNumFromName(portname);
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_port   = htons(m_ServerPort);
	m_ServerAddr.sin_addr = *((struct in_addr *)lphost->h_addr);
	
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	GetPortNumFromName()
// ����:	set node name
// ����:	char*
// ����:	void
//---------------------------------------------------------------------------
short KNetClient::GetPortNumFromName(char *pname)
{
	long pnum = g_Hash(pname) + N_SOCKET_MIN_PORTNUM;
	return (short)pnum;
} 
//---------------------------------------------------------------------------
// ����:	Connect
// ����:	
// ����:	char*
// ����:	bool
//---------------------------------------------------------------------------
bool KNetClient::Connect(char *name)
{
//	KASSERT(m_Socket == 0);
	if (m_Socket)
		return FALSE;
	
	bool retval = FALSE;
	
	if (!FillServerAddr(name))
		return FALSE;
	
	int  res;
	bool connected = FALSE;
	bool error     = FALSE;
	long num_retries = 0;
	
	while ((!connected) && (!error) && (num_retries < 4))
	{
		// close socket
		if (m_Socket)
		{
			shutdown(m_Socket, 2);
			closesocket(m_Socket);
			m_Socket = 0;
			num_retries++;
		}
		
		// open socket
		m_Socket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_Socket == INVALID_SOCKET)
		{
			g_DebugLog("KNetClient::Connect(): could not create socket!");
			return FALSE;
		}
		
		// try to connect to server
		g_DebugLog("KNetClient: trying %s:%s, port %d...\n",
			m_ServerHostName, m_ServerPortName, m_ServerPort);
		res = connect(m_Socket, (struct sockaddr *) &(m_ServerAddr),
			sizeof(struct sockaddr));
		if (res == SOCKET_ERROR)
		{
			int err=WSAGetLastError();
			g_DebugLog("KNetClient::Connect(): wrong portname.\n");
			return FALSE;
		}
		
		
		connected=TRUE;
	}
	
	if (connected)
	{
		g_DebugLog("KNetClient: connected.\n");
		retval = TRUE;
	}
	else
	{
		g_DebugLog("KNetClient: Could not connect to a valid server!\n");
		shutdown(m_Socket,2);
		closesocket(m_Socket);
		m_Socket = 0;
	}
	res =WSAAsyncSelect(m_Socket, g_GetMainHWnd(), WM_SOCKET_READ, FD_READ );
    if(res==SOCKET_ERROR)
	{
		g_DebugLog("WSAAsyncSelect failed on socket");
		return FALSE;
	}
	return retval;
}																   

//---------------------------------------------------------------------------
// ����:	SendMsg
// ����:	
// ����:	void*, long
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KNetClient::SendMsg(void *buf, long size)
{
    WORD *pTemp;
	KASSERT(buf);
	KASSERT(size > 0);
	
	if(size>(SEND_DATA_SIZE-2)) return 0;//too big
	pTemp=(WORD *)SendBuffer;
	*pTemp=(WORD)(size+2);
	memcpy(SendBuffer+2,buf,size);
	int res;
	res = send(m_Socket, (char *) SendBuffer, size+2, 0);
	
#ifdef NET_DEBUG
	//����ʹ�øô���	
	char szMsg[200];
	g_StrCpy(szMsg, "C2S:");
	//sprintf(szMsg, "%s[%d]", szMsg, size); 
	BYTE * pBuf = (PBYTE)buf;
	
	int nSize = size;
	if (size > 50) nSize = 50;
	
	for (int i = 0; i < nSize; i++)
	{
		sprintf(szMsg, "%s %02x", szMsg, *(pBuf++));
	}
	g_DebugLog(szMsg);
#endif
	
	if (res == SOCKET_ERROR||res==0)
	{
		g_DebugLog("KNetClient::SendMsg(): send() failed!");
		return FALSE;
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	RecieveData
// ����:	��Ҫ���͸������������ݷ�����صķ����
// ����:	BYTE *pBuffer,DWORD dwSize
// ����:	int
//---------------------------------------------------------------------------
int KNetClient:: RecieveData(BYTE *pBuffer,DWORD dwSize)
{
    WORD *pTemp;
	KASSERT(pBuffer);
	KASSERT(dwSize > 0);
	if((dwDataSize+dwSize+2)>SEND_DATA_SIZE) 
     SendPackage();
	DataNum++;
    pTemp=(WORD *)(SendBuffer+dwDataSize);
	*pTemp=(WORD)(dwSize+2);
	memcpy(SendBuffer+dwDataSize+2, pBuffer, dwSize);
	dwDataSize+=(WORD)dwSize+2;
	return 0;
}



//---------------------------------------------------------------------------
// ����:	SendPackage
// ����:	�����ִ����ݰ�
// ����:	void
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KNetClient::SendPackage(void)
{
	int res;
	if(dwDataSize==0) return FALSE;
	res = send(m_Socket, (char *) SendBuffer, dwDataSize, 0);
	DataNum = 0;
	dwDataSize=0;
	
	if (res == SOCKET_ERROR||res==0)
	{
		g_DebugLog("KNetClient::SendMsg(): send() failed!");
		return FALSE;
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	FindStringInMem
// ����:	Ѱ���ڴ��е�ָ���ַ���������������0
// ����:	char *,DWORD ,char * 
// ����:	char *
//---------------------------------------------------------------------------
char * KNetClient::FindStringInMem(char *pMem,DWORD dwSize,char *pString)
{
	int i,len=strlen(pString);
	char *pTemp=pMem;
	
	if((int)dwSize<len) return NULL;
	
	for(i=0;i<((int)dwSize-len);i++) 
	{
		if((*pTemp)!=(*pString)) 
		{
			pTemp++;
			continue;
		}
		if(g_MemComp(pTemp,pString,len)==TRUE) return pTemp;  
		
		pTemp++;
		continue;
	}
	return NULL;
}
//---------------------------------------------------------------------------
// ����:	SortProtocol
// ����:	
// ����:	void 
// ����:	void
//---------------------------------------------------------------------------
void KNetClient::SortProtocol(void)
{
	int i;
	BYTE *pTemp=RecvData.Data;
    DWORD Num=0,dwSysSize=0;

	for(i=0;i<(int)RecvData.DataNum;i++)
	{		
		switch(*pTemp)
		{
        case 0:
			g_DebugLog("Sys Protocol:0");
			pTemp++;
			dwSysSize++;
            continue;
			break;
		case s2c_shakehand:
			pTemp+=4;
			dwSysSize+=4;
			Num++;
			g_DebugLog("Sys Protocol:shakehand");
            continue;
			break;
		case s2c_replyping:
			pTemp+=8;
			dwSysSize+=8;
			Num++;
            continue;
			break;
		case s2c_passwordchange:
			pTemp+=20;
			dwSysSize+=20;
			Num++;
            continue;
			break;
		default:
			if(Num<=RecvData.DataNum)
			{   
				if(SysCommand.dwCurrentOffset<63*1024)
				{
					memcpy(SysCommand.Data+SysCommand.dwCurrentOffset,RecvData.Data,dwSysSize);
				    SysCommand.dwCurrentOffset+=dwSysSize;
				    SysCommand.dwNum+=Num;
				}
				if(CoreCommand.dwCurrentOffset<63*1024)
				{
					memcpy(CoreCommand.Data+CoreCommand.dwCurrentOffset,RecvData.Data+dwSysSize,RecvData.dwSize-dwSysSize);
					CoreCommand.dwCurrentOffset+=RecvData.dwSize-dwSysSize;
					CoreCommand.dwNum+=RecvData.DataNum-Num;
				}
			}
			g_DebugLog("Sorting Protocol...:%d,%d", RecvData.DataNum, CoreCommand.dwNum);
			return;
			break;
		}
	}
	
}
//---------------------------------------------------------------------------
// ����:	DeCompress
// ����:	
// ����:	void 
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KNetClient::DeCompress(RECV_DATA* pData)
{
TCodeInfo tCodeInfo;
BYTE      SwapBuf[RECV_DATA_SIZE+RECV_DATA_SIZE/10+1024];
        
        if(pData->isCompressed) //��ѹ��
		{
			tCodeInfo.lpData=SwapBuf;
			tCodeInfo.dwDataLen=pData->dwSize;
			tCodeInfo.lpPack=pData->Data;
			tCodeInfo.dwPackLen =pData->dwCompSize;
			BOOL reval=((KCodecLha *)pCodec)->Decode(&tCodeInfo);   
			if(reval==FALSE) return FALSE;
			memcpy(RecvData.Data,SwapBuf,pData->dwSize);
		}
		else
           	memcpy(RecvData.Data,pData->Data,pData->dwSize);
		memcpy(&RecvData, pData, sizeof(RECV_DATA)-RECV_DATA_SIZE);
		return TRUE;
}



//---------------------------------------------------------------------------
// ����:	DataProcess
// ����:	
// ����:	void 
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KNetClient::DataProcess(void)
{
	int infoflag=0; 
	RECV_DATA*  pTemp; 
	DWORD dwSize,offset=0;
    memset(&RecvData,0,sizeof(RECV_DATA));

//�����ǰ���пհ�������BackRecvData���գ��򲹵���β��,�������BackRecvData   	
  pTemp =(RECV_DATA *)FindStringInMem((char *)RecvBuffer.Data,RecvBuffer.dwSize,"PACK");	
  if(pTemp==NULL) return FALSE;
  dwSize=(BYTE *)pTemp-RecvBuffer.Data;
  if(pTemp!=(RECV_DATA *)RecvBuffer.Data&&BackOffset)
  {
	  memcpy(BackRecvData.Data+BackOffset, RecvBuffer.Data,dwSize);
      BackOffset+=dwSize;
	  if(DeCompress(&BackRecvData)==TRUE)
	  {
		infoflag++;
		//����
		Decipher();
		//�ּ��ϵͳЭ�����Ϸ����Э��
		SortProtocol();
	  }
  }
     offset+=dwSize;
     BackOffset=0;
     

	while(TRUE)
	{
		pTemp =(RECV_DATA *)FindStringInMem((char *)RecvBuffer.Data+offset,RecvBuffer.dwSize-offset,"PACK");
		if(pTemp==NULL)
		{
			if(infoflag) 
				return TRUE;
			else
				return FALSE; 
		}
		dwSize=RecvBuffer.dwSize-((BYTE *)pTemp-RecvBuffer.Data);
		//���ʣ�µĳ��Ȳ���һ����ͷ����궨���ȣ�����󱸰�
        if(dwSize<(sizeof(RECV_DATA)-RECV_DATA_SIZE))
		{
		 memcpy(&BackRecvData,pTemp, dwSize);
         BackOffset=dwSize;
			if(infoflag) 
				return TRUE;
			else
				return FALSE; 
		}
		//���ʣ�����ݲ���һ������ʵ�ʳ��ȣ��ŵ��󱸰�
		if (dwSize<(pTemp->dwCompSize + sizeof(RECV_DATA)-RECV_DATA_SIZE))
		{
		 memcpy(&BackRecvData,pTemp, dwSize);
         BackOffset=dwSize;
			if(infoflag) 
				return TRUE;
			else
				return FALSE; 
		}


        if(DeCompress(pTemp)==FALSE) continue;
		infoflag++;
		//����
		Decipher();
		//�ּ��ϵͳЭ�����Ϸ����Э��
		SortProtocol();
		dwSize=pTemp->dwCompSize + sizeof(RECV_DATA)-RECV_DATA_SIZE;
		offset+=dwSize;
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	RecvMsg
// ����:	
// ����:	void*, long
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KNetClient::RecvMsg(void)
{
	int res;
	
	res = recv(m_Socket,  (char *)RecvBuffer.Data,64*1024, 0);
	if (res == SOCKET_ERROR||res==0)
	{
		g_DebugLog("KNetClient::RecvMsg(): recv() failed!");
		return FALSE;
	}
	RecvBuffer.dwSize=res;
	if(res>0) RecvDataSize+=res;
#ifdef NET_DEBUG
	if (res >= 2048)
		g_DebugLog("Receive Size: %d", res);
#endif
    if(DataProcess()==FALSE)
	{
		g_DebugLog("KNetClient::DataProcess ERROR!");
		return FALSE;         
	}
	RecvFlag=1;
	return TRUE;  
}
//---------------------------------------------------------------------------
// ����:	MessagePreProcess
// ����:	
// ����:	void
// ����:	int
//---------------------------------------------------------------------------
int KNetClient::MessagePreProcess(void)
{
	BYTE *pTemp;
	DWORD *pTemp1;
    BYTE  Buffer[32];
    int i;
	
	pTemp=SysCommand.Data;
	
	for(i=0;i<(int)SysCommand.dwNum;i++)
	{		
		switch(*pTemp)
		{
		case s2c_shakehand:
			Buffer[0]=c2s_replyshakehand;
			SendMsg(Buffer,4);
			pTemp+=4;
			continue;
			break;
		case s2c_replyping:
			pTemp1=(DWORD *)(pTemp+4);
			Ping=(GetTickCount()-*pTemp1)/2; 
			pTemp+=8;
			continue;
			break;
		case s2c_passwordchange:
			memcpy(LastCipher,Cipher,16);
			memcpy(Cipher,pTemp+4,16);
			Buffer[0]=c2s_passwordrecieved;
			SendMsg(Buffer,4);
			pTemp+=20;
			continue;
			break;
		default:
			return 1;
			break;
		}
	}
	return 0;
}

//---------------------------------------------------------------------------
// ����:	SendPing()
// ����:	
// ����:	void
// ����:	int
//---------------------------------------------------------------------------
int KNetClient::SendPing(void)
{
	BYTE  Buffer[32];
	DWORD *pTemp=(DWORD *)(Buffer+4);
	*pTemp=GetTickCount();
    Buffer[0]=c2s_sendping;
	SendMsg(Buffer,8);
	return 0;
}
//---------------------------------------------------------------------------
// ����:	Decipher
// ����:	���ܷ������ѹ��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KNetClient::Decipher(void)
{
	int i,index=0;
	BYTE *pTemp1=RecvData.Data;
	BYTE *pTemp2=Cipher;
    WORD Num=RecvData.dwSize;
    WORD CipherVerify=0,LastCipherVerify=0;
	//cheak cipher
	if(!RecvData.CipherVerify) return;
    for(i=0;i<16;i++)
	{
		CipherVerify+=(WORD)Cipher[i];
		LastCipherVerify+=LastCipher[i];
	}
    if(CipherVerify!=RecvData.CipherVerify) 
	{
		if(LastCipherVerify==RecvData.CipherVerify)
			memcpy(Cipher,LastCipher,16);
		else
		{
			RecvFlag=0;
			return;
		}
	}
	//decipher
    for(i=0;i<(int)Num;i++) 
	{
		*pTemp1^=*(pTemp2+index);
		index=(index+1)%16;
	} 
}

//---------------------------------------------------------------------------
// ����:	GetHostName()
// ����:	
// ����:	void
// ����:	char*
//---------------------------------------------------------------------------
char* KNetClient::GetHostName(void)
{
	return m_ServerHostName;
}
//---------------------------------------------------------------------------
// ����:	GetPortName()
// ����:	
// ����:	void
// ����:	char*
//---------------------------------------------------------------------------
char* KNetClient::GetPortName(void)
{
	return m_ServerPortName;
}
//--------------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����:	ClearRecvFlag
// ����:	��ս������ͽ��ձ�־��ÿѭ����Ϣ����������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KNetClient::ClearRecvFlag(void)
{
  SysCommand.dwNum=0;
  SysCommand.dwCurrentOffset=0;
  CoreCommand.dwNum=0;
  CoreCommand.dwCurrentOffset=0;
  RecvFlag=0;
}



//---------------------------------------------------------------------------
// ����:	DisConnect
// ����:	�Ͽ�����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KNetClient::DisConnect(void)
{
	shutdown(m_Socket, 2);
	closesocket(m_Socket);
	m_Socket = 0;
}



