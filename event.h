///event.h

#ifndef EVENT_H
#define EVENT_H

#ifndef FD_DEF_H
#include "fddef.h"
#endif

#include <stdlib.h>

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{


//////////////////////////////////////////////////////////////////////////
class Event
{
public:
	Event(void* event_sender)
		:_sender(event_sender)
	{
	}

	virtual ~Event()
	{
	}

	void* sender(){ return _sender; }

	void extra(void* extra, size_t len){ _extra = extra;  _extra_len=len; }
	void* extra(){ return _extra; }
	size_t extra_len(){ return _extra_len; };

protected:
	void* _sender;
	void* _extra = nullptr;
	size_t _extra_len = 0;
};


}//net
NAMESP_END

#endif /*EVENT_H*/
