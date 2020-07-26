//acceptor_basic.h

#ifndef ACCEPTOR_BASIC_H
#define ACCEPTOR_BASIC_H

#include "addr.h"
#include "fddef.h"
#include "namespdef.h"

#include <memory>

NAMESP_BEGIN
namespace net
{



template<class Event, class AcceptCb>
class AcceptorBasic 
{ 
public:
	AcceptorBasic(const AddrPair& local_addr)
		:_local_addr(local_addr)
	{
		open();
	}

	AcceptorBasic(unsigned short port)
	{
		_local_addr.port = port;
		open();
	}

	virtual ~AcceptorBasic(){
		close();
	}

	void listenOnAccept(const AcceptCb& cb){ _accept_cb = cb; }

	fd_t fd(){ return _fd; }

private:
	void close(){
		::close(_fd);
	}
	
	void open()
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
	using event_t = Event;
	std::unique_ptr<event_t> _evt;
	AcceptCb _accept_cb;
};

}//net
NAMESP_END

#endif /*ACCEPTOR_BASIC_H*/
