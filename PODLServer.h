#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "openssl/md5.h"
#include "PODLPacket.h"

/**
 * UDP Server for sending PODL Packets
 */
class PODLServer
{

private:
	sockaddr_in srvaddr, cliaddr;
	int sock = 0;
    std::string password;
public:
	PODLServer(std::string ipaddr, int port, std::string password);
	~PODLServer();
	int SendPacket(char* buffer, int size);
	PODLPacket RecvPacket(char* msg);
	int Run();

};
