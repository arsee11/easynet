//addr.h
//copyright	: Copyright (c) 2014 arsee.
//author	: arsee

//****************************
//modify	
//data	: 2014-12-11
//log	: create 
//****************************

#ifndef ADDR_H
#define ADDR_H

#ifndef NAMESPDEF_H
#include "namespdef.h"
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

#include <string.h>


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

inline bool operator==(const AddrPair& rhs, const AddrPair& lhs){
	return (rhs.port==lhs.port && rhs.ip==lhs.ip);
}


struct addr_hash_value
{
	size_t operator()(const AddrPair& addr)const{
		size_t h1 = std::hash<unsigned short>()(addr.port);
		size_t h2 = std::hash<std::string>()(addr.ip);
		return h1 ^ (h2<<1);
	}
};

inline std::string getip(const sockaddr_in* addr){
	char str[16] = {0};
	return inet_ntop(AF_INET, (void*)&(addr->sin_addr), str, 16);
}

inline uint16_t getport(const sockaddr_in* addr){
	return ntohs(addr->sin_port);
}

inline AddrPair makeAddrPair(const sockaddr_in* inaddr)
{
	return AddrPair{getport(inaddr), getip(inaddr)};
}

inline void makeSockaddr(const AddrPair& addr, sockaddr* saddr, socklen_t& nsaddr)
{
	sockaddr_in inaddr;
	::memset(&inaddr, 0, sizeof(inaddr));
	inaddr.sin_family = AF_INET;
	if(addr.ip.empty())
		inaddr.sin_addr.s_addr = INADDR_ANY;
	else
		::inet_pton(AF_INET, addr.ip.c_str(), &(inaddr.sin_addr));

	inaddr.sin_port = htons(addr.port);

	nsaddr = sizeof inaddr;
	::memcpy(saddr, &inaddr, nsaddr);
}

}//net
NAMESP_END

#endif/*ADDR_H*/
