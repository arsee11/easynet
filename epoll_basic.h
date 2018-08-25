//epoll_basic.h

#ifndef EPOLL_BASIC_H	
#define EPOLL_BASIC_H

#include <sys/epoll.h>
#include <fcntl.h>
#include <exception>
#include <unordered_map>

#include "fddef.h"
#include "namespdef.h"
#include "event_queue.h"


NAMESP_BEGIN
namespace net
{


template<class EventQueue, typename FD>
class EpollBasic
{
	static constexpr int _max = 2048;

	using InputListener = std::function<void(fd_t)>;
	using OutputListener = InputListener;
	using CloseListener = InputListener;
	
	using InputListenerMap = std::unordered_map<fd_t, InputListener>; 
	using OutputListenerMap = std::unordered_map<fd_t, OutputListener>; 
	using CloseListenerMap = std::unordered_map<fd_t, CloseListener>; 

public:
	EpollBasic(EventQueue* q):_evt_queue(q){
		init();
	}

	bool attach(fd_t fd);
	void detach(fd_t fd){
		if( epoll_ctl(_efd, EPOLL_CTL_DEL, fd, NULL) == -1)
			perror("epoll_ctl");
	}

	void listenInput(const InputListener& l, fd_t fd){
		_input_listeners.insert(std::make_pair(fd, l) );
		attach(fd);
	}

	void listenOutput(const OutputListener& l, fd_t fd){
		_output_listeners.insert(std::make_pair(fd, l) );
		attach(fd);
	}

	void listenClose(const CloseListener& l, fd_t fd){
		_close_listeners.insert(std::make_pair(fd, l) );
		attach(fd);
	}

	void select();

	void postSend(fd_t fd){
		epoll_event tmp;
		tmp.data.fd = fd;
		tmp.events = EPOLLET|EPOLLOUT;
		if( epoll_ctl(_efd, EPOLL_CTL_MOD, fd, &tmp)==-1 ) 
			perror("epoll_ctl");
	}

	void postRecv(fd_t fd){
		epoll_event tmp;
		tmp.data.fd = fd;
		tmp.events = EPOLLET|EPOLLIN;
		if( epoll_ctl(_efd, EPOLL_CTL_MOD, fd, &tmp)==-1 ) 
			perror("epoll_ctl");
	}

private:
	void init(){
		_efd = epoll_create(_max);
		if( _efd == -1 )
		{
			perror("epoll_create");
			throw std::exception();
		}
	}

private:
	fd_t _efd;
	InputListenerMap _input_listeners; 
	OutputListenerMap _output_listeners; 
	CloseListenerMap _close_listeners; 
	EventQueue* _evt_queue;
};

}//namespace net
NAMESP_END

#endif /*EPOLL_BASIC_H*/
