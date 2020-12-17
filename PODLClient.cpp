#include "PODLClient.h"

/**
 * PODLClient::PODLClient 
 * 
 * @param  {string} ipaddr : 
 * @param  {int} port      : 
 */
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
/**
 * PODLClient::~PODLClient 
 * 
 */
PODLClient::~PODLClient()
{
	close(sock);
}
/**
 * PODLClient::RecvPacket 
 * 
 * @return {int}  : 
 */
int PODLClient::RecvPacket()
{
	int n = 0;
	socklen_t slen = sizeof(addr);
	char* buf = new char[PODL_MAX_SIZE];
	while(1)
	{
		//send the message
		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		//try to receive some data, this is a blocking call
		n = recvfrom(sock, buf, PODL_MAX_SIZE, 0, (struct sockaddr *) &addr, &slen);
		if(n == -1)
		{
			printf("Error receiving data\n");
		}

        PODLPacket packet(buf);
		std::cout << packet;
		printf("\n");
	}
	close(sock);
	return 0;
}

/**
 * PODLClient::SendPacket 
 * 
 * @param  {char*} msg : 
 * @param  {int} size  : 
 * @return {int}       : 
 */
int PODLClient::SendPacket(const char* msg, int size)
{
	socklen_t slen = sizeof(addr);
	if(sendto(sock, msg, size, 0, (const struct sockaddr*)&addr, slen) == -1)
	{
		printf("Send Failed");
	}
	return 0;
}

/**
 * PODLClient::Run() 
 * 
 * @return {int}  : 
 */
int PODLClient::Run()
{
    //byte buffer
    char buffer[PODL_MAX_SIZE];

	PODLPacket goodPacket;
    goodPacket.msg.id = 28;
    
    unsigned char checksum[MD5_DIGEST_LENGTH];

    //Correct Password Case
    string stringData = "password";
    goodPacket.msg.length = stringData.length();
    int packetSize = goodPacket.msg.length + PODL_MIN_SIZE;

    std::copy(stringData.c_str(), stringData.c_str() + goodPacket.msg.length, goodPacket.msg.data);
    MD5((unsigned char*)&goodPacket.msg, packetSize, (unsigned char*)&checksum);

    memcpy(buffer, (char*)&goodPacket.msg, packetSize);
    memcpy(buffer + packetSize, (char*)checksum, MD5_DIGEST_LENGTH);


	SendPacket(buffer, goodPacket.msg.length + PODL_CS_MIN_SIZE);
	RecvPacket();

    //wrong password case
    stringData = "pas3swo";
    goodPacket.msg.length = stringData.length();
    packetSize = goodPacket.msg.length + PODL_MIN_SIZE;

    std::copy(stringData.c_str(), stringData.c_str() + goodPacket.msg.length, goodPacket.msg.data);
    MD5((unsigned char*)&goodPacket, packetSize, (unsigned char*)&checksum);

    memcpy(buffer, (char*)&goodPacket.msg, packetSize);
    memcpy(buffer + packetSize, (char*)checksum, MD5_DIGEST_LENGTH);

	SendPacket(buffer, goodPacket.msg.length + PODL_CS_MIN_SIZE);
	RecvPacket();

    //wrong checksum case
    stringData = "password";
    goodPacket.msg.length = stringData.length();
    packetSize = goodPacket.msg.length + PODL_MIN_SIZE;

    std::copy(stringData.c_str(), stringData.c_str() + goodPacket.msg.length, goodPacket.msg.data);
    //MD5((unsigned char*)&goodPacket, packetSize, (unsigned char*)&checksum);

    memcpy(buffer, (char*)&goodPacket.msg, packetSize);
    memcpy(buffer + packetSize, (char*)checksum, MD5_DIGEST_LENGTH);
    
	SendPacket(buffer, goodPacket.msg.length + PODL_CS_MIN_SIZE);
	RecvPacket();

	return 0;
}
/*int main( int argc, const char* argv[] )
{
	PODLClient cli("127.0.0.1", 10000);
	cli.Run();

	return 1;
}*/
