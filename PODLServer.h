#include <string>
#include <iostream>
#include <memory>
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
	socklen_t addrlen;
	std::string password;
public:
	PODLServer(std::string ipaddr, int port, std::string password);
	~PODLServer();
	int SendPacket(char* buffer, int size);
	int RecvPacket(char* msg, PODLPacket* packet);
	int Run();

};
