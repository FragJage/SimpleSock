#include "SimpleSockTCP.h"
#include "UnitTest.h"

class TestTCPSock : public TestClass<TestTCPSock>
{
public:
    TestTCPSock();
    ~TestTCPSock();
    bool Listen1();
    bool Connect();
    bool Blocking();
    bool GetAddress();
    bool Send();
    bool WaitRecv();
    bool Accept();
    bool Recv();
    bool Recv2();
    bool Close();
    bool Listen2();
    bool TestException();

private:
    SimpleSockTCP sockTCPclient;
    SimpleSockTCP sockTCPserveur;
    SimpleSockTCP sockTCPAccept;
    std::string testBuffer;
};
