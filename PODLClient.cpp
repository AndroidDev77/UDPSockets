#include "PODLClient.h"


PODLClient::PODLClient(string ipaddr, int port)
{
	if((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		perror("Error creating socket");
		exit(1);
	}

	addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	
	if(inet_aton(ipaddr.c_str(), &addr.sin_addr) == 0)
	{
		fprintf(stderr, "inet_aton() error\n");
		exit(1);
	}

}

PODLClient::~PODLClient()
{
	close(sock);
}

int PODLClient::RecvPacket()
{
	//char* message = new char[UDPServer::BUFLEN];
	int i = 0;
	int n = 0;
	socklen_t slen = sizeof(addr);
	char* buf = new char[BUFLEN];
	PODLPacket packet;
	while(1)
	{
		//send the message
		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		//try to receive some data, this is a blocking call
		n = recvfrom(sock, (char*)&packet, BUFLEN, 0, (struct sockaddr *) &addr, &slen);
		if(n == -1)
		{
			printf("Error receiving data\n");
		}
		std::cout << packet;
		printf("\n");
	}
	close(sock);
	return 0;
}
int PODLClient::SendPacket(const char* msg, int size)
{
	socklen_t slen = sizeof(addr);
	if(sendto(sock, msg, size, 0, (const struct sockaddr*)&addr, slen) == -1)
	{
		printf("Send Failed");
	}
	return 0;
}

int PODLClient::Run()
{
	string msg = "hello Server";
	SendPacket(msg.c_str(), msg.length());
	RecvPacket();
	return 0;
}
int main( int argc, const char* argv[] )
{
	PODLClient cli("127.0.0.1", 10000);
	cli.Run();

	return 1;
}
