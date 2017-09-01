#include <cassert>
#include "TestUDPSock.h"

using namespace std;

TestUDPSock::TestUDPSock() : TestClass("SimpleSockUDP", this)
{
	addTest("Open1", &TestUDPSock::Open1);
    addTest("GetPort", &TestUDPSock::GetPort);
	addTest("Listen1", &TestUDPSock::Listen1);
	addTest("Blocking", &TestUDPSock::Blocking);
    addTest("Send", &TestUDPSock::Send);
	addTest("WaitRecv", &TestUDPSock::WaitRecv);
	addTest("Recv1", &TestUDPSock::Recv1);
	addTest("Recv2", &TestUDPSock::Recv2);
	addTest("Close", &TestUDPSock::Close);
	addTest("SetNetworkInterface", &TestUDPSock::SetNetworkInterface);
	addTest("Open2", &TestUDPSock::Open2);
	addTest("Open3", &TestUDPSock::Open3);
	addTest("Listen3", &TestUDPSock::Listen3);
	addTest("TestException", &TestUDPSock::TestException);
}

TestUDPSock::~TestUDPSock()
{
}

bool TestUDPSock::Open1()
{
    sockUDPemetteur.Open(4639, "127.0.0.1");
    return true;
}

bool TestUDPSock::GetPort()
{
    int port = 0;

    port = sockUDPemetteur.GetPort();
    assert(4639==port);
    return true;
}

bool TestUDPSock::Listen1()
{
    sockUDPrecepteur.Listen(4639, "127.0.0.1");
    sockUDPrecepteur.Blocking(false);
    return true;
}

bool TestUDPSock::Blocking()
{
    sockUDPemetteur.Blocking(true);
    return true;
}

bool TestUDPSock::Send()
{
    testBuffer = "TOTO-test";
    sockUDPemetteur.Send(testBuffer);
    return true;
}

bool TestUDPSock::WaitRecv()
{
    sockUDPrecepteur.WaitRecv(10000);
    return true;
}

bool TestUDPSock::Recv1()
{
    string buffer;

    sockUDPrecepteur.Recv(buffer);
    assert(testBuffer==buffer);
    return true;
}

bool TestUDPSock::Recv2()
{
    char bufferCh[16];

    memset(bufferCh, 0, 16);
    sockUDPemetteur.Send("TestWithChar");
    sockUDPrecepteur.WaitRecv(10000);
    sockUDPrecepteur.Recv(bufferCh,16);
    string bufStr(bufferCh);
    assert("TestWithChar"==bufStr);

    sockUDPemetteur.Send("TestPartOne");
    sockUDPemetteur.Send("TestPartTwo");
    sockUDPrecepteur.WaitRecv(10000);
    sockUDPrecepteur.Recv(bufStr,11);
    assert("TestPartOne"==bufStr);
    sockUDPrecepteur.Recv(bufStr);
    assert("TestPartTwo"==bufStr);

    return true;
}

bool TestUDPSock::Close()
{
    sockUDPrecepteur.Close();
    sockUDPemetteur.Close();
    return true;
}

bool TestUDPSock::SetNetworkInterface()
{
    sockUDPrecepteur.SetNetworkInterface("127.0.0.1");
    return true;
}

bool TestUDPSock::Open2()
{
    sockUDPemetteur.Open(4639);
    sockUDPemetteur.Close();
    return true;
}

bool TestUDPSock::Open3()
{
    sockUDPemetteur.Open(4639, inet_addr("127.0.0.1"));
    sockUDPemetteur.Close();
    return true;
}

/*
bool TestUDPSock::Listen2()
{
    sockUDPrecepteur.Listen(4639);
    sockUDPrecepteur.Close();
    return true;
}
*/

bool TestUDPSock::Listen3()
{
    sockUDPrecepteur.Listen(4639, inet_addr("127.0.0.1"));
    assert("127.0.0.1"==sockUDPemetteur.LocalAddress("127.0.0.1"));
    sockUDPrecepteur.Close();
    return true;
}

bool TestUDPSock::TestException()
{
    SimpleSockUDP sock2;
    string buf;

    sockUDPrecepteur.Listen(4639, inet_addr("127.0.0.1"));
    try
    {
        sock2.Listen(4639, inet_addr("127.0.0.1"));
        assert(false);
    }
    catch(SimpleSockUDP::Exception &e)
    {

        assert(0x25==e.GetNumber());
    }

    try
    {
        sockUDPrecepteur.Recv(buf, -2);
        assert(false);
    }
    catch(SimpleSockUDP::Exception &e)
    {
        assert(0x54==e.GetNumber());
    }

    sockUDPrecepteur.Close();
    sockUDPrecepteur.Blocking(false);   //Increase code coverage

    return true;
}
