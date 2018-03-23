///eventqueue_test.cpp
//
//

#include "../event_queue.h"
#include "../netevents.h"
#include "../netevent_listener.h"
#include "../netpeer.h"

#include <iostream>

using namespace std;
using namespace arsee::net;

class MyEventListener: public NetworkEventListener
{
public: 
	void onNewPeer(NetPeer* conn)override
	{
		cout<<"new peer{"<<conn->remote_addr().ip<<","<<conn->remote_addr().port<<"}"<<endl;
	}	

	void onInput(const void* buf, size_t len, NetPeer* conn)override
	{
		cout<<"onInput"<<endl;
	}	

	void onOutput(NetPeer* conn)override
	{
		
		cout<<"onOutput"<<endl;
	}


	void onClose(NetPeer* conn)override
	{
		cout<<"connection{"<<endl;
	}
};

typedef EventQueue event_queue_t;

bool filter(Event* e)
{
	return false;
};

int main()
{
	event_queue_t eq;
	MyEventListener* el = new MyEventListener;
	el->listen<NetInputEvent>();
	el->listen<NetAcceptEvent>();
	eq.addListener(el);
	eq.addFilter<NetInputEvent>(filter);
	eq.pushEvent( new NetAcceptEvent(new NetPeer(1, AddrPair{10000, "192.168.2.1"})) );
	eq.pushEvent( new NetInputEvent(new EventSender(1)) );
	eq.process();
}
