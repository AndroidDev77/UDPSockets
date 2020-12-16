#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Order.h"

using namespace std;

class UDPClient
{

private:
	sockaddr_in addr;
	int BUFLEN = 512;
	int sock = 0;
public:
	UDPClient(string ipaddr, int port);
	~UDPClient();
	int RecvData();
	int SendData(const char* msg, int size);
	int Run();

};
