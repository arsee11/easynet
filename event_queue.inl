///event_queue.inl
//
//

NAMESP_BEGIN
namespace net
{

inline void EventQueue::bind(std::type_index evttype, EventListener* l){ 
	const auto& i = _type_listener_map.find(evttype);
	if( i == _type_listener_map.end())
	{
		listener_list ls;
		ls.push_back(l);
		_type_listener_map.insert(std::make_pair(evttype, ls) );
	}
	else
		i->second.push_back(l);
}

inline void EventQueue::unbind(std::type_index evttype, EventListener* l){
	const auto& i = _type_listener_map.find(evttype);
	if( i == _type_listener_map.end())
	{
		i->second.remove(l);
	}
}

inline void EventQueue::bindfd(std::type_index evttype, fd_t fd, EventListener* l){ 
	const auto& i = _fd_listener_map.find(evttype);
	if( i == _fd_listener_map.end())
	{
		listener_list ls;
		ls.push_back(l);
		listener_map lsm;
		lsm.insert( std::make_pair(fd, ls) );
		_fd_listener_map.insert(std::make_pair(evttype, lsm) );
	}
	else
	{
		auto j = i->second.find(fd);
		if(j == i->second.end() )
		{
			listener_list ls;
			ls.push_back(l);
			i->second.insert( std::make_pair(fd, ls) );
		}
		else
			j->second.push_back(l);

	}
}

inline void EventQueue::unbindfd(std::type_index evttype, fd_t fd){
	const auto& i = _fd_listener_map.find(evttype);
	if( i != _fd_listener_map.end())
	{
		i->second.erase(fd);
	}
}

inline void EventQueue::process(){
	while(!_events.empty() )
	{
		event_ptr e = _events.front();
		_events.pop_front();
		//if filter return true, break the event process
		/*if(filter(e))
			return;
		*/

		process(e);
	}
}

/*	bool filter(event_ptr e){
		auto i = _event_filter_map.find( std::type_index( typeid(*e)) );
		if( i != _event_filter_map.end() )
			return i->second(_filters, e);

		return false;
	}
*/

}//net
NAMESP_END

