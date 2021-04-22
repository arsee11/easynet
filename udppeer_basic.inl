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
    std::cout<<"onInput\n";
    AddrPair addr;
	MsgWrapper msg(10240);
	int rlen=-1;
	if( ( rlen=_socket.recvfrom(msg.rd_ptr(), msg.capicity(), addr) )>=0 )
	{
		msg.size(rlen);
		if(_onrecv_cb != nullptr){
			_onrecv_cb(addr, msg);
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

