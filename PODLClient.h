#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "PODLPacket.h"


using namespace std;
/**
 *  UDP Client for Sending PODL Packets
 */

class PODLClient
{

private:
	sockaddr_in addr;
	int sock = 0;
public:
	PODLClient(string ipaddr, int port);
	~PODLClient();
	int SendPacket(const char* data, int size);
	int SendPacket(PODLPacket packet);
	int RecvPacket();
	int Run();

};
