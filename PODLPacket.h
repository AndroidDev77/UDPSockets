#include <arpa/inet.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>

#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>

#include "openssl/md5.h"

#define PODL_MIN_SIZE 9
#define PODL_CS_MIN_SIZE PODL_MIN_SIZE + 16
#define PODL_MAX_SIZE 280

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

    void Deserialize(char* inbuffer);
    int Serialize(char* outbuffer);
};

extern std::ostream& operator<<(std::ostream &strm, const PODLPacket &packet);

#define PODLPACKET
#endif