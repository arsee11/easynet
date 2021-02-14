///event_queue.inl
//
//

NAMESP_BEGIN
namespace net
{

template<class Poller>
template<class Event>
void EventQueue<Poller>::bind(fd_t fd, Event* e)
{
	_poller.addPollee(fd, e);
}

template<class Poller>
template<class Event>
void EventQueue<Poller>::unbind(fd_t fd, Event* e)
{
	_poller.delPollee(fd, e);
}

template<class Poller>
void EventQueue<Poller>::unbind(fd_t fd)
{
	_poller.delPollee(fd);
}

template<class Poller>
template<class Callback>
void EventQueue<Poller>::bindInput(fd_t fd, Callback cb)
{
    auto e = new NetInputEvent(cb);
	_poller.addPollee(fd, e);
}

template<class Poller>
void EventQueue<Poller>::process()
{
	auto events = _poller.select();
	for(auto e : events)
	{
		process(e);
	}
}

}//net
NAMESP_END

