///acceptor_udp.h

#ifndef ACCEPTOR_UDP_H
#define ACCEPTOR_UDP_H

#ifndef FD_DEF_H
#include "fddef.h"
#endif

#ifndef ADDR_H
#include "addr.h"
#endif


#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

#include <unordered_map>
#include <tuple>
#include "netpeer.h"

NAMESP_BEGIN
namespace net
{

class Event;
class NetPeer;
class NetPeerUdp;

class AcceptorUdp 
{
public: 
	typedef fd_t netpeer_key_t;

public:
	AcceptorUdp(const AddrPair& local_addr, bool isopen=true)throw(sockexcpt)
		:_local_addr(local_addr)
	{
		if(isopen)
			open();
	}

	AcceptorUdp(unsigned short local_port, bool isopen=true)throw(sockexcpt){
		_local_addr.port = local_port;
		if(isopen)
			open();
	}

	~AcceptorUdp(){
		close();
	}

	void open()throw(sockexcpt);

	void close(){
		if(_isopened)
			::close(_fd);
	}

	std::tuple<Event*, NetPeer*> accept();

	fd_t fd(){ return _fd; }

	netpeer_ptr_t getPeer(netpeer_key_t key){ return nullptr; }

private:
	NetPeerUdp* findPeer(const AddrPair& addr);
	void addPeer(const AddrPair& addr, NetPeerUdp* peer);

private:
	fd_t _fd;
	AddrPair _local_addr;
	bool _isopened=false;
	std::unordered_map<AddrPair, NetPeerUdp*, addr_hash_value> _netpeer_set;
	static constexpr int MAX_BUF_LEN = 1400*10;
	char _recvbuf[MAX_BUF_LEN];
};

}//net
NAMESP_END

#endif /*ACCEPTOR_UDP_H*/
