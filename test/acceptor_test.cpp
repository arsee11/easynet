//acceptor_test.cpp

#include "../acceptor.h"

#include <iostream>
#include <stdio.h>

using namespace arsee::net;
using namespace std;

int main()
{
	sockInit();
	

	Acceptor a(10000);	
	NetPeer* peer = a.accept();

	if(peer == nullptr)
	{
		cout<<"connect failed"<<endl;
		return 1;
	}
	
	cout<<"netpeer:{"<<peer->remote_addr().port<<","<<peer->remote_addr().ip<<"}"<<endl;

	string buf="Hello, I am Server";
	int len = peer->write(buf.c_str(), buf.size());
	cout <<len<<" bytes sent"<<endl;
	char rbuf[32]={0};
	len = peer->read(rbuf, 31);
	cout <<len<<" bytes recv:"<<rbuf<<endl;
	
	cout<<"bye"<<endl;

	sockUninit();
	
}
