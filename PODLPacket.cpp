#include "PODLPacket.h"

PODLPacket::PODLPacket()
{
    memcpy(msg.header,std::string("PODL").c_str(),4);
}

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
	//msg.id = be32toh(msg.id);
    std::copy(indata, indata + msg.length, msg.data);

    indata += msg.length;
    memcpy(&checksum, indata, sizeof(checksum));

}

std::ostream& operator<<(std::ostream &strm, const PODLPacket &packet) {
    char mdString[33];
     
    for(int i = 0; i < packet.msg.length; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)packet.checksum[i]);

    return strm << "PODL(" << packet.msg.header << "," << packet.msg.id 
	            << "," <<  (int)packet.msg.length << "," <<
                std::string(reinterpret_cast<const char*>(&packet.msg.data [0]),
                packet.msg.length) << "," << mdString << ")\n";
}

