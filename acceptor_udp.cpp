//acceptor_udp.cpp

#include "acceptor_udp.h"
#include <string.h>
#include "netevents.h"

NAMESP_BEGIN
namespace net
{

void AcceptorUdp::open()throw(sockexcpt)
{
	if(_isopened)
		throw sockexcpt("aleady opened");

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if(_local_addr.ip.empty())
		addr.sin_addr.s_addr = INADDR_ANY;
	else
		inet_pton(AF_INET, _local_addr.ip.c_str(), &(addr.sin_addr));

	addr.sin_port = htons(_local_addr.port);
	_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(_fd == -1 )
		throw sockexcpt("socket");

	if( bind(_fd, (sockaddr*)&addr, sizeof(addr)) == -1)
		throw sockexcpt("bind");

	_isopened = true;
}


std::tuple<Event*, NetPeer*> AcceptorUdp::accept()
{
	AddrPair addr;
	sockaddr_in inaddr;
	socklen_t addrlen = sizeof(inaddr);
	memset(&inaddr, 0, sizeof(inaddr));
	int rlen=-1;
	if( ( rlen=recvfrom(_fd, _recvbuf, MAX_BUF_LEN, 0, (sockaddr *)&inaddr, &addrlen) )>=0 )
	{
		addr.ip = inet_ntoa(inaddr.sin_addr);
		addr.port = ntohs(inaddr.sin_port);
	}

	NetPeerUdp* peer=nullptr; 
	if( (peer=findPeer(addr))==nullptr )
	{
		peer = new NetPeerUdp(_fd, addr);
		addPeer(addr, peer);
	}
	NetInputEvent* e = new NetInputEvent(peer); 
	e->extra(_recvbuf, rlen);
	return std::make_tuple(e, nullptr);
}


NetPeerUdp* AcceptorUdp::findPeer(const AddrPair& addr)
{
	auto i =  _netpeer_set.find(addr);
	if (i != _netpeer_set.end())
		return i->second;

	return nullptr;
}

void AcceptorUdp::addPeer(const AddrPair& addr, NetPeerUdp* peer)
{
	_netpeer_set.insert(std::make_pair(addr, peer));
}

}//net
NAMESP_END

