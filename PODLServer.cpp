#include <chrono>
#include "PODLServer.h"

/**
 * PODLServer::PODLServer 
 * 
 * @param  {std::string} ipaddr    : unused
 * @param  {int} port              : 
 * @param  {std::string} ipassword : 
 */
PODLServer::PODLServer(std::string ipaddr, int port, std::string ipassword)
{
    password = ipassword;

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
/**
 * PODLServer::~PODLServer 
 *  Close socket
 */
PODLServer::~PODLServer()
{
	close(sock);
}


/**
 * PODLServer::SendPacket
 * 
 * @param  {PODLPacket} packet : 
 * @return {int}               : 
 */
int PODLServer::SendPacket(char* buffer, int size)
{
	socklen_t slen = sizeof(cliaddr);
   
	if (sendto(sock,buffer, size ,0 , (struct sockaddr*) &cliaddr, slen) == -1)
        {
               fprintf(stderr, "sendto() failed\n");
        }

	return 0;
}

/**
 * PODLServer::RecvPacket
 * 
 * @param  {char*} buffer : 
 * @return {PODLPacket}   : 
 */
PODLPacket PODLServer::RecvPacket(char* buffer)
{
    	socklen_t len = sizeof(cliaddr); 

    	int n = recvfrom(sock, (char *)buffer, PODL_MAX_SIZE,  0, ( struct sockaddr *) &cliaddr, &len);
    	PODLPacket packet;
        if(n > 0)
        {
            packet = PODLPacket(buffer);
    	    std::cout << "PODLServer: Received: " << packet << std::endl;
        }

	    return packet;
}

/**
 * PODLServer::Run 
 * 
 * @return {int}  : 
 */
int PODLServer::Run()
{
	char *buffer = new char[PODL_MAX_SIZE];
    char *resbuffer = new char[PODL_MAX_SIZE];
	unsigned char *checksum = new unsigned char [MD5_DIGEST_LENGTH];
    int passwordResult = 1;
    
	while(1)
	{
        //Clear buffers
        memset(buffer, 0, PODL_MAX_SIZE);
        memset(resbuffer, 0, PODL_MAX_SIZE);
        //memset(checksum, 0, PODL_MAX_SIZE);

        //Received Packet
        PODLPacket packet = RecvPacket(buffer);
        int packetSize = packet.msg.length + PODL_CS_MIN_SIZE; // 25 is size of packet without data
        int msgSize = packet.msg.length + PODL_MIN_SIZE;
        // Response Packet
        PODLPacket resPacket = PODLPacket();

        // Checkpassword
        passwordResult = password.compare(std::string(reinterpret_cast<const char*>(&packet.msg.data [0]),
                            packet.msg.length));

        // correct password
        if(passwordResult == 0)
        {
           resPacket.msg.data[0] = 0; 
           std::cout << "PODLServer: Correct Password" <<std::endl;
        }
        // wrong password
        else
        {
            resPacket.msg.data[0] = 1;
            std::cout << "PODLServer: Incorrect Password" <<std::endl;
        }
        // Check checksum
        MD5(reinterpret_cast<unsigned char *>(buffer), msgSize, checksum);
        char mdString1[33];
        char mdString2[33];
        for(int i = 0; i < 16; i++)
            {
                sprintf(&mdString1[i*2], "%02x", (unsigned int)checksum[i]);
                sprintf(&mdString2[i*2], "%02x", (unsigned int)packet.checksum[i]);
            }
    
        int checksumRes = strcmp(mdString1, mdString2);

        if(checksumRes != 0)
        {
            resPacket.msg.data[0] = 2;
            std::cout << "PODLServer: Incorrect Checksum" <<std::endl;
        }

        resPacket.msg.id = packet.msg.id;
        resPacket.msg.length = 1;
        int datalength = resPacket.msg.length + PODL_MIN_SIZE;

        MD5(reinterpret_cast<unsigned char *>(resbuffer), datalength , (unsigned char*)&resPacket.checksum);

        memcpy(resbuffer, &resPacket, datalength); 
        memcpy(resbuffer, &resPacket.checksum, 16);
		SendPacket(resbuffer, datalength + 16);
		std::cout << packet;
		
	}

    delete[] buffer;
    delete[] resbuffer;
    delete[] checksum;

	return 1;
}

/*int main( int argc, const char* argv[] )
{
    std::string password = "password";
	PODLServer serv("127.0.0.1", 10000, password);
	serv.Run();

	return 1;
}*/
