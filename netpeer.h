///netpeer.h
//

#ifndef NET_PEER_H
#define NET_PEER_H

#include "fddef.h"
#include "addr.h"
#include "event.h"
#include <memory>
#include <unordered_map>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

class NetPeer : public EventSender
{
public:
	NetPeer()=delete;

	NetPeer(fd_t fd, const AddrPair& remote_addr)
		:EventSender(fd)
		,_remote_addr(remote_addr)
	{}
	
	~NetPeer(){
	}
	
	///Wreturn num of recv bytes, or <0 on failed.
	virtual int read(char *buf, int len){ throw sockexcpt("don't call it!"); }
	int write(const char *buf, int len);

	AddrPair remote_addr(){ return _remote_addr; }
	
	void close();

protected:
	AddrPair _remote_addr;
};



//////////////////////////////////////////////////////////////
class NetPeerImpl : public NetPeer
{
public:
	NetPeerImpl(fd_t fd, const AddrPair& remote_addr)
		:NetPeer(fd, remote_addr)
	{}

	int read(char* buf, int len)override;
};


typedef std::shared_ptr<NetPeer> netpeer_ptr_t;
typedef std::unordered_map<fd_t, netpeer_ptr_t> netpeer_manager_t;

}//net
NAMESP_END

#endif /*NET_PEER_H*/
