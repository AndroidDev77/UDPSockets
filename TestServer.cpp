#include <chrono>
#include "TestServer.h"

TestServer::TestServer(string ipaddr, int port)
{
	if((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		printf("Error creating socket\n");
		exit(1);
	}
	cliaddr = {0};
	srvaddr = {0};
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(port);
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind socket to port
	if( bind(sock, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) == -1)
	{
		printf("bind() error");
		exit(1);
	}

}

TestServer::~TestServer()
{
	close(sock);
}



int main( int argc, const char* argv[] )
{
	TestServer serv("127.0.0.1", 4545);

	return 1;
}
