#include <arpa/inet.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>

#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>

#include "openssl/md5.h"

//header, id, length size
#define PODL_MIN_SIZE 9
//header, id, length, and checksum
#define PODL_CS_MIN_SIZE PODL_MIN_SIZE + 16
#define PODL_MAX_PASSWORD_LEN 255
#define PODL_MAX_SIZE PODL_MAX_PASSWORD_LEN + PODL_CS_MIN_SIZE


#ifndef PODLPACKET

class PODLPacket
{
public:

    #pragma pack(push, 1)
    struct Data {
        char header[4];
        uint32_t id;
        uint8_t length;
        char data[255];
    };
	Data msg;
    #pragma pack(pop)
    unsigned char checksum[MD5_DIGEST_LENGTH];
	PODLPacket();
    ~PODLPacket();
	PODLPacket(char* indata);

    int Deserialize(char* inbuffer);
    int Serialize(char* outbuffer);
    
};

extern std::ostream& operator<<(std::ostream &strm, const PODLPacket &packet);



#define PODLPACKET
#endif