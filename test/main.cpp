#include <iostream>
#include <cassert>
#include "SimpleSock.h"
#include "UnitTest/UnitTest.h"

using namespace std;


SimpleSockTCP sockTCPclient;
SimpleSockTCP sockTCPserveur;
SimpleSockTCP sockTCPAccept;
SimpleSockUDP sockUDPemetteur;
SimpleSockUDP sockUDPrecepteur;

TEST_METHOD(SimpleSockTCP, Listen)
{
    sockTCPserveur.Listen(4639);
    return true;
}

TEST_METHOD(SimpleSockTCP, Connect)
{
    sockTCPclient.Connect("127.0.0.1", 4639);
    return true;
}

TEST_METHOD(SimpleSockTCP, Blocking)
{
    sockTCPserveur.Blocking(false);
    sockTCPclient.Blocking(false);
    return true;
}

TEST_METHOD(SimpleSockTCP, Send)
{
    sockTCPclient.Send("TOTO-Test");
    return true;
}

TEST_METHOD(SimpleSockTCP, WaitRecv)
{
    sockTCPserveur.WaitRecv(10000);
    return true;
}

TEST_METHOD(SimpleSockTCP, Accept)
{
    sockTCPserveur.Accept(&sockTCPAccept);
    return true;
}

TEST_METHOD(SimpleSockTCP, Recv)
{
    string buffer;

    sockTCPAccept.Blocking(false);
    sockTCPAccept.Recv(buffer);
    assert("TOTO-Test"==buffer);
    return true;
}

TEST_METHOD(SimpleSockTCP, Close)
{
    sockTCPclient.Close();
    sockTCPserveur.Close();
    sockTCPAccept.Close();
    return true;
}

TEST_METHOD(SimpleSockUDP, Open)
{
    sockUDPemetteur.Open(4639, "127.0.0.1");
    return true;
}

TEST_METHOD(SimpleSockUDP, GetPort)
{
    int port = 0;

    port = sockUDPemetteur.GetPort();
    assert(4639==port);
    return true;
}

TEST_METHOD(SimpleSockUDP, Listen)
{
    sockUDPrecepteur.Listen(4639, "127.0.0.1");
    return true;
}

TEST_METHOD(SimpleSockUDP, Blocking)
{
    sockUDPrecepteur.Blocking(false);
    return true;
}

TEST_METHOD(SimpleSockUDP, Send)
{
    sockUDPemetteur.Send("TOTO-test");
    return true;
}

TEST_METHOD(SimpleSockUDP, WaitRecv)
{
    sockUDPrecepteur.WaitRecv(10000);
    return true;
}

TEST_METHOD(SimpleSockUDP, Recv)
{
    string buffer;

    sockUDPrecepteur.Recv(buffer);
    assert("TOTO-test"==buffer);
    return true;
}

TEST_METHOD(SimpleSockUDP, Close)
{
    sockUDPrecepteur.Close();
    sockUDPemetteur.Close();
    return true;
}

int main()
{
    UnitTest test;
    return test.Run();
}
