
#include "../addr.h"

#include <iostream>

using namespace std;
using namespace arsee::net;

void test_getip_getport()
{
	sockaddr_in addr;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = inet_addr("192.168.2.1");
	
	cout<<"ip:"<<getip(&addr)<<", port:"<<getport(&addr)<<endl;
}
	

int main()
{
	test_getip_getport();
}
