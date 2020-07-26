//udppeer_basic.inl


NAMESP_BEGIN
namespace net
{

template<class EventQueueT, class MsgWrapper>
void UdpPeerBasic<EventQueueT, MsgWrapper>::open()
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if(_local_addr.ip.empty())
		addr.sin_addr.s_addr = INADDR_ANY;
	else
		inet_pton(AF_INET, _local_addr.ip.c_str(), &(addr.sin_addr));

	addr.sin_port = htons(_local_addr.port);
	_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(_fd == -1 )
		throw sockexcpt("socket");

	if( bind(_fd, (sockaddr*)&addr, sizeof(addr)) == -1)
		throw sockexcpt("bind");
	
	_evt.reset( new InputEvent() );
	_listener.template listen( _fd, _evt.get(), [this](){ this->onInput(); } );
}


template<class EventQueueT, class MsgWrapper>
void UdpPeerBasic<EventQueueT,MsgWrapper>::close(){
	_listener.template unlisten(_fd, _evt.get());
	::close(_fd);
}

template<class EventQueueT, class MsgWrapper>
void UdpPeerBasic<EventQueueT, MsgWrapper>::onInput()
{
	sockaddr_in inaddr;
	socklen_t addrlen = sizeof(inaddr);
	memset(&inaddr, 0, sizeof(inaddr));
	MsgWrapper msg(10240);
	int rlen=-1;
	if( ( rlen=recvfrom(_fd, msg.begin(), msg.maxsize(), 0, (sockaddr *)&inaddr, &addrlen) )>=0 )
	{
		msg.size(rlen);
		AddrPair addr = makeAddrPair(&inaddr);
		if(_onrecv_cb != nullptr){
			_onrecv_cb(addr, msg);
		}	
	}
}

template<class EventQueueT, class MsgWrapper>
void UdpPeerBasic<EventQueueT, MsgWrapper>::sendTo(const AddrPair& remote, const MsgWrapper& msg)
{
	sockaddr_in inaddr;
	socklen_t addrlen = sizeof inaddr;
	makeSockaddr(remote, (sockaddr*)&inaddr, addrlen);
	int ret = ::sendto(_fd, msg.begin(), msg.size(), 0, (sockaddr*)&inaddr, sizeof(inaddr));
	if(ret<0)
		perror("sendto");
}

}//net
NAMESP_END

