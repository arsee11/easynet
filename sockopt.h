//sockopt.h

#include "fddef.h"
#include "sys/types.h"
#include "sys/socket.h"

NAMESP_BEGIN
namespace net
{

struct ReuseAddrOpt
{
	bool apply(fd_t fd)
	{
		if(fd < 0) 
			return false;

		if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
			perror("setsockopt");
			return false;
		}

		return true;
	}
	
	int val=1;
};

}//net
NAMESP_END
