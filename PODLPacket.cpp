#include "PODLPacket.h"


class PODLException: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Error Parsing PODL Packet: Password Length too long";
  }
} podlLengthException;

/**
 * PODLPacket::PODLPacket 
 * 
 */
PODLPacket::PODLPacket()
{
    memcpy(msg.header,std::string("PODL").c_str(),4);
    msg.id = 0;
    msg.length = 0;
    msg.data[255] = {0};
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
int PODLPacket::Deserialize(char* inbuffer)
{
    //Keep track of memory location
    char* inbufferIt = inbuffer;
    try{
        //set header, id, length
        std::copy(inbuffer, inbuffer + PODL_MIN_SIZE, reinterpret_cast<char*>(&msg));
        inbufferIt += PODL_MIN_SIZE;
        //check password length 
        if(msg.length > PODL_MAX_PASSWORD_LEN)
        {
            throw podlLengthException;
        }
        //set password
        std::copy(inbufferIt, inbufferIt + msg.length, msg.data);
        inbufferIt += msg.length;
        std::copy(inbufferIt, inbufferIt+MD5_DIGEST_LENGTH, checksum);
        //swap network to host
        msg.id = be32toh(msg.id);
    }
    catch(std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

/**
 * PODLPacket ::Serialize
 * 
 * @param  {char*} outbuffer : 
 * @return {int}             : #of Bytes written
 */
int PODLPacket::Serialize(char* outbuffer)
{
    //Do not serialize of length is too long
    try{
    if(msg.length > PODL_MAX_PASSWORD_LEN)
        throw podlLengthException;

    int size = PODL_MIN_SIZE + msg.length;
    msg.id = htobe32(msg.id); //hack to get network endinaness fix later
    std::copy(reinterpret_cast<char*>(&msg), reinterpret_cast<char*>(&msg) + size, outbuffer);
    std::copy(checksum, checksum + MD5_DIGEST_LENGTH, outbuffer + size); 
    msg.id = be32toh(msg.id);
    }
    catch(std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;

}
/**
 *  Print Packet
 * @param  {std::ostream} strm : 
 * @param  {PODLPacket} packet : 
 * @return {std::ostream}      : 
 */
std::ostream& operator<<(std::ostream &strm, const PODLPacket &packet) {
    char mdString[33];
     
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)packet.checksum[i]);

    return strm << packet.msg.header << "," << packet.msg.id 
	            << "," <<  (int)packet.msg.length << "," <<
                std::string(reinterpret_cast<const char*>(&packet.msg.data [0]),
                packet.msg.length) << "," << mdString << ")\n";
}

