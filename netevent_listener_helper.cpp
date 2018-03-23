///netevent_listener_helper.cpp
//

#include "netevent_listener_helper.h"
#include "netevent_listener.h"

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

void procNetInput(Event* e, NetworkEventListener* l )
{
	l->onInput(e->extra(), e->extra_len(), (NetPeer*)e->sender());
}

void procNetAccept(Event* e, NetworkEventListener* l )
{
	l->onNewPeer((NetPeer*)e->sender());
}

void procNetClose(Event* e, NetworkEventListener* l )
{
	l->onClose((NetPeer*)e->sender());
}

}//net
NAMESP_END

