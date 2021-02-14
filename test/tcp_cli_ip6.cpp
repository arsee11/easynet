#include <stdio.h>
#include "connector.h"

using namespace arsee::net;

#define MAXBUF 1024

int main(int argc, char **argv)
{
    char buf[MAXBUF + 1];
    if (argc != 3) {
        printf("usage: %s [ip6:port]\n", argv[0]);
        exit(0);
    }

	EventQueueEpoll eq;
    Connector6 conn(&eq, 10086);

    Connector6::netpeer_ptr peer = conn.connect(AddrPair{argv[1], atoi(argv[2])});
    if(peer != nullptr){
        peer->listenOnRecv([](const Connector6::netpeer_ptr& peer, const MsgSt& msg){
                printf("recv(%d):%s\n", msg.size(), msg.rd_ptr());
        });
        peer->write(MsgSt("hello,wolrd", 12));
    }

	while(true)
	{
		eq.process();
	}

    return 0;

}
