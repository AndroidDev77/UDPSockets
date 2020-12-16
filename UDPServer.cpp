#include <chrono>
#include "UDPServer.h"

UDPServer::UDPServer(string ipaddr, int port)
{
	if((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		printf("Error creating socket\n");
		exit(1);
	}
	cliaddr = {0};
	srvaddr = {0};
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(port);
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind socket to port
	if( bind(sock, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) == -1)
	{
		printf("bind() error");
		exit(1);
	}

}

UDPServer::~UDPServer()
{
	close(sock);
}

int UDPServer::SendData(const char* message, int size)
{
	int i = 0;
	socklen_t slen = sizeof(cliaddr);
	
	while(1)
	{

		//send the message
		if (sendto(sock, message, size,0 , (struct sockaddr*) &cliaddr, slen) == -1)
		//if(send(sock, message, size, 0)==1)
		{
			fprintf(stderr, "sendto() failed\n");
		}
		i++;
		printf("Length: %d %d Sent Message # %d \n", size, slen, i);

		sleep(2);

	}
	close(sock);
	return 0;
}

int UDPServer::SendOrder(Order order)
{
	socklen_t slen = sizeof(cliaddr);

	if (sendto(sock,(char*) &order, order.item.size,0 , (struct sockaddr*) &cliaddr, slen) == -1)
        {
               fprintf(stderr, "sendto() failed\n");
        }

	return 0;
}

int UDPServer::RecvData(char* buffer)
{
    	int n;
    	socklen_t len = sizeof(cliaddr);  //len is value/resuslt

    	n = recvfrom(sock, (char *)buffer, BUFLEN,  0, ( struct sockaddr *) &cliaddr, &len);
    	buffer[n] = '\0';
    	printf("Client : %s\n", buffer);
	return 0;
}

int UDPServer::Run()
{
	char buffer[BUFLEN];
	RecvData(buffer);
	string testMessage = "Hello World";
	while(1)
	{
        	Order order;
        	uint64_t milli = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		order.item.time = milli;
        	order.item.size = sizeof(Order);
        	string name = "Zoom";
		memcpy(&order.item.assetName,name.c_str(),4);
        	order.item.price = 10.50;
        	order.item.quantity = 3;
        	order.item.orderType = 1;

		SendOrder(order);
		std::cout << order;
		sleep(2);
	}
	//SendData(testMessage.c_str(), testMessage.length());
	return 1;
}
int main( int argc, const char* argv[] )
{
	UDPServer serv("127.0.0.1", 4545);
	serv.Run();

	return 1;
}
