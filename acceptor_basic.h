//acceptor_basic.h

#ifndef ACCEPTOR_BASIC_H
#define ACCEPTOR_BASIC_H

#include "addr.h"
#include "fddef.h"
#include "namespdef.h"

#include <functional>
#include <typeindex>

NAMESP_BEGIN
namespace net
{



template<class EventQueueT>
class AcceptorBasic 
{ 
public:
	AcceptorBasic(EventQueueT* q, const AddrPair& local_addr)throw(sockexcpt)
		:_local_addr(local_addr)
		,_evt_queue(q)
	{
	}

	AcceptorBasic(EventQueueT* q, unsigned short port)throw(sockexcpt)
		:_evt_queue(q)
	{
		_local_addr.port = port;
	}

	virtual ~AcceptorBasic(){
		close();
	}

	void close(){
		::close(_fd);
	}
	
	fd_t fd(){ return _fd; }

	void open()throw(sockexcpt)
	{
		_fd = ::socket(AF_INET, SOCK_STREAM, 0);
		if(_fd == -1 )
			throw sockexcpt("socket");

		int var=1;
		setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &var, sizeof(int)); 
		sockaddr addr;
		socklen_t naddr=0;
		makeSockaddr(_local_addr, &addr, naddr);
		if( ::bind(_fd, &addr, naddr) == -1)
			throw sockexcpt("bind");
	
		::listen(_fd, 5);
	}

private:
	fd_t _fd;
	AddrPair _local_addr;

protected:
	EventQueueT* _evt_queue;
};

}//net
NAMESP_END

#endif /*ACCEPTOR_BASIC_H*/
