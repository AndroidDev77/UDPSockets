#include "PODLPacket.h"

/**
 * PODLPacket::PODLPacket 
 * 
 */
PODLPacket::PODLPacket()
{
    memcpy(msg.header,std::string("PODL").c_str(),4);
    msg.id = 0;
    msg.length = 0;
    checksum[16] = {0};
}

/**
 * PODLPacket::~PODLPacket 
 *  
 */
PODLPacket::~PODLPacket()
{
    
}


/**
 * PODLPacket::PODLPacket 
 * 
 * @param  {char*} indata : 
 */
PODLPacket::PODLPacket(char* indata)
{
    //PODLPacket::Data* sdata = (PODLPacket::Data*)indata;
    Deserialize(indata);
}

/**
 * PODLPacket::Deserialize
 * 
 * @param  {char*} indata : 
 */
void PODLPacket::Deserialize(char* inbuffer)
{
    //PODLPacket::Data* sdata = (PODLPacket::Data*)indata;
    // todo remove copy in favor of buffercast
    memcpy(&msg.header, inbuffer, 4);
    inbuffer += 4;
    memcpy(&msg.id, inbuffer, sizeof(Data::id));
    inbuffer += sizeof(Data::id);
    
    memcpy(&msg.length, inbuffer, sizeof(Data::length));
    inbuffer += sizeof(Data::length);

    //swap network to host
	//msg.id = be32toh(msg.id);
    std::copy(inbuffer, inbuffer + msg.length, msg.data);

    inbuffer += msg.length;
    memcpy(&checksum, inbuffer, sizeof(checksum));

}

/**
 * PODLPacket ::Serialize
 * 
 * @param  {char*} outbuffer : 
 * @return {int}             : #of Bytes written
 */
int PODLPacket::Serialize(char* outbuffer)
{
    int size = PODL_CS_MIN_SIZE + msg.length;
    
    memcpy(outbuffer, (char*)&msg, size);
    memcpy(outbuffer + size, (char*)checksum, MD5_DIGEST_LENGTH);

    return size + MD5_DIGEST_LENGTH;

}
/**
 *  Print Packet
 * @param  {std::ostream} strm : 
 * @param  {PODLPacket} packet : 
 * @return {std::ostream}      : 
 */
std::ostream& operator<<(std::ostream &strm, const PODLPacket &packet) {
    char mdString[33];
     
    for(int i = 0; i < packet.msg.length; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)packet.checksum[i]);

    return strm << "PODL(" << packet.msg.header << "," << packet.msg.id 
	            << "," <<  (int)packet.msg.length << "," <<
                std::string(reinterpret_cast<const char*>(&packet.msg.data [0]),
                packet.msg.length) << "," << mdString << ")\n";
}

