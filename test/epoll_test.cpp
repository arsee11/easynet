///epollr_test.cpp
//
//

#include "../epoll.h"
#include "../acceptor.h"
#include <iostream>

using namespace arsee::net;
using namespace std;

int main()
{
	Acceptor a(10000);
	Epoll e;
	e.listenfd(a.fd() );
	while(true)
	{
		Epoll::event_set_t evts = e.select();
		cout<<"num of events:"<<evts.size()<<endl;
	/*	for(auto evt : evts)
		{
			if( evt->type() == NetEvent::ACCEPT )
			{
				NetPeer* peer = a.accept();
				if(peer != nullptr)
				{
					cout<<"new peer{"<<peer->remote_addr().ip<<","
					<<peer->remote_addr().port<<"}"<<endl;

					e.attach(peer->fd());
				}
			}
			else if( evt->type() == NetEvent::INPUT)
			{
				char buf[1024];
				int n = recv(evt->sender()->fd(), buf, 1024, 0);
				if(n < 0 )
					perror("recv:");
				else if(n == 0)
					cout<<"remote closed"<<endl;
				else
					cout<<"recv:"<<buf<<endl;	
			}
			else if( evt->type() == NetEvent::CLOSE)
			{
				close(evt->sender()->fd());
				cout<<"close"<<endl;	
			}
		}
		
	*/
	}
};
