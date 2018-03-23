//acceptor.cpp

#include "acceptor.h"
#include <string.h>

NAMESP_BEGIN
namespace net
{

void Acceptor::open()throw(sockexcpt)
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
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_fd == -1 )
		throw sockexcpt("socket");

	if( bind(_fd, (sockaddr*)&addr, sizeof(addr)) == -1)
		throw sockexcpt("bind");

	listen(_fd, 5);
	_isopened = true;
}


NetPeer* Acceptor::accept()
{
	sockaddr_in addr;
	socklen_t alen = sizeof(addr);
	fd_t nfd = ::accept(_fd, (sockaddr*)&addr, &alen);
	if(nfd < 0 )
	{
		perror("accept");
		return nullptr;
	}

	return  new NetPeerImpl(nfd, AddrPair{getport(&addr), getip(&addr)} );
}


}//net
NAMESP_END

