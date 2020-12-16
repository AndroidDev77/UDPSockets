#include <arpa/inet.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>

#include <vector>
#include "openssl/md5.h"

#define PODL_MIN_SIZE 9
#define PODL_CS_MIN_SIZE PODL_MIN_SIZE + 16

class PODLPacket
{
public:

    #pragma pack(push, 1)
    struct Data {
        char header[4];
        uint32_t id;
        uint8_t length;
        std::vector<char> data;
        //char data[];
    };
	Data msg;
    #pragma pack(pop)

    unsigned char checksum[MD5_DIGEST_LENGTH];
	PODLPacket();
	PODLPacket(char* indata);
};


PODLPacket::PODLPacket(char* indata)
{
    //PODLPacket::Data* sdata = (PODLPacket::Data*)indata;
    // todo remove copy in favor of buffercast
    memcpy(&msg.header, indata, 4);
    indata += 4;
    memcpy(&msg.id, indata, sizeof(Data::id));
    indata += sizeof(Data::id);
    
    memcpy(&msg.length, indata, sizeof(Data::length));
    indata += sizeof(Data::length);

    //swap network to host
	msg.id = be32toh(msg.id);
    std::copy(indata, indata + msg.length, msg.data);

    indata += msg.length;
    memcpy(&checksum, indata, sizeof(checksum));

}

PODLPacket::PODLPacket()
{
    memcpy(msg.header,std::string("PODL").c_str(),4);
}

std::ostream& operator<<(std::ostream &strm, const PODLPacket &packet) {
    char mdString[33];
     
    for(auto i : packet.checksum)
        sprintf(&mdString[i*2], "%02x", (unsigned int)i);

    return strm << "PODL(" << packet.msg.header << "," << packet.msg.id << ","
	            << "," <<  packet.msg.length << "," <<
                std::string(reinterpret_cast<const char*>(&packet.msg.data [0]),
                packet.msg.data.size()) << "," << mdString << ")\n";
}

