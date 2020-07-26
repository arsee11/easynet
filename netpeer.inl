///netpeer.inl
//

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
	
NAMESP_BEGIN
namespace net
{

template<class EventQueueT, class MsgWrapper>
void NetPeerBasic<EventQueueT,  MsgWrapper>::onInput()
{
	while(true)
	{
		MsgWrapper msg(10240);
		int rsize=read(msg.begin(), msg.maxsize());
		msg.size(rsize);

		if(rsize > 0 ){
			std::cout<<"read size="<<rsize<<std::endl;
			//todo notify completed data
		}
		else{
			if(errno != EAGAIN && errno != EWOULDBLOCK){
				std::cout<<"read failed fd="<<this->fd()<<std::endl;
				this->onClose();
			}
			break;
		}
	}
	
}

template<class EventQueueT, class MsgWrapper>
void NetPeerBasic<EventQueueT,  MsgWrapper>::onClose()
{
	close();
	//todo notify sock have closed
}

template<class EventQueueT, class MsgWrapper>
int NetPeerBasic<EventQueueT,  MsgWrapper>::write(const MsgWrapper& msg)
{
	return write(msg.begin(), msg.size());
}

template<class EventQueueT, class MsgWrapper>
int NetPeerBasic<EventQueueT,  MsgWrapper>::write(const void *buf, size_t len)
{
	size_t n=0;
	do{
		int m = send(this->fd(), (const char*)buf+n, len-n, 0);
		if(m <= 0){
			if(errno == EAGAIN || errno == EWOULDBLOCK ){
				//todo post wait for output event alive.
			}
			else{
				std::cout<<"write data error fd="<<this->fd()<<std::endl;
				break;
			}
		}
		else{
			n += m;
		}

	}while( n < len );

	return n;
}

template<class EventQueueT,  class MsgWrapper>
void NetPeerBasic<EventQueueT,  MsgWrapper>::close()
{
	if(_fd != INVALID_SOCKET){
		::close(_fd);
	}
}

template<class EventQueueT,  class MsgWrapper>
int NetPeerBasic<EventQueueT,  MsgWrapper>::read(void *buf, int len)
{
	return ::recv(_fd, buf, len, 0);
}

}//net
NAMESP_END
