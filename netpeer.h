///netpeer.h
//

#ifndef NET_PEER_H
#define NET_PEER_H

#include "fddef.h"
#include "addr.h"
#include <memory>
#include <string.h>
#include <unordered_map>
#include <sys/socket.h>

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

class NetPeer 
{
public:
	NetPeer()=delete;

	NetPeer(fd_t fd, const AddrPair& remote_addr)
		:_fd(fd)
		,_remote_addr(remote_addr)
	{}
	
	~NetPeer(){
	}
	
	///Wreturn num of recv bytes, or <0 on failed.
	virtual int read(char *buf, int len){ throw sockexcpt("don't call it!"); }
	virtual int write(const char *buf, int len);

	AddrPair remote_addr(){ return _remote_addr; }
	
	virtual void close();
	
	fd_t fd()const{ return _fd; }

protected:
	AddrPair _remote_addr;
	fd_t _fd;
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


//////////////////////////////////////////////////////////////
class NetPeerUdp : public NetPeer
{
public:
	NetPeerUdp(fd_t fd, const AddrPair& remote_addr)
		:NetPeer(fd, remote_addr)
	{
		memset(&_sockaddr, 0, sizeof(_sockaddr));
		_sockaddr.sin_family = AF_INET;
		inet_pton(AF_INET, _remote_addr.ip.c_str(), &(_sockaddr.sin_addr));
		_sockaddr.sin_port = htons(_remote_addr.port);
	}

	int read(char* buf, int len)override{ return 0; }
	void close()override{};
	int write(const char *buf, int len)override;

private:
	sockaddr_in _sockaddr;
	
};

typedef std::shared_ptr<NetPeer> netpeer_ptr_t;
typedef std::unordered_map<fd_t, netpeer_ptr_t> netpeer_manager_t; 
}//net
NAMESP_END

#endif /*NET_PEER_H*/
