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
    struct timeval tv;
    tv.tv_sec = 60;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

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
int PODLClient::RecvPacket(char* resbuffer)
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
			
		PODLPacket packet;
		isInvalidPacket = packet.Deserialize(resbuffer);
		std::cout << "PODLClient: Response Received: " << packet << std::endl;
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

/**
 * PODLClient::Run() 
 * 
 * @return {int}  : 
 */
int PODLClient::RunTests()
{
    //byte buffer
	std::vector<char> buffer(PODL_MAX_SIZE);
	std::vector<char> resbuffer(PODL_MAX_SIZE);

	PODLPacket goodPacket;
    goodPacket.msg.id = 28;
    
    unsigned char checksum[MD5_DIGEST_LENGTH];

    //Correct Password Case
    string stringData = "password";
    goodPacket.msg.length = stringData.length();
    int packetSize = goodPacket.msg.length + PODL_MIN_SIZE;

    std::copy(stringData.c_str(), stringData.c_str() + stringData.length(), goodPacket.msg.data);
    MD5((unsigned char*)&goodPacket.msg, packetSize, (unsigned char*)&goodPacket.checksum);

	goodPacket.Serialize(buffer.data());
	SendPacket(buffer.data(), buffer.size());

	//Clear Response buffer
	resbuffer.assign(PODL_MAX_SIZE,0);
	RecvPacket(resbuffer.data());

    //wrong password case
	stringData = "pas2rd";
    goodPacket.msg.length = stringData.length();
    packetSize = goodPacket.msg.length + PODL_MIN_SIZE;

    std::copy(stringData.c_str(), stringData.c_str() + stringData.length(), goodPacket.msg.data);
    MD5((unsigned char*)&goodPacket, packetSize, (unsigned char*)&goodPacket.checksum);

	goodPacket.Serialize(buffer.data());
	SendPacket(buffer.data(), buffer.size());

	//Clear Buffer
	buffer.assign(PODL_MAX_SIZE,0);
	RecvPacket(resbuffer.data());

    //wrong checksum case, use previous checksum
    stringData = "password";
    goodPacket.msg.length = stringData.length();
    packetSize = goodPacket.msg.length + PODL_MIN_SIZE;

    std::copy(stringData.c_str(), stringData.c_str() + stringData.length(), goodPacket.msg.data);
    MD5((unsigned char*)&goodPacket, packetSize, (unsigned char*)&goodPacket.checksum);

	goodPacket.Serialize(buffer.data());
	SendPacket(buffer.data(), buffer.size());

	resbuffer.assign(PODL_MAX_SIZE,0);
	RecvPacket(resbuffer.data());

    //wrong passwordlen
    stringData = "password";
    goodPacket.msg.length = stringData.length() + 30;
    packetSize = goodPacket.msg.length + PODL_MIN_SIZE;

    std::copy(stringData.c_str(), stringData.c_str() + stringData.length(), goodPacket.msg.data);
    MD5((unsigned char*)&goodPacket, packetSize, (unsigned char*)&goodPacket.checksum);

	goodPacket.Serialize(buffer.data());
	SendPacket(buffer.data(), buffer.size());

	resbuffer.assign(PODL_MAX_SIZE,0);
	RecvPacket(resbuffer.data());
	return 0;
}


/** 
 * For challenge 2
 * PODLClient::Run() 
 * 
 * @return {int}  : 
 */
int PODLClient::RunTests()
{
    //byte buffer
	std::vector<char> buffer(PODL_MAX_SIZE);
	std::vector<char> resbuffer(PODL_MAX_SIZE);

	PODLPacket goodPacket;
    goodPacket.msg.id = 28;
    
    unsigned char checksum[MD5_DIGEST_LENGTH];

    //Correct Password Case
    string stringData = "password";
    goodPacket.msg.length = stringData.length();
    int packetSize = goodPacket.msg.length + PODL_MIN_SIZE;

    std::copy(stringData.c_str(), stringData.c_str() + stringData.length(), goodPacket.msg.data);
    MD5((unsigned char*)&goodPacket.msg, packetSize, (unsigned char*)&goodPacket.checksum);

	goodPacket.Serialize(buffer.data());
	SendPacket(buffer.data(), buffer.size());

	//Clear Response buffer
	resbuffer.assign(PODL_MAX_SIZE,0);
	RecvPacket(resbuffer.data());

    //wrong password case
	stringData = "pas2rd";
    goodPacket.msg.length = stringData.length();
    packetSize = goodPacket.msg.length + PODL_MIN_SIZE;

    std::copy(stringData.c_str(), stringData.c_str() + stringData.length(), goodPacket.msg.data);
    MD5((unsigned char*)&goodPacket, packetSize, (unsigned char*)&goodPacket.checksum);

	goodPacket.Serialize(buffer.data());
	SendPacket(buffer.data(), buffer.size());

	//Clear Buffer
	buffer.assign(PODL_MAX_SIZE,0);
	RecvPacket(resbuffer.data());

    //wrong checksum case, use previous checksum
    stringData = "password";
    goodPacket.msg.length = stringData.length();
    packetSize = goodPacket.msg.length + PODL_MIN_SIZE;

    std::copy(stringData.c_str(), stringData.c_str() + stringData.length(), goodPacket.msg.data);
    MD5((unsigned char*)&goodPacket, packetSize, (unsigned char*)&goodPacket.checksum);

	goodPacket.Serialize(buffer.data());
	SendPacket(buffer.data(), buffer.size());

	resbuffer.assign(PODL_MAX_SIZE,0);
	RecvPacket(resbuffer.data());

    //wrong passwordlen
    stringData = "password";
    goodPacket.msg.length = stringData.length() + 30;
    packetSize = goodPacket.msg.length + PODL_MIN_SIZE;

    std::copy(stringData.c_str(), stringData.c_str() + stringData.length(), goodPacket.msg.data);
    MD5((unsigned char*)&goodPacket, packetSize, (unsigned char*)&goodPacket.checksum);

	goodPacket.Serialize(buffer.data());
	SendPacket(buffer.data(), buffer.size());

	resbuffer.assign(PODL_MAX_SIZE,0);
	RecvPacket(resbuffer.data());
	return 0;
}


/*int main(int argc, const char* argv[])
{
	PODLClient cli("127.0.0.1", 10000);
	cli.RunTests();

	return 1;
}*/