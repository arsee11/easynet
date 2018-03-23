//acceptor.h
//copyright	:Copyright (c) 2014 arsee.
//license	:GNU GPL V2.

#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#ifndef FD_DEF_H
#include "fddef.h"
#endif

#ifndef ADDR_H
#include "addr.h"
#endif

#include "netpeer.h"

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif


NAMESP_BEGIN
namespace net
{

class Acceptor 
{
public:
	Acceptor(const AddrPair& local_addr, bool isopen=true)throw(sockexcpt)
		:_local_addr(local_addr)
	{
		if(isopen)
			open();
	}

	Acceptor(unsigned short port, bool isopen=true)throw(sockexcpt){
		_local_addr.port = port;
		if(isopen)
			open();
	}

	~Acceptor(){
		close();
	}

	void open()throw(sockexcpt);

	void close(){
		if(_isopened)
			::close(_fd);
	}

	NetPeer* accept();

	fd_t fd(){ return _fd; }

private:
	fd_t _fd;
	AddrPair _local_addr;
	bool _isopened=false;
};

}//net
NAMESP_END

#endif /*ACCEPTOR_H*/
