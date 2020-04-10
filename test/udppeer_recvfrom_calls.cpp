//udppeer_test.cpp


#include <iostream>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv)
{
	if(argc < 2){
		cout<<"usage: cmd [local port]\n";
		return 0;
	}

	int port = atoi(argv[1]);
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd == -1 ){
		perror("socket");
		return 0;
	}

	if( bind(fd, (sockaddr*)&addr, sizeof(addr)) == -1){
		perror("bind");
		return 0;
	}
	
	sockaddr_in inaddr;
	::memset(&inaddr, 0, sizeof(inaddr));
    socklen_t naddr=sizeof(inaddr);

	char buf[24]={0};
	int size=2;
	int n=5000000;
	auto t1=chrono::high_resolution_clock::now();
	for(int i=0; i<n; i++){
		recvfrom(fd, buf, size, 0, (sockaddr*)&inaddr, &naddr);
		//read(fd, buf, size);
	}
	auto t2=chrono::high_resolution_clock::now();
	auto d=t2-t1;
	auto dms=chrono::duration_cast<chrono::milliseconds>(d);
	long ms = dms.count();
	cout<<"took:"<<ms<<"ms\n";
	cout<<n/ms*1000<<" times call per second\n";


	return 0;
}
