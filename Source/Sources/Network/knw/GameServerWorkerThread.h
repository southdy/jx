/********************************************************************
	created:	2003/03/04
	file base:	GameServerWorkerThread
	file ext:	h
	author:		liupeng
	
	purpose:	
*********************************************************************/
#ifndef __INCLUDE_ONLINEGAME_GAMESERVERWORKERTHREAD_H__
#define __INCLUDE_ONLINEGAME_GAMESERVERWORKERTHREAD_H__

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
	#pragma once
#endif

#include "IOCPServer\SocketServer.h"

/*
 * Classes defined in other files...
 */
namespace OnlineGameLib {
namespace Win32 {

//[ Include in .\..\IPCPServer
class CIOBuffer;
//]

} // End of namespace OnlineGameLib
} // End of namespace Win32


class CGameServerWorkerThread : public OnlineGameLib::Win32::CSocketServer::WorkerThread
{
public:

	explicit CGameServerWorkerThread( OnlineGameLib::Win32::CIOCompletionPort &iocp );
	
	virtual ~CGameServerWorkerThread();
	
private:
	
	virtual void ReadCompleted(
				OnlineGameLib::Win32::CSocketServer::Socket *pSocket,
				OnlineGameLib::Win32::CIOBuffer *pBuffer);

	virtual void OnError( const OnlineGameLib::Win32::_tstring &message );
	
	/*
	 * No copies do not implement
	 */
	CGameServerWorkerThread( const CGameServerWorkerThread &rhs );
	CGameServerWorkerThread &operator=( const CGameServerWorkerThread &rhs ); 
};

#endif //__INCLUDE_ONLINEGAME_GAMESERVERWORKERTHREAD_H__