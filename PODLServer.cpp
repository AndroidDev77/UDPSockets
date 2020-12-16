#include <chrono>
#include "PODLServer.h"

PODLServer::PODLServer(std::string ipaddr, int port, std::string password)
{
    password = password;

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

PODLServer::~PODLServer()
{
	close(sock);
}

int PODLServer::SendPacket(const char* message, int size)
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

int PODLServer::SendPacket(PODLPacket packet)
{
	socklen_t slen = sizeof(cliaddr);
    int datalength = packet.msg.length + 25;
	if (sendto(sock,(char*) &packet, datalength,0 , (struct sockaddr*) &cliaddr, slen) == -1)
        {
               fprintf(stderr, "sendto() failed\n");
        }

	return 0;
}

PODLPacket PODLServer::RecvPacket(char* buffer)
{
    	socklen_t len = sizeof(cliaddr); 

    	int n = recvfrom(sock, (char *)buffer, BUFLEN,  0, ( struct sockaddr *) &cliaddr, &len);
    	
        PODLPacket packet = PODLPacket(buffer);
    	printf("Client : %s\n", buffer);

	    return packet;
}

int PODLServer::Run()
{
	char buffer[BUFLEN];
	unsigned char checksum[MD5_DIGEST_LENGTH];
    int passwordResult = 1;
    
	while(1)
	{
        //Received Packet
        PODLPacket packet = RecvPacket(buffer);
        int packetSize = packet.msg.length + PODL_CS_MIN_SIZE; // 25 is size of packet without data

        // Response Packet
        PODLPacket resPacket = PODLPacket();

        // Checkpassword
        
        passwordResult = password.compare(std::string(reinterpret_cast<const char*>(&packet.msg.data [0]),
                            packet.msg.data.size()));

        // correct password
        if(passwordResult == 0)
           resPacket.msg.data[0] = 0;
        // wrong password
        else
            resPacket.msg.data[0] = 1;

        // Check checksum
        MD5((unsigned char*)&buffer, packetSize, (unsigned char*)&checksum);
        char mdString1[33];
        char mdString2[33];
        for(int i = 0; i < 16; i++)
            {
                sprintf(&mdString1[i*2], "%02x", (unsigned int)checksum[i]);
                sprintf(&mdString2[i*2], "%02x", (unsigned int)packet.checksum[i]);
            }
    
        int checksumRes = strcmp(mdString1, mdString2);
        if(checksumRes != 0)
            resPacket.msg.data[0] = 2;

        resPacket.msg.id = packet.msg.id;
        resPacket.msg.length = 1;
		SendPacket(packet);
		std::cout << packet;
		
	}
	return 1;
}
int main( int argc, const char* argv[] )
{
    std::string password = "password";
	PODLServer serv("127.0.0.1", 10000, password);
	serv.Run();

	return 1;
}
