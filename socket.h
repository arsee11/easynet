///socket.h

#ifndef SOCKET_H
#define SOCKET_H

#include "addr.h"
#include "fddef.h"

NAMESP_BEGIN
namespace net
{

template<class IPv>
class Socket
{
public:
    Socket()
        :Socket(AddrPair{"", 0})
    {}

    Socket(const AddrPair& addr, int type, int protocol)
        :_local_addr(addr)
    {
        IPv::check_ip(addr.ip);
	    _fd = socket(IPv::family, type, protocol);
	    if(_fd == INVALID_SOCKET){
		    perror("sock");
		    return ;
	    }
	
        typename IPv::addr_t saddr;
        int addr_size = IPv::makeSockaddr(addr, &saddr);
            if (bind(_fd, (sockaddr*)&saddr, (socklen_t)addr_size) == SOCKET_ERROR){
		    perror("bind");
            ::close(_fd);
            _fd = INVALID_SOCKET;
	    }
    }

    Socket(int fd, const AddrPair& local_addr, const AddrPair& remote_addr)
        :_fd(fd)
        ,_local_addr(local_addr)
        ,_remote_addr(remote_addr)
    {
    }

    void close(){
        ::close(_fd);
        _fd = INVALID_SOCKET;
    }
    
    bool invalid(){ return _fd==INVALID_SOCKET; }
    fd_t fd()const{ return _fd; }
    AddrPair remote_addr()const{ return _remote_addr; }

protected:
    fd_t _fd=INVALID_SOCKET;
    AddrPair _local_addr;
    AddrPair _remote_addr;
    
};



//////////////////////////////////////////////////////////////////////
template<class IPv>
class TcpSocket : public Socket<IPv>
{
public:
    TcpSocket()
        :TcpSocket(AddrPair{"", 0})
    {}

    TcpSocket(const AddrPair& addr)
        :Socket<IPv>(addr, SOCK_STREAM, IPPROTO_TCP)
    {
    }

    TcpSocket(int fd, const AddrPair& local_addr, const AddrPair& remote_addr)
        :Socket<IPv>(fd, local_addr, remote_addr)
    {
    }

    bool connect(const AddrPair& remote_addr){
        typename IPv::addr_t saddr;
        int addr_size = IPv::makeSockaddr(remote_addr, &saddr);
	    if (::connect(this->_fd, (sockaddr *)&saddr, (socklen_t)addr_size) == SOCKET_ERROR)
	    {
		    perror("connect");
		    return false;
	    }

        return true;
    }

    bool listen(){
		int r = ::listen(this->_fd, 5);
        if(r == SOCKET_ERROR){
		    perror("listen");
            return false;
        }

        return true;
    }

    TcpSocket accept(){
        typename IPv::addr_t addr;
		socklen_t len = sizeof(addr);	
		fd_t newfd = ::accept(this->_fd, (sockaddr*)&addr, &len);
        AddrPair remote_addr = IPv::makeAddrPair(&addr);
        return TcpSocket(newfd, this->_local_addr, this->_remote_addr);
    }

    int send(const void* buf, int size){
        return ::send(this->_fd, buf, size, 0);
    }

    int recv(void *buf, int len){
	    return ::recv(this->_fd, buf, len, 0);
    }
    
};



/////////////////////////////////////////////////////////////////////////////////
template<class IPv>
class UdpSocket : public Socket<IPv>
{
public:
    UdpSocket()
        :UdpSocket(AddrPair{"", 0})
    {}

    UdpSocket(const AddrPair& addr)
        :Socket<IPv>(addr, SOCK_DGRAM, 0)
    {
    }

    UdpSocket(int fd, const AddrPair& local_addr, const AddrPair& remote_addr)
        :Socket<IPv>(fd, local_addr, remote_addr)
    {
    }

    int recvfrom(void* buf, int len, AddrPair& addr){
        typename IPv::addr_t inaddr;
	    socklen_t addrlen = sizeof(inaddr);
	    memset(&inaddr, 0, sizeof(inaddr));
	    int rlen = ::recvfrom(this->_fd, buf, len, 0, (sockaddr*)&inaddr, &addrlen);
		addr = IPv::makeAddrPair(&inaddr);
        return rlen;
    }

    int sendto(const void* buf, int len, const AddrPair& addr){
        typename IPv::addr_t inaddr;
	    socklen_t addrlen = sizeof inaddr;
        IPv::makeSockaddr(addr, &inaddr);
	    int ret = ::sendto(this->_fd, buf, len, 0, (sockaddr*)&inaddr, sizeof(inaddr));
        return ret;
    }
};

}//net
NAMESP_END

#endif /*SOCKET_H*/
