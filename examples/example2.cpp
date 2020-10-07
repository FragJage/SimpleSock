/***********************************************************************************************/
/**                                                                                           **/
/** EXAMPLE 2                                                                                 **/
/**                                                                                           **/
/** Emitter and receiver with TCP socket                                                      **/
/**                                                                                           **/
/***********************************************************************************************/

#include <iostream>
#include "SimpleSockTCP.h"

using namespace std;

int main()
{
    string bufSrv;
    string bufCli;

    SimpleSockTCP sockTCPclient;
    SimpleSockTCP sockTCPserver;
    SimpleSockTCP sockTCPaccept;


    try
    {
        sockTCPserver.Listen(5750);
        sockTCPserver.Blocking(false);
        sockTCPclient.Connect("127.0.0.1", 5750);
        sockTCPclient.Blocking(false);
    }
    catch(const char* e)
    {
        cout << "Argh ! Unable to create TCP socket : " << e << endl;
        return 1;
    }

    cout << "Client emit : TOTO-test" << endl;
    sockTCPclient.Send("TOTO-test");

    cout << "Server wait for receiving" << endl;
    if(!sockTCPserver.WaitRecv(10))
    {
        cout << "Argh ! No reception on server." << endl;
        return 1;
    }

    cout << "Server accept connection" << endl;
    if(!sockTCPserver.Accept(&sockTCPaccept))
    {
        cout << "Argh ! Fail to accept reception." << endl;
        return 1;
    }

    cout << "Server receiving" << endl;
    sockTCPaccept.Blocking(false);
    sockTCPaccept.Recv(bufSrv);
    cout << "Server receive : " << bufSrv << endl;

    cout << "Server emit : TOTO-OkiOki" << endl;
    sockTCPaccept.Send("TOTO-OkiOki");

    if(!sockTCPclient.WaitRecv(10))
    {
        cout << "Argh ! No reception on client." << endl;
        return 1;
    }

    try
    {
        sockTCPclient.Recv(bufCli);
    }
    catch(const char* e)
    {
        cout << "Argh ! Unable to receive data : " << e << endl;
        return -1;
    }

    cout << "Client receive : " << bufCli << endl;

    sockTCPserver.Close();
    sockTCPclient.Close();
    sockTCPaccept.Close();
    cout << "All sockets are closed" << endl;

    return 0;
}
