#include "../netevents.h"
#include <iostream>


using namespace std;
using namespace std::placeholders;
using namespace arsee::net;


struct Listener
{
void onEvent(fd_t fd){
	cout<<"fd="<<fd<<endl;
}
}; 

void listener(NetPeer* p, const void* msg, size_t size){
	cout<<"p="<<p<<",msg="<<(const char*)msg<<",size="<<size<<endl;
}

int main()
{
	fd_t fd=100;
	Listener l;
	NetInputEvent evt(std::bind(&Listener::onEvent, &l, _1), fd);
	evt.fire();
	
	const char* msg="hello,world";
	NetInputCompletedEvent evt2(listener, nullptr, msg, 12);
	evt2.fire();
	return 0;
}
