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
string testBuffer;

TEST_METHOD(SimpleSockTCP, Listen1)
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

TEST_METHOD(SimpleSockTCP, GetAddress)
{
    string adr;

    adr = sockTCPclient.GetAddress();
    assert("127.0.0.1"==adr);
    return true;
}

TEST_METHOD(SimpleSockTCP, Send)
{
    ostringstream oss;
    int i;

    for(i=0; i<2500; i++)
        oss << ('A'+i%10);
    testBuffer = oss.str();
    sockTCPclient.Send(testBuffer);
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
    assert(testBuffer==buffer);
    return true;
}

#define SSTCP_SIZE 2000
TEST_METHOD(SimpleSockTCP, Recv2)
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

TEST_METHOD(SimpleSockTCP, Close)
{
    sockTCPAccept.Close();
    sockTCPclient.Close();
    sockTCPserveur.Close();
    return true;
}

TEST_METHOD(SimpleSockTCP, Listen2)
{
    sockTCPserveur.Listen(4639, "127.0.0.1");
    sockTCPserveur.Close();
    return true;
}

TEST_METHOD(SimpleSockTCP, Exception)
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

TEST_METHOD(SimpleSockUDP, Open1)
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

TEST_METHOD(SimpleSockUDP, Listen1)
{
    sockUDPrecepteur.Listen(4639, "127.0.0.1");
    sockUDPrecepteur.Blocking(false);
    return true;
}

TEST_METHOD(SimpleSockUDP, Blocking)
{
    sockUDPemetteur.Blocking(true);
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

TEST_METHOD(SimpleSockUDP, SetNetworkInterface)
{
    sockUDPrecepteur.SetNetworkInterface("127.0.0.1");
    return true;
}

TEST_METHOD(SimpleSockUDP, Open2)
{
    sockUDPemetteur.Open(4639);
    sockUDPemetteur.Close();
    return true;
}

TEST_METHOD(SimpleSockUDP, Open3)
{
    sockUDPemetteur.Open(4639, inet_addr("127.0.0.1"));
    sockUDPemetteur.Close();
    return true;
}

/*
TEST_METHOD(SimpleSockUDP, Listen2)
{
    sockUDPrecepteur.Listen(4639);
    sockUDPrecepteur.Close();
    return true;
}
*/

TEST_METHOD(SimpleSockUDP, Listen3)
{
    sockUDPrecepteur.Listen(4639, inet_addr("127.0.0.1"));
    assert("127.0.0.1"==sockUDPemetteur.LocalAddress("127.0.0.1"));
    sockUDPrecepteur.Close();
    return true;
}

int main()
{
    UnitTest test;
    return test.Run();
}
