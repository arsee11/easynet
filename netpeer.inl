///netpeer.inl
//

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
	
NAMESP_BEGIN
namespace net
{

template<class EventQueueT, class MsgWrapper>
int NetPeerBasic<EventQueueT,  MsgWrapper>::write(const MsgWrapper& msg)
{
	return write(msg.begin(), msg.size());
}

template<class EventQueueT, class MsgWrapper>
void NetPeerBasic<EventQueueT, MsgWrapper>::write_async(MsgWrapper& buf, SendCb call_back)
{
   do_write(buf, call_back);
}

template<class EventQueueT,  class MsgWrapper>
int NetPeerBasic<EventQueueT,  MsgWrapper>::read(void *buf, int len)
{
	return ::recv(_fd, buf, len, 0);
}

template<class EventQueueT,  class MsgWrapper>
void NetPeerBasic<EventQueueT,  MsgWrapper>::close()
{
	if(_fd != INVALID_SOCKET){
        _listener.unlisten(_fd);
		::close(_fd);
		_fd = INVALID_SOCKET;
	}
}

template<class EventQueueT, class MsgWrapper>
void NetPeerBasic<EventQueueT,  MsgWrapper>::onInput()
{
    //read until all availabe datas have recv
    while(true){
        MsgWrapper msg(10240);
        int rsize=read(msg.wr_ptr(), msg.capicity());
        msg.size(rsize);

        if(rsize > 0 ){
        	std::cout<<"read size="<<rsize<<std::endl;
        	if(_recv_cb != nullptr){
        	    _recv_cb(this->shared_from_this(), msg);
        	}
        	if(rsize < msg.capicity()) //all availabel datas have recv
        	{
        	    break;
        	}
        }
        else{ // no datas to read
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
   	std::cout<<"onClose fd="<<this->fd()<<std::endl;
	close();
    //todo notify sock have closed
}

template<class EventQueueT, class MsgWrapper>
int NetPeerBasic<EventQueueT,  MsgWrapper>::do_write(MsgWrapper& buf, SendCb call_back)
{
    int ret = ::send(this->fd(), buf.rd_ptr(), buf.size(), 0);
    if(ret <=0 && errno != EAGAIN && errno != EWOULDBLOCK ){
        call_back( 0, ret);
        return 0;
    }
    if(ret < buf.size()){
        buf.consume(ret);
        _output_evt->setHandler( [this, buf, call_back]{
            this->onOutput(buf, call_back);
        });
        _listener.listen(this->fd(), _output_evt.get());
    }

    call_back(ret, 0);
    return ret;
}

template<class EventQueueT, class MsgWrapper>
void NetPeerBasic<EventQueueT,  MsgWrapper>:: onOutput(MsgWrapper buf, SendCb call_back){
    int ret = do_write(buf, call_back);
    if(ret == buf.size()){
        _listener.unlisten(this->fd(), _output_evt.get());
    }
}

}//net
NAMESP_END
