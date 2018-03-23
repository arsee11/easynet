///event.h

#ifndef EVENT_H
#define EVENT_H

#ifndef FD_DEF_H
#include "fddef.h"
#endif

#include <stdlib.h>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{


/////////////////////////////////////////////////////////////////////////
class EventSender
{
public:
	EventSender(fd_t fd):_fd(fd){}
	virtual ~EventSender(){}

	fd_t fd()const{ return _fd; }

protected:
	fd_t _fd;
};


//////////////////////////////////////////////////////////////////////////
class Event
{
public:
	Event(EventSender* s)
		:_sender(s)
	{
	}

	virtual ~Event()
	{
		if(_extra != nullptr)
			free(_extra);
	}

	EventSender* sender(){ return _sender; }

	///take onwership of extra.
	void extra(void* extra, size_t len){ _extra = extra;  _extra_len=len; }
	void* extra(){ return _extra; }
	size_t extra_len(){ return _extra_len; };

protected:
	EventSender* _sender;
	void* _extra = nullptr;
	size_t _extra_len = 0;
};


}//net
NAMESP_END

#endif /*EVENT_H*/
