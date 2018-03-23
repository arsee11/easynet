//select.h

#ifndef SELECT_H	
#define SELECT_H	

#include <sys/epoll.h>
#include <fcntl.h>
#include <exception>
#include <iostream>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

using namespace std;

NAMESP_BEGIN
namespace net
{

class Select
{
public:
	Select()
	{
	}
	
	//@return  alivable events.
	event_set_t select()
	{
#ifdef DEBUG
		cout<<"selecting..."<<endl;
#endif
	}

};

}//net
NAMESP_END

#endif /*SELECT_H*/	
