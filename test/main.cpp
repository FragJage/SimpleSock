#include <iostream>
#include "SimpleSock.h"

using namespace std;

#define COULEUR_VERTE 1
#define COULEUR_ROUGE 2
#define COULEUR_INVERSE 3
#define COULEUR_NORMALE 4

#ifdef WIN32
#define OSSEP "\\"
#endif // WIN

#ifdef __linux__
#define OSSEP "/"
#endif // LINUX

#ifdef WIN32
#include <Windows.h>
void EcrireEnCouleur(int couleur, string msg)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    switch(couleur)
    {
        case COULEUR_VERTE  :
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        case COULEUR_ROUGE  :
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
            break;
        case COULEUR_INVERSE:
            SetConsoleTextAttribute(consoleHandle, BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED);
            break;
        case COULEUR_NORMALE:
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
            break;
    }
    cout << msg;
}
#endif // WIN

#ifdef __linux__
void EcrireEnCouleur(int couleur, string msg)
{
    switch(couleur)
    {
        case COULEUR_VERTE  :
            printf("\033[32m");
            break;
        case COULEUR_ROUGE  :
            printf("\033[31m");
            break;
        case COULEUR_INVERSE:
            printf("\033[30;47m");
            break;
        case COULEUR_NORMALE:
            printf("\033[0m");
            break;
    }
    cout << msg;
}
#endif // LINUX

void Titre(string myTitre)
{
    string complete(60, '°');

    cout << endl;
    complete = complete.substr(0, 60-myTitre.length());
    EcrireEnCouleur(COULEUR_INVERSE, "°°° ");
    EcrireEnCouleur(COULEUR_INVERSE, myTitre);
    EcrireEnCouleur(COULEUR_INVERSE, " ");
    EcrireEnCouleur(COULEUR_INVERSE, complete);
    EcrireEnCouleur(COULEUR_NORMALE, "");
    cout << endl;
}

void Resultat(string myTest, bool myResult)
{
    cout << " - " << myTest << " : ";

    if(myResult)
        EcrireEnCouleur(COULEUR_VERTE, "OK :-)");
    else
        EcrireEnCouleur(COULEUR_ROUGE, "KO :-(");
    EcrireEnCouleur(COULEUR_NORMALE, "");
    cout << endl;
}

void testUnitaireTCP()
{
    string buffer;
    bool bOk;
    SimpleSockTCP sockTCPclient;
    SimpleSockTCP sockTCPserveur;
    SimpleSockTCP sockTCPAccept;

    Titre("Classe SimpleSockTCP");

    bOk = true;
    try
    {
        sockTCPserveur.Listen(4639);
    }
    catch(const char* e)
    {
        bOk = false;
    }
    Resultat("Test de la methode Listen", bOk);

    bOk = true;
    try
    {
        sockTCPclient.Connect("127.0.0.1", 4639);
    }
    catch(const char* e)
    {
        bOk = false;
    }
    Resultat("Test de la methode Connect", bOk);

    bOk = true;
    try
    {
        sockTCPserveur.Blocking(false);
        sockTCPclient.Blocking(false);
    }
    catch(const char* e)
    {
        bOk = false;
    }
    Resultat("Test de la methode Blocking", bOk);

    bOk = true;
    try
    {
        sockTCPclient.Send("TOTO-Test");
    }
    catch(const char* e)
    {
        bOk = false;
    }
    Resultat("Test de la methode Send", bOk);

    bOk = true;
    try
    {
        sockTCPserveur.WaitRecv(10000);
    }
    catch(const char* e)
    {
        bOk = false;
    }
    Resultat("Test de la methode WaitRecv", bOk);

    bOk = true;
    try
    {
        sockTCPserveur.Accept(&sockTCPAccept);
    }
    catch(const char* e)
    {
        bOk = false;
    }
    Resultat("Test de la methode Accept", bOk);

    bOk = true;
    try
    {
        sockTCPAccept.Blocking(false);
        sockTCPAccept.Recv(buffer);
    }
    catch(const char* e)
    {
        bOk = false;
    }
    if(buffer!="TOTO-Test") bOk = false;
    Resultat("Test de la methode Recv", bOk);

    bOk = true;
    try
    {
        sockTCPclient.Close();
        sockTCPserveur.Close();
        sockTCPAccept.Close();
    }
    catch(const char* e)
    {
        bOk = false;
    }
    Resultat("Test de la methode Close", bOk);

}

void testUnitaireUDP()
{
    string buffer;
    bool bOk;
    SimpleSockUDP sockUDPemetteur;
    SimpleSockUDP sockUDPrecepteur;
    int port;

    Titre("Classe SimpleSockUDP");

    bOk = true;
    try
    {
        sockUDPemetteur.Open(4639, "127.0.0.1");
    }
    catch(const char* e)
    {
        bOk = false;
    }
    Resultat("Test de la methode Open", bOk);

    bOk = true;
    port = 0;
    try
    {
        port = sockUDPemetteur.GetPort();
    }
    catch(const char* e)
    {
        bOk = false;
    }
    if(port!=4639) bOk = false;
    Resultat("Test de la methode GetPort", bOk);

    bOk = true;
    try
    {
        sockUDPrecepteur.Listen(4639, "127.0.0.1");
    }
    catch(const char* e)
    {
        bOk = false;
    }
    Resultat("Test de la methode Listen", bOk);

    bOk = true;
    try
    {
        sockUDPrecepteur.Blocking(false);
    }
    catch(const char* e)
    {
        bOk = false;
    }
    Resultat("Test de la methode Blocking", bOk);

    bOk = true;
    try
    {
        sockUDPemetteur.Send("TOTO-test");
    }
    catch(const char* e)
    {
        bOk = false;
    }
    Resultat("Test de la methode Send", bOk);

    bOk = true;
    try
    {
        sockUDPrecepteur.Recv(buffer);
    }
    catch(const char* e)
    {
        bOk = false;
    }
    if(buffer!="TOTO-test") bOk = false;
    Resultat("Test de la methode Recv", bOk);

    bOk = true;
    try
    {
        sockUDPrecepteur.Close();
        sockUDPemetteur.Close();
    }
    catch(const char* e)
    {
        bOk = false;
    }
    Resultat("Test de la methode Close", bOk);
}

int main()
{
    string buffer;

    testUnitaireUDP();
    testUnitaireTCP();

    cin >> buffer;

    return 0;
}
