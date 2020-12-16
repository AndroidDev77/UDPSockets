#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "openssl/md5.h"
#include "PODLPacket.h"

using namespace std;

class PODLServer
{

private:
	sockaddr_in srvaddr, cliaddr;
	int BUFLEN = 280;
	int sock = 0;
    string password;
public:
	PODLServer(string ipaddr, int port, string password);
	~PODLServer();
	int SendPacket(const char* data, int size);
	int SendPacket(PODLPacket packet);
	PODLPacket RecvPacket(char* data);
	int Run();

};
