///netpeer.cpp
//

#include "netpeer.h"	

#include <sys/types.h>
#include <sys/socket.h>
	
NAMESP_BEGIN
namespace net
{
int NetPeer::write(const char *buf, int len)
{
	int n=0;
	do
	{
		int m = send(_fd, buf+n, len-n, 0);
		if(m < 0 )
			break;

		n += m;
	}while( n < len );

	return n;
}

void NetPeer::close()
{
	if(_fd != INVALID_SOCKET)
	{
		::close(_fd);
		_fd = INVALID_SOCKET;
	}
}



//////////////////////////////////////////////////////
int NetPeerImpl::read(char* buf, int len)
{
	return recv(_fd, buf, len, 0);
}

}//net
NAMESP_END
