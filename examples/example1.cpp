/***********************************************************************************************/
/**                                                                                           **/
/** EXAMPLE 1                                                                                 **/
/**                                                                                           **/
/** Emitter and receiver with UDP socket                                                      **/
/**                                                                                           **/
/***********************************************************************************************/

#include <iostream>
#include "SimpleSock.h"

using namespace std;

int main()
{
    string buffer;

    SimpleSockUDP sockUDPemetteur;
    SimpleSockUDP sockUDPrecepteur;

    try
    {
        sockUDPemetteur.Open(4639, "127.0.0.1");
        sockUDPrecepteur.Listen(4639, "127.0.0.1");
        sockUDPrecepteur.Blocking(false);
    }
    catch(const char* e)
    {
        cout << "Argh ! Unable to create UDP socket : " << e << endl;
    }

    cout << "Emit : TOTO-test" << endl;
    sockUDPemetteur.Send("TOTO-test");
    sockUDPrecepteur.Recv(buffer);
    cout << "Receive : " << buffer << endl;

    sockUDPrecepteur.Close();
    sockUDPemetteur.Close();
    cout << "All sockets are closed" << endl;

    return 0;
}
