///eventqueue_test.cpp
//
//

#include "../event_queue.h"
#include "../netevents.h"

#include <iostream>

using namespace std;
using namespace arsee::net;

void onInput(fd_t fd)
{
	cout<<"onInput:"<<fd<<endl;
}	


int main()
{
	EventQueue eq;
	event_ptr evt = std::make_shared<NetInputEvent>(onInput, 101);
	eq.pushEvent(evt); 
	eq.process();
}
