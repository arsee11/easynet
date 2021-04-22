///netpeer.inl
//

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
	
NAMESP_BEGIN
namespace net
{

template<class Socket, class EventQueueT, class MsgWrapper>
int NetPeerBasic<Socket, EventQueueT,  MsgWrapper>::write(const MsgWrapper& msg)
{
    return _socket.send(msg.rd_ptr(), msg.size());
}

template<class Socket, class EventQueueT, class MsgWrapper>
void NetPeerBasic<Socket, EventQueueT, MsgWrapper>::write_async(MsgWrapper& buf, SendCb call_back)
{
   do_write(buf, call_back);
}

template<class Socket, class EventQueueT, class MsgWrapper>
void NetPeerBasic<Socket, EventQueueT,  MsgWrapper>::close()
{
    if(!_socket.invalid()){
        _listener.unlisten(_socket.fd());
        _socket.close();
    }
}

template<class Socket, class EventQueueT, class MsgWrapper>
void NetPeerBasic<Socket, EventQueueT,  MsgWrapper>::onInput()
{
    //read until all availabe datas have recv
    while(true){
        MsgWrapper msg(10240);
        int rsize=_socket.recv(msg.wr_ptr(), msg.capicity());
        msg.size(rsize);

        if(rsize > 0 ){
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
                //std::cout<<"read failed fd="<<this->fd()<<std::endl;
                //this->onClose();
            }
            break;
        }
    }
}

template<class Socket, class EventQueueT, class MsgWrapper>
void NetPeerBasic<Socket, EventQueueT,  MsgWrapper>::onClose()
{
    std::cout<<"onClose fd="<<this->fd()<<std::endl;
    close();
    if(_close_cb != nullptr){
        _close_cb(this->shared_from_this());
    }
}

template<class Socket, class EventQueueT, class MsgWrapper>
int NetPeerBasic<Socket, EventQueueT,  MsgWrapper>::do_write(MsgWrapper& buf, SendCb call_back)
{
    int ret = _socket.send(buf.rd_ptr(), buf.size());
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

template<class Socket, class EventQueueT, class MsgWrapper>
void NetPeerBasic<Socket, EventQueueT,  MsgWrapper>:: onOutput(MsgWrapper buf, SendCb call_back){
    int ret = do_write(buf, call_back);
    if(ret == buf.size()){
        _listener.unlisten(this->fd(), _output_evt.get());
    }
}

}//net
NAMESP_END
