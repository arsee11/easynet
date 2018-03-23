//file:tcpsock.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2015-3-17
//log	: record 
//****************************

namespace net
{
////////////////////////////////
//Socket Option operations.
//@Sock the socket class.
struct SockOpt
{
	bool reuseAddr()
	{
		sock.sock();
	}

	SOCKET _sock;
};
}//
