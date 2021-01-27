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

    //Add timeout to sock options
    struct timeval tv;
    tv.tv_sec = 999; // disable
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

	cliaddr = {0};
	srvaddr = {0};
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(port);
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    addrlen = sizeof(cliaddr); 
	//bind socket to port
	if( bind(sock, reinterpret_cast<sockaddr *>(&srvaddr), sizeof(srvaddr)) == -1)
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
   
	if (sendto(sock,buffer, size ,0 , reinterpret_cast<sockaddr *>(&cliaddr), slen) == -1)
        {
               fprintf(stderr, "sendto() failed\n");
        }

	return 0;
}

/**
 * PODLServer::RecvPacket
 * 
 * @param  {char*} buffer : 
 * @return {PODLPacket*} buffer : 
 * @return {int}   : packet status
 */
int PODLServer::RecvPacket(char* buffer, PODLPacket& packet)
{   //Message framing logic
    /*int peekBytes = 0;
    uint8_t reportedPasswordLen = 0;
    
    //Peek read data to get full length to properly frame message
    peekBytes = recvfrom(sock, buffer, PODL_MIN_SIZE,  MSG_PEEK, reinterpret_cast<sockaddr *>(&cliaddr), &addrlen);
    if(peekBytes > PODL_MIN_SIZE -1)
    {
        //ninth byte is password length
        reportedPasswordLen = static_cast<uint8_t>(buffer[8]);   
    }
    // -1 indicated error
    else if(peekBytes == -1)
    {
        std::cout << "PODLServer: Socket Error Receiving Data" << std::endl;
        isInvalidPacket = 1;
        return isInvalidPacket;
    }
    // password len invalid
    if(reportedPasswordLen > PODL_MAX_PASSWORD_LEN )
    {
        isInvalidPacket = 1;
        return isInvalidPacket;
    }*/
    int isInvalidPacket = 0;
    int numBytes = 0;
    numBytes = recvfrom(sock, buffer, PODL_MAX_SIZE,  MSG_WAITALL, reinterpret_cast<sockaddr *>(&cliaddr), &addrlen);
    if(numBytes > 0)
    {
        isInvalidPacket = packet.Deserialize(buffer);

        if(isInvalidPacket == 0)
            std::cout << "PODLServer: Received: " << packet << std::endl;
    }
    else
    {
        isInvalidPacket = 1;
    }
    

    return isInvalidPacket;
}


/**
 * PODLServer::Run 
 * 
 * @return {int}  : 
 */
int PODLServer::Run()
{
    //Receiver buffer
    std::vector<char> buffer(PODL_MAX_SIZE);
    //Response buffer
    std::vector<char> resbuffer(PODL_MAX_SIZE);

    char mdString1[33];
    char mdString2[33];

	unsigned char checksum[MD5_DIGEST_LENGTH];
    int passwordResult = 1;
    int isInvalidPacket = 0;
    bool running = true;

	while(running)
	{
        //Clear buffers
        buffer.assign(PODL_MAX_SIZE,0);
        resbuffer.assign(PODL_MAX_SIZE,0);


        //Received Packet
        PODLPacket packet = PODLPacket();
        isInvalidPacket = RecvPacket(buffer.data(), packet);
        
        if(!isInvalidPacket)
        {
            int msgSize = packet.msg.length + PODL_MIN_SIZE;
            // Response Packet
            PODLPacket resPacket = PODLPacket();

            // Checkpassword
            std::string sentPassword = std::string(packet.msg.data, packet.msg.length);
            passwordResult = std::lexicographical_compare(sentPassword.begin(), sentPassword.end(), password.begin(), password.end());
        
            // wrong password
            if(passwordResult == 1)
            {
                resPacket.msg.data[0] = 0x01;
                resPacket.msg.data[1] = 0x00;
                resPacket.msg.length = 2;
                std::cout << "PODLServer: Sent Password < Password" <<std::endl;
            }
            else
            {
                resPacket.msg.data[0] = 0x01;
                resPacket.msg.data[1] = 0xff;
                resPacket.msg.length = 2;
                std::cout << "PODLServer: Sent Password > Password" <<std::endl;

            }

            // correct password
            if(sentPassword == password)
            {
                resPacket.msg.data[0] = 0x00;
                resPacket.msg.length = 1;
                passwordResult = 0;
                std::cout << "PODLServer: Correct Password" <<std::endl;
            }
            // Check checksum
            MD5(reinterpret_cast<unsigned char*>(&packet.msg), msgSize, checksum);
            for(int i = 0; i < 16; i++)
                {
                    sprintf(&mdString1[i*2], "%02x", static_cast<unsigned int>(checksum[i]));
                    sprintf(&mdString2[i*2], "%02x", static_cast<unsigned int>(packet.checksum[i]));
                }
        
            int checksumRes = strcmp(mdString1, mdString2);

            if(checksumRes != 0)
            {
                resPacket.msg.data[0] = 0x02;
                resPacket.msg.length = 1;
                std::cout << "PODLServer: Incorrect Checksum" <<std::endl;
            }

            //copy id from received packet
            resPacket.msg.id = packet.msg.id;
            int datalength = resPacket.msg.length + PODL_MIN_SIZE;

            MD5((unsigned char*)&resPacket.msg, datalength ,resPacket.checksum);
            
            resPacket.Serialize(resbuffer.data());
            SendPacket(resbuffer.data(), resbuffer.size());
        }
		
	}

	return 0;
}

/*
int main( int argc, const char* argv[] )
{
    std::string password = "password";
	PODLServer serv("127.0.0.1", 10000, password);
	serv.Run();

	return 1;
}
*/