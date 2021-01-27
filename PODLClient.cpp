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

	//Add timeout to sock options
    //struct timeval tv;
    //tv.tv_sec = 999;
    //tv.tv_usec = 0;
    //setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

	//addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	slen = sizeof(addr);

	
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
int PODLClient::RecvPacket(char* resbuffer, PODLPacket& packet)
{
	int numBytes = 0;
	int isInvalidPacket = 0;
	//send the message
	//receive a reply and print it
	//clear the buffer by filling null, it might have previously received data
	//try to receive some data, this is a blocking call
	try{
		numBytes = recvfrom(sock, resbuffer, PODL_MAX_SIZE, 0, reinterpret_cast<sockaddr *>(&addr), &slen);
		if(numBytes == -1)
		{
			std::cout <<"PODLClient: Error receiving data\n" << std::endl;
			return 1;
		}
			
		isInvalidPacket = packet.Deserialize(resbuffer);
		std::cout << "PODLClient: Response Received: " << packet  << std::endl;
	}
	catch(exception ex)
	{
		std::cout << "Error receiving Packet " << ex.what() << std::endl;
	}
	return isInvalidPacket;
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
	if(sendto(sock, msg, size, 0, reinterpret_cast<sockaddr *>(&addr), slen) == -1)
	{
		printf("Send Failed");
		return 1;
	}
	return 0;
}

int PODLClient::SendPacket(PODLPacket& sendPacket, PODLPacket& respPacket)
{
	std::vector<char> buffer(PODL_MAX_SIZE);
	std::vector<char> resbuffer(PODL_MAX_SIZE);
	MD5((unsigned char*)&sendPacket.msg, sendPacket.msg.length + PODL_MIN_SIZE, sendPacket.checksum);

	sendPacket.Serialize(buffer.data());
	SendPacket(buffer.data(), buffer.size());

	resbuffer.assign(PODL_MAX_SIZE,0);
	RecvPacket(resbuffer.data(), respPacket);

	return 0;
}


/**
 * PODLClient::Run() 
 * 
 * @return {int}  : 
 */
int PODLClient::RunTests()
{
	PODLPacket sendPacket;
	PODLPacket respPacket;
    sendPacket.msg.id = 28;
    
    unsigned char checksum[MD5_DIGEST_LENGTH];

    //Correct Password Case
    string stringData = "password";
    sendPacket.msg.length = stringData.length();
    int packetSize = sendPacket.msg.length + PODL_MIN_SIZE;
    std::copy(stringData.c_str(), stringData.c_str() + stringData.length(), sendPacket.msg.data);

	SendPacket(sendPacket, respPacket);

    //wrong password case
	stringData = "pas2rd";
    sendPacket.msg.length = stringData.length();
    std::copy(stringData.c_str(), stringData.c_str() + stringData.length(), sendPacket.msg.data);

	SendPacket(sendPacket, respPacket);

    //wrong checksum case, use previous checksum
    stringData = "password";
    sendPacket.msg.length = stringData.length();
    std::copy(stringData.c_str(), stringData.c_str() + stringData.length(), sendPacket.msg.data);

	SendPacket(sendPacket, respPacket);

    //wrong passwordlen
    stringData = "password";
    sendPacket.msg.length = stringData.length() + 30;
    std::copy(stringData.c_str(), stringData.c_str() + stringData.length(), sendPacket.msg.data);

	SendPacket(sendPacket, respPacket);
	return 0;
}



/** 
 * For challenge 2
 * PODLClient::Run() 
 * 
 * @return {int}  : 
 */
// Approach Start with test password
// Check Response, if response data == 0x00 correct
// if 0x01 0x00 increment password value
// if 0x01 0xff decrement password value
/*
Use binary search to reduce iterations
Treat 0...127 as array
*/
int PODLClient::RunBinarySearchCrack()
{
    std::string sentPassword =  std::string(1,0);
    bool passwordFound = false;
    int passwordIndex = 0;

    int l = 0;   //left bound
    int r = 127; //right bound
	int m = 0;   //middle

    while(!passwordFound)
    {
		//Set char from middle value
        m = l + (r-l)/2; 
        sentPassword[passwordIndex] = (char)m;
        int res = CheckPassword(sentPassword);
        std::cout << "Test Password: " <<  sentPassword << std::endl;

        if (res == 0)
        {
            r = m - 1;
        }
        else if(res == 1){
			//Add char and check if previous value was correct or just less than
            sentPassword.push_back((char)127);
            res = CheckPassword(sentPassword);
            if(res == 0)
            {
                std::cout << "Char found Moving to next char" << std::endl;
                passwordIndex++;
                l = 0;
                r = 127;
            }
            else
            {
				//char was incorrect remove test char
                sentPassword.pop_back();
                l = m + 1;
            }
        }
		//Correct Password Found
		else if(res == 2)
		{
			passwordFound = true;
			break;
		}
		//Bad MD5
		else
		{
			return 1;
		}
		
    }
	std::cout << "Cracked Password: " << sentPassword << std::endl;
	return 0;
}

/**
 * PODLClient::CheckPassword
 * 
 * @param  {std::string} password : password to try
 * @return {int}                  : result from PODL response
 * Note: Changes can be made to reduce dataspace but will be left
 * 		 for readability 
 * 		 
 */
int PODLClient::CheckPassword(std::string password)
{

	int returnCode = 0;

	PODLPacket sendPacket;
	PODLPacket respPacket;
    
    unsigned char checksum[MD5_DIGEST_LENGTH];

	string testPassword = password;
	int passwordLength = 0;
	sendPacket.msg.id = 28;

	sendPacket.msg.length = testPassword.length();
	int packetSize = sendPacket.msg.length + PODL_MIN_SIZE;

	std::copy(testPassword.c_str(), testPassword.c_str() + testPassword.length(), sendPacket.msg.data);
	MD5((unsigned char*)&sendPacket.msg, packetSize, (unsigned char*)&sendPacket.checksum);

	SendPacket(sendPacket, respPacket);

	// Correct Password
	if(respPacket.msg.data[0] == 0x00)
	{
		std::cout << "Found Password: " << testPassword.c_str() << std::endl; 
		returnCode = 2;
	}
	else if(respPacket.msg.data[0] == 0x01 && (unsigned char)(respPacket.msg.data[1]) == 0x00)
	{
		std::cout << "Sent Password:  < CorrectPassword " << testPassword.c_str() << std::endl; 
		returnCode = 1;
	}
	else if(respPacket.msg.data[0] == 0x01 &&  (unsigned char)respPacket.msg.data[1] == 0xff)
	{
		std::cout << "Sent Password:  > CorrectPassword " << testPassword.c_str() << std::endl; 
		returnCode = 0;
	}
	else if(respPacket.msg.data[0] == 0x02)
	{
		std::cout << "Bad MD5" << std::endl;
		returnCode = 3;
	}
	return returnCode;

}
/*int main(int argc, const char* argv[])
{
	PODLClient cli("127.0.0.1", 10000);
	cli.RunTests();

	return 1;
}*/