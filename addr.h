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
	std::string 	ip;
	unsigned short 	port;
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




////////////////////////////////////////////////////////////////////////////////////
struct IPv4
{
    using addr_t = sockaddr_in;

    static constexpr int family=AF_INET;
    static bool check_ip(const std::string& ip){ return true; }

static int makeSockaddr(const AddrPair& addr, addr_t* inaddr)
{
	::memset(inaddr, 0, sizeof(addr_t));
	inaddr->sin_family = family;
	if(addr.ip.empty())
		inaddr->sin_addr.s_addr = INADDR_ANY;
	else
		::inet_pton(family, addr.ip.c_str(), &(inaddr->sin_addr));

	inaddr->sin_port = htons(addr.port);

	int nsaddr = sizeof(addr_t);
    return nsaddr;
}
    
static AddrPair makeAddrPair(const addr_t* inaddr)
{
	char str[16] = {0};
	inet_ntop(family, (void*)&(inaddr->sin_addr), str, 16);
	uint16_t port = ntohs(inaddr->sin_port);
	return AddrPair{str, port};
}

};



////////////////////////////////////////////////////////////////////////////////////
struct IPv6
{
    using addr_t = sockaddr_in6;

    static constexpr int family=AF_INET6;
    static bool check_ip(const std::string& ip){ return true; }

static int makeSockaddr(const AddrPair& addr, addr_t* inaddr)
{
	::memset(inaddr, 0, sizeof(addr_t));
	inaddr->sin6_family = family;
	if(addr.ip.empty())
		inaddr->sin6_addr = in6addr_any;
	else
		::inet_pton(family, addr.ip.c_str(), &(inaddr->sin6_addr));

	inaddr->sin6_port = htons(addr.port);

	int nsaddr = sizeof(addr_t);
    return nsaddr;
}
    
static AddrPair makeAddrPair(const addr_t* inaddr)
{
	char str[64] = {0};
	inet_ntop(family, (void*)&(inaddr->sin6_addr), str, 64);
	uint16_t port = ntohs(inaddr->sin6_port);
	return AddrPair{str, port};
}

};

}//net
NAMESP_END

#endif/*ADDR_H*/
