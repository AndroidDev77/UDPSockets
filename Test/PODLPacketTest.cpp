#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

#include <algorithm>
#include "openssl/md5.h"
#include "../PODLPacket.h"

TEST_GROUP(PODLPacket)
{
  PODLPacket* packet;
  std::string msgDataString;

  void setup()
  {
    packet = new PODLPacket();
    (*packet).msg.id = 28;
    (*packet).msg.length = 9;
    msgDataString = "password";
    std::copy(msgDataString.c_str(), msgDataString.c_str() + (*packet).msg.length, (*packet).msg.data);

  }
  void teardown()
  {
    delete packet;
  }
};

TEST(PODLPacket, SerializeTest)
{

    std::vector<char> buffer(80);
    (*packet).Serialize(buffer.data());
    
    PODLPacket testPacket(buffer.data());
    CHECK_EQUAL_TEXT(28, testPacket.msg.id, "ID Invalid");
    CHECK_EQUAL_TEXT(9, testPacket.msg.length, "Msg Length Invalid");
    STRCMP_EQUAL_TEXT(msgDataString.c_str(), testPacket.msg.data, "Password Invalid");

}

TEST(PODLPacket, MD5Test)
{

    int msgSize = (*packet).msg.length + PODL_MIN_SIZE;
    MD5((unsigned char*)packet, msgSize, (*packet).checksum);
    char mdString1[33];

    std::string mdString2 = "e4f58df12fcc1d8d63289ff35f45b1e7";

    for(int i = 0; i < 16; i++)
    {
        sprintf(&mdString1[i*2], "%02x", (unsigned int)(*packet).checksum[i]);
    }
    
    CHECK_EQUAL(mdString1, mdString2);

}

int main(int ac, char** av)
{
    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
  return RUN_ALL_TESTS(ac, av);
}