///connector.cpp
//
//

#include "connector.h"
#include <string.h>

NAMESP_BEGIN
namespace net
{

NetPeer* TCPConnector::connect(const AddrPair& remot_addr)
{
	fd_t s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s ==INVALID_SOCKET)
	{
		perror("sock");
		return nullptr;
	}
	
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_local_addr.port);
	if( _local_addr.ip.empty() )
		addr.sin_addr.s_addr = INADDR_ANY;
	else
	{
		if( inet_pton(AF_INET, _local_addr.ip.c_str(), &(addr.sin_addr) ) < 0 )
		{
			perror("inet_pton");
			return nullptr;
		}
	}	

	if (bind(s, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		perror("bind");
		return nullptr;
	}
	
	sockaddr_in raddr;
	memset(&raddr, 0, sizeof(raddr));
	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(remot_addr.port);
	inet_pton(AF_INET, remot_addr.ip.c_str(), &(raddr.sin_addr));
	if (::connect(s, (sockaddr *)&raddr, sizeof(raddr)) == SOCKET_ERROR)
	{
		perror("connect");
		return nullptr;
	}

	return new NetPeerImpl(s, AddrPair{getport(&raddr), getip(&raddr)} );
}

}//net
NAMESP_END
