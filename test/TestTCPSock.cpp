#include <cassert>
#include "TestTCPSock.h"

using namespace std;

TestTCPSock::TestTCPSock() : TestClass("SimpleSockTCP", this)
{
	addTest("Listen1", &TestTCPSock::Listen1);
    addTest("Connect", &TestTCPSock::Connect);
    addTest("Blocking", &TestTCPSock::Blocking);
    addTest("GetAddress", &TestTCPSock::GetAddress);
    addTest("Send", &TestTCPSock::Send);
    addTest("WaitRecv", &TestTCPSock::WaitRecv);
    addTest("Accept", &TestTCPSock::Accept);
    addTest("Recv", &TestTCPSock::Recv);
    addTest("Recv2", &TestTCPSock::Recv2);
    addTest("Close", &TestTCPSock::Close);
    addTest("Listen2", &TestTCPSock::Listen2);
    addTest("Exception", &TestTCPSock::TestException);
}

TestTCPSock::~TestTCPSock()
{
}

bool TestTCPSock::Listen1()
{
    sockTCPserveur.Listen(4639);
    return true;
}

bool TestTCPSock::Connect()
{
    sockTCPclient.Connect("127.0.0.1", 4639);
    return true;
}

bool TestTCPSock::Blocking()
{
    sockTCPserveur.Blocking(false);
    sockTCPclient.Blocking(false);
    return true;
}

bool TestTCPSock::GetAddress()
{
    string adr;

    adr = sockTCPclient.GetAddress();
    assert("127.0.0.1"==adr);
    return true;
}

bool TestTCPSock::Send()
{
    ostringstream oss;
    int i;

    for(i=0; i<2500; i++)
        oss << ('A'+i%10);
    testBuffer = oss.str();
    sockTCPclient.Send(testBuffer);
    return true;
}

bool TestTCPSock::WaitRecv()
{
    sockTCPserveur.WaitRecv(10000);
    return true;
}

bool TestTCPSock::Accept()
{
    sockTCPserveur.Accept(&sockTCPAccept);
    return true;
}

bool TestTCPSock::Recv()
{
    string buffer;

    sockTCPAccept.Blocking(false);
    sockTCPAccept.Recv(buffer);
    assert(testBuffer==buffer);
    return true;
}

#define SSTCP_SIZE 2000
bool TestTCPSock::Recv2()
{
    unsigned int i;
    char sendBuffer[SSTCP_SIZE];
    char recvBuffer[SSTCP_SIZE*2];


    for(i=0; i<SSTCP_SIZE; i++)
        sendBuffer[i] = 65+(i%10);

    sockTCPclient.Send(sendBuffer, SSTCP_SIZE);
    sockTCPclient.Send(sendBuffer, SSTCP_SIZE);
    sockTCPAccept.WaitRecv(10000);
    sockTCPAccept.Recv(recvBuffer, SSTCP_SIZE*2);

    for(i=0; i<sizeof(sendBuffer); i++)
    {
        assert(sendBuffer[i]==recvBuffer[i]);
        assert(sendBuffer[i]==recvBuffer[SSTCP_SIZE+i]);
    }

    return true;
}

bool TestTCPSock::Close()
{
    sockTCPAccept.Close();
    sockTCPclient.Close();
    sockTCPserveur.Close();
    return true;
}

bool TestTCPSock::Listen2()
{
    sockTCPserveur.Listen(4639, "127.0.0.1");
    sockTCPserveur.Close();
    return true;
}

bool TestTCPSock::TestException()
{
    try
    {
        sockTCPclient.Connect("255.0.0.1", 4639);
        assert(false);
    }
    catch(SimpleSockTCP::Exception &e)
    {
        e.what();
        assert(97==e.GetNumber());
    }
    return true;
}
