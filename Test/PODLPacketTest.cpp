#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

#include <algorithm>
#include "../PODLPacket.h"

TEST_GROUP(PODLPacket)
{
  PODLPacket* packet;

  void setup()
  {
    packet = new PODLPacket();
    (*packet).msg.id = 28;
    (*packet).msg.length = 9;
    std::string stringData = "password";
    std::copy(stringData.c_str(), stringData.c_str() + (*packet).msg.length, (*packet).msg.data);

  }
  void teardown()
  {
    delete packet;
  }
};

TEST(PODLPacket, FirstTest)
{

    char buffer[80];
    int size = (*packet).Serialize(buffer);
    
    PODLPacket testPacket(buffer);
    CHECK_EQUAL(28, testPacket.msg.id);
    CHECK_EQUAL(9, testPacket.msg.length);

    //
    
}

int main(int ac, char** av)
{
  return RUN_ALL_TESTS(ac, av);
}