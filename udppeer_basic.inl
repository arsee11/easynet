//udppeer_basic.inl


NAMESP_BEGIN
namespace net
{

template<class EventQueueT, class MsgWrapper>
void UdpPeerBasic<EventQueueT, MsgWrapper>::open()throw(sockexcpt)
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
	
	_evt_queue->attach(_fd);
	_listener.template listen<PollInEvent>( _fd, [this](fd_t fd){ this->onInput(fd); } );
}


template<class EventQueueT, class MsgWrapper>
void UdpPeerBasic<EventQueueT,MsgWrapper>::close(){
	_listener.template unlisten<PollInEvent>(_fd);
	_evt_queue->detach(_fd);
	::close(_fd);
}

template<class EventQueueT, class MsgWrapper>
void UdpPeerBasic<EventQueueT, MsgWrapper>::onInput(fd_t fd)
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
		const listener_map& lmap= _evt_queue->findListenerMap(
			std::type_index(typeid(InputEvent))
		);

		if(lmap.size() > 0 )
		{
			auto l=lmap.find(fd);
			if( l!= lmap.end())
				_evt_queue->pushEvent( event_ptr(
					new InputEvent(l->second, addr, msg ) ) );
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

