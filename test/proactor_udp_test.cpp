//test.cpp

#include <iostream>
#include <strings.h>
#include <string.h>
#include <memory>
#include <string>
#include "../proactor.h"
#include "../epoll.h"
#include "../acceptor.h"
#include "../netevent_listener.h"

using namespace std;
using namespace std::placeholders;
using namespace arsee::net;


class NetSession
{
public: 
	NetSeesion(NetPeer* peer)
		:_peer(peer)
	{
		peer->listen<NetPeer::InputEvent>(std::bind(&NetSession::onInput, this, _1, _w));
	}


	void onInput(const void* buf, size_t len)
	{
		cout<<"recv{"<<_peer->remote_addr().ip<<","<<_peer->remote_addr().port<<"}";
		cout<<"("<<len<<"):"<<(const char*)buf<<endl;	
		peer->write((char*)buf, len);
	}	

	void onOutput(size_t bytesSent)
	{
		cout<<"sent{"<<_peer->remote_addr().ip<<","<<_peer->remote_addr().port<<"}";
		cout<<"("<<len<<")"<<endl;
	}


	void onClose()
	{
		cout<<"close{"<<_peer->remote_addr().ip<<","<<_peer->remote_addr().port<<"}";
	}

private:
	NetPeer* _peer;
};

void onAccept(NetPeer* peer)
{
	cout<<"accept{"<<peer->remote_addr().ip<<","<<peer->remote_addr().port<<"}";
}

int main( )
{
	
	Proactor p;
	EventQueue eq;
	eq.addAddFilter(&p);

	AcceptorUdp a(&eq, 1261, false);
	a.listen<NetAccepptEvent>(onAccept);

	NetPeer* peer = a.getPeer(AddrPair("127.0.0.1", 1261);
	NetSession session(peer);


	while(true)
	{
		eq.poll();
	}

	return 0;
};
