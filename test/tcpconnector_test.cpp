//tcpconnector_test.cpp

#include "../connector.h"

#include <iostream>
#include <stdio.h>

using namespace arsee::net;
using namespace std;

int main()
{
	sockInit();
	
	string buf;

	TCPConnector c(10000);
	NetPeer* peer = c.connect( AddrPair{10000, "172.16.3.25"} );

	if(peer == nullptr)
	{
		cout<<"connect failed"<<endl;
		return 1;
	}

	while( buf != "q")
	{
		cout<<"input something(q to quit):";
		cin>>buf;
		int len = peer->write(buf.c_str(), buf.size());
		cout <<len<<" bytes sent"<<endl;
		char rbuf[11]={0};
		len = peer->read(rbuf, 10);
		cout <<len<<" bytes recv:"<<rbuf<<endl;
	}
	
	cout<<"bye"<<endl;

	sockUninit();
	
}
