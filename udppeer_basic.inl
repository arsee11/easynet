//udppeer_basic.inl

#include <iostream>

NAMESP_BEGIN
namespace net
{

template<class Socket, class EventQueueT, class MsgWrapper>
void UdpPeerBasic<Socket, EventQueueT, MsgWrapper>::open()
{
    _socket.bind(); 
    _evt.reset( new InputEvent([this](){this->onInput();}) );
	this->template listen( _socket.fd(), _evt.get());
}


template<class Socket, class EventQueueT, class MsgWrapper>
void UdpPeerBasic<Socket, EventQueueT,MsgWrapper>::close(){
	this->template unlisten(_socket.fd(), _evt.get());
	_socket.close();
}

template<class Socket, class EventQueueT, class MsgWrapper>
void UdpPeerBasic<Socket, EventQueueT, MsgWrapper>::onInput()
{
    while(true){
        AddrPair addr;
	    MsgWrapper msg(10240);
	    int rlen=-1;
	    rlen=_socket.recvfrom(msg.rd_ptr(), msg.capicity(), addr);
		msg.size(rlen);
        if(rlen > 0 ){
		    if(_onrecv_cb != nullptr){
			    _onrecv_cb(addr, msg);
		    }	
        }
        else{ // no datas to read
            if(errno != EAGAIN && errno != EWOULDBLOCK){
                //std::cout<<"read failed error ocussed, fd="<<this->fd()<<std::endl;
				//_onerror_cb(errno);
            }
            break;
        }
    }
}

template<class Socket, class EventQueueT, class MsgWrapper>
void UdpPeerBasic<Socket, EventQueueT, MsgWrapper>::sendTo(const AddrPair& remote, const MsgWrapper& msg)
{
	int ret = _socket.sendto(msg.rd_ptr(), msg.size(), remote);
	if(ret<0)
		perror("sendto");
}

}//net
NAMESP_END

