#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Order.h"

using namespace std;

class UDPServer
{

private:
	sockaddr_in srvaddr, cliaddr;
	int BUFLEN = 512;
	int sock = 0;
public:
	UDPServer(string ipaddr, int port);
	~UDPServer();
	int SendData(const char* data, int size);
	int SendOrder(Order order);
	int RecvData(char* data);
	int Run();

};
