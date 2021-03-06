#include <string>
#include <iostream>
#include <memory>
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
	sockaddr_in addr = {0};
	int sock = 0;
	socklen_t slen;
public:
	PODLClient(string ipaddr, int port);
	~PODLClient();

	int SendPacket(const char* data, int size);
	int RecvPacket(char* resbuffer,  PODLPacket& packet);

	int SendPacket(PODLPacket& sendPacket, PODLPacket& resPacket);

	int CheckPassword(std::string password);
	int RunTests();
	int RunBinarySearchCrack();

};
