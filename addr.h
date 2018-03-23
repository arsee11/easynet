//addr.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2014-12-11
//log	: create 
//****************************

#ifndef ADDR_H
#define ADDR_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <exception>
#include <string>

#if defined(_MSC_VER)
#include <winsock2.h>
#endif

#if defined(__GNUC__)
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <string>


NAMESP_BEGIN
namespace net
{

#if defined(__GNUC__)
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

class sockexcpt:public std::exception
{
public:
	sockexcpt(const char *str)
		:_str(str)
	{}

	const char* what()const throw()
	{
		return _str.c_str();
	}

private:
	std::string _str;
};


struct AddrPair
{
	unsigned short 	port;
	std::string 	ip;
};

inline std::string getip(sockaddr_in* addr){
	char str[16] = {0};
	return inet_ntop(AF_INET, (void*)&(addr->sin_addr), str, 16);
}

inline uint16_t getport(sockaddr_in* addr){
	return ntohs(addr->sin_port);
}

inline bool sockInit(){
#if defined(_MSC_VER)
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (r != NO_ERROR)
	{
       		return false;
	}
#endif	
	return true;
}

inline bool sockUninit(){
#if defined(_MSC_VER)
	return WSACleanup()==0;
#else
	return true;
#endif
}
	
}//net
NAMESP_END

#endif/*ADDR_H*/
