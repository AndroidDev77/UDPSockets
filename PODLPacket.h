#include <arpa/inet.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "openssl/md5.h"

#pragma pack(push, 1)
class PODLPacket
{
public:
	struct Data {

        char header[4];
        uint32_t id;
        uint8_t length;
        char data[255];

        unsigned char checksum[MD5_DIGEST_LENGTH];
	};
	Data data;
	PODLPacket();
	PODLPacket(char* indata);

};
#pragma pack(pop)

PODLPacket::PODLPacket(char* indata)
{
    //PODLPacket::Data* sdata = (PODLPacket::Data*)indata;
    // todo remove copy in favor of buffercast
    memcpy(&data.header, indata, 4);
    indata += 4;
    memcpy(&data.id, indata, sizeof(Data::id));
    indata += sizeof(Data::id);
    
    memcpy(&data.length, indata, sizeof(Data::length));
    indata += sizeof(Data::length);

    //swap network to host
	data.id = be32toh(data.id);
    memcpy(data.data, indata, data.length);

    indata += data.length;
    memcpy(&data.checksum, indata, sizeof(Data::checksum));

}

PODLPacket::PODLPacket()
{
    memcpy(data.header,"PODL",4);
}

std::ostream& operator<<(std::ostream &strm, const PODLPacket &packet) {
  return strm << "PODL(" << packet.data.header << "," << packet.data.length << ","
	      << packet.data.data << "," 
	       << ")\n";
}

