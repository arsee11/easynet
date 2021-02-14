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

template<class Socket, class Event, class AcceptCb>
class AcceptorBasic 
{ 
public:
	AcceptorBasic(const AddrPair& local_addr)
        :_socket(local_addr)
	{
        open();
	}

	AcceptorBasic(unsigned short port)
        :AcceptorBasic(AddrPair{"",port})      
	{
	}

	virtual ~AcceptorBasic(){
	}

	void listenOnAccept(const AcceptCb& cb){ _accept_cb = cb; }

	fd_t fd()const{ return _socket.fd(); }

private:
	void open(){
        _socket.listen();
	}

protected:
	using event_t = Event;
	std::unique_ptr<event_t> _evt;
	AcceptCb _accept_cb;
    Socket _socket;
};

}//net
NAMESP_END

#endif /*ACCEPTOR_BASIC_H*/
