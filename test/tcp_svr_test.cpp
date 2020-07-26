//netpeer_test.cpp

#include "../acceptor.h"
#include "../netlisteners.h"

#include <iostream>
#include <stdio.h>

using namespace arsee::net;
using namespace std;
using namespace std::placeholders;

struct MySession  
{

	MySession(NetEventQueue* eq)
		:_alistener(eq)
		,_ilistener(eq)
		,_clistener(eq)
	{
		_alistener.listen([this](const netpeer_ptr& peer){ this->onAccept(peer); });
	}

	void onAccept(const netpeer_ptr& peer)
	{
		cout<<"onAccept["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"<<endl;
		//listen<InputCmpEvent>( std::bind(&MySession::onInput, this, _1, _2) );
		_ilistener.listen(peer->fd(), [this](const netpeer_ptr& p, MsgSt m){this->onInput(p, m);}); 
		_clistener.listen(peer->fd(), [this](const netpeer_ptr& peer){ this->onClose(peer); });
		_peers.push_back(peer);
	}

	void onInput(const netpeer_ptr& peer, MsgSt msg) 
	{
		cout<<"onInput("<<peer->fd()<<")["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"<<endl;
		cout<<msg.begin()<<"("<<msg.size()<<")"<<endl;
		peer->write(msg);
	}

	void onClose(const netpeer_ptr& peer)
	{
		cout<<"onClose["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"<<endl;
		_ilistener.unlisten(peer->fd());
		_clistener.unlisten(peer->fd());
		_peers.remove(peer);
	}

	std::list<netpeer_ptr> _peers;
	AcceptCmpListener _alistener;
	InputCmpListener _ilistener;
	CloseCmpListener _clistener;
};

int main()
{
	NetEventQueue eq;
	AcceptorC a(&eq, 4444);	

	eq.attach( a.fd() );

	MySession s(&eq);

	while(true)
	{
		eq.process();
	}

	getchar();

}
