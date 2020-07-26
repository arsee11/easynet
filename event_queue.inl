///event_queue.inl
//
//

NAMESP_BEGIN
namespace net
{

template<
	template<class> class Poller
>
void EventQueue<Poller>::bind(fd_t fd, event_ptr e){
	_poller.addInput(fd, e);
}

template<
	template<class> class Poller
>
void EventQueue<Poller>::unbind(fd_t fd){
}

template<
	template<class> class Poller
>
void EventQueue<Poller>::process(){
	auto events = _poller.select();
	for(auto e : events)
	{
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

