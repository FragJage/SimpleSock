#include "UnitTest.h"
#include "SimpleSockUDP.h"

class TestUDPSock : public TestClass<TestUDPSock>
{
public:
    TestUDPSock();
    ~TestUDPSock();
    bool Open1();
    bool GetPort();
    bool Listen1();
    bool Blocking();
    bool Send();
    bool WaitRecv();
    bool Recv1();
    bool Recv2();
    bool Close();
    bool SetNetworkInterface();
    bool Open2();
    bool Open3();
    bool Listen3();
    bool TestException();

private:
    SimpleSockUDP sockUDPemetteur;
    SimpleSockUDP sockUDPrecepteur;
    std::string testBuffer;
};
