#include "UDPClient.h"


UDPClient::UDPClient(string ipaddr, int port)
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

UDPClient::~UDPClient()
{
	close(sock);
}

int UDPClient::RecvData()
{
	//char* message = new char[UDPServer::BUFLEN];
	int i = 0;
	int n = 0;
	socklen_t slen = sizeof(addr);
	char* buf = new char[BUFLEN];
	Order order;
	int orderSize = sizeof(Order);
	while(1)
	{
		//send the message
		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		//try to receive some data, this is a blocking call
		n = recvfrom(sock, (char*)&order, orderSize, 0, (struct sockaddr *) &addr, &slen);
		if(n == -1)
		{
			printf("Error receiving data\n");
		}
		std::cout << order;
		printf("\n");
	}
	close(sock);
	return 0;
}
int UDPClient::SendData(const char* msg, int size)
{
	socklen_t slen = sizeof(addr);
	if(sendto(sock, msg, size, 0, (const struct sockaddr*)&addr, slen) == -1)
	{
		printf("Send Failed");
	}
	return 0;
}

int UDPClient::Run()
{
	string msg = "hello Server";
	SendData(msg.c_str(), msg.length());
	RecvData();
	return 0;
}
int main( int argc, const char* argv[] )
{
	UDPClient cli("127.0.0.1", 4545);
	cli.Run();

	return 1;
}
