
#include "../addr.h"

#include <iostream>

using namespace std;
using namespace arsee::net;

void test_get_a_host_ip()
{
    cout<<"got a ip:"<<IP::get_a_host_ipv4()<<endl;
}

int main()
{
	test_get_a_host_ip();
}
