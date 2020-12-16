#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Order.h"

using namespace std;

class TestServer
{

private:
	sockaddr_in srvaddr, cliaddr;
	int BUFLEN = 512;
	int sock = 0;
public:
	TestServer(string ipaddr, int port);
	~TestServer();
	int SendData(const char* data, int size);
	int SendOrder(Order order);
	int RecvData(char* data);
	int Run();

};