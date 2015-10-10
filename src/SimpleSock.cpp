/*** LICENCE ***************************************************************************************/
/*
  SimpleSock - Simple class to manage socket communication TCP or UDP

  This file is part of SimpleSock.

    SimpleSock is free software : you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SimpleSock is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SimpleSock.  If not, see <http://www.gnu.org/licenses/>.
*/
/***************************************************************************************************/
#include <iostream>
#include <exception>
#include "SimpleSock.h"
using namespace std;

/***************************************************************************************************/
/***                                                                                             ***/
/*** Class SimpleSock                                                                            ***/
/***                                                                                             ***/
/***************************************************************************************************/
bool SimpleSock::m_initSocket = false;

void SimpleSock::InitSocket()
{
    SimpleSock::m_initSocket = true;

    #ifdef WIN32
    WSADATA	WsaData;
    if (WSAStartup(MAKEWORD(2,0), &WsaData) != 0)
        throw SimpleSock::Exception(0x0011, "SimpleSock::InitSocket: WSAStartup error", GetSocketError());
    #endif // WIN32
}

SimpleSock::SimpleSock(int type)
{
    m_sockHandle = INVALID_SOCKET;
    if(!SimpleSock::m_initSocket) InitSocket();
    m_isOpen = false;
    m_sockType = type;
    m_blocking = false;
}

SimpleSock::~SimpleSock()
{
    Close();
}

void SimpleSock::SetSocket(SOCKET sockHandle, struct sockaddr_in sockAddress, int type)
{
    m_sockType = type;
    m_sockHandle = sockHandle;
    m_sockAddress = sockAddress;
    m_isOpen = true;
}

int SimpleSock::GetPort()
{
    return ntohs(m_sockAddress.sin_port);
}

string SimpleSock::GetAddress()
{
    return inet_ntoa(m_sockAddress.sin_addr);
}

void SimpleSock::Open(int port, unsigned long ipAddress)
{
    if(m_sockHandle == INVALID_SOCKET)
    {
        m_sockHandle = (int)socket(AF_INET, m_sockType, 0);
        if(m_sockHandle==0) throw  SimpleSock::Exception(0x0001, "SimpleSock::Constructor: unable to create the socket", GetSocketError());
    }

    memset(&m_sockAddress, 0, sizeof(m_sockAddress));
	m_sockAddress.sin_family = AF_INET;
	m_sockAddress.sin_port=htons(port);
    m_sockAddress.sin_addr.s_addr=ipAddress;
    m_isOpen = true;
}

bool SimpleSock::isOpen()
{
    return m_isOpen;
}

void SimpleSock::Close()
{
    if(m_sockHandle != INVALID_SOCKET)
    {
        #ifdef WIN32
            shutdown(m_sockHandle, SD_BOTH);
        #else
            shutdown(m_sockHandle, SHUT_RDWR);
        #endif
        closesocket(m_sockHandle);
    }
 	m_sockHandle = INVALID_SOCKET;
    m_isOpen = false;
}

void SimpleSock::Listen(int port, unsigned long ipAddress)
{
    int arg = 1;

    Open(port, ipAddress);

    switch(m_sockType)
    {
        case SOCK_DGRAM :
            if(setsockopt(m_sockHandle, SOL_SOCKET, SO_BROADCAST, (char*)&arg, sizeof(int))!=0)
                throw SimpleSock::Exception(0x0021, "SimpleSock::Listen: setsockopt(SO_BROADCAST) error", GetSocketError());
            break;

        case SOCK_STREAM :
            if(setsockopt(m_sockHandle, SOL_SOCKET, SO_REUSEADDR, (char*)&arg, sizeof(int))!=0)
                throw SimpleSock::Exception(0x0022, "SimpleSock::Listen: setsockopt(SO_REUSEADDR) error", GetSocketError());
            break;

        default :
            throw SimpleSock::Exception(0x0024, "SimpleSock::Listen: unknown socket type");
    }

   	if(bind(m_sockHandle, (struct sockaddr*) &m_sockAddress, sizeof(m_sockAddress))!=0)
        throw  SimpleSock::Exception(0x0025, "SimpleSock::Listen: bind error", GetSocketError());

    if(m_sockType==SOCK_STREAM)
    {
        if(listen(m_sockHandle, 50)!=0)
            throw  SimpleSock::Exception(0x0027, "SimpleSock::Listen: listen error", GetSocketError());
    }
}

bool SimpleSock::WaitRecv(int delay)
{
	fd_set fsSock;
	struct timeval tvDelay;

    FD_ZERO(&fsSock);
    FD_SET(m_sockHandle, &fsSock);

    tvDelay.tv_sec=0;
    tvDelay.tv_usec=delay;

    select(m_sockHandle+1, &fsSock, NULL, NULL, &tvDelay);

    return FD_ISSET(m_sockHandle, &fsSock)>0;
}

void SimpleSock::Send(const char* buffer, int bufferSize)
{
    size_t sizeSent;
    size_t sizeMax;
    int status;


    if(!m_isOpen) throw SimpleSock::Exception(0x0042, "SimpleSock::Send: Open() method is mandatory before Send()");

    sizeMax = bufferSize;
    sizeSent = 0;

    while(sizeSent < sizeMax)
    {
        switch(m_sockType)
        {
            case SOCK_DGRAM :
                status = sendto(m_sockHandle, buffer + sizeSent, sizeMax - sizeSent, 0, (struct sockaddr *)&m_sockAddress, sizeof(struct sockaddr));
                break;

            case SOCK_STREAM :
                status = send(m_sockHandle, buffer + sizeSent, sizeMax - sizeSent, 0);
                break;

            default :
                throw SimpleSock::Exception(0x0040, "SimpleSock::Send: unknown socket type");
        }

        if((status==-1)&&(!OperationOk()))
			throw SimpleSock::Exception(0x0041, "SimpleSock::Send: send error", GetSocketError());

		if(status != -1) sizeSent += status;
    }
}

void SimpleSock::Send(const string& buffer)
{
    Send(buffer.c_str(), buffer.size());
}

unsigned SimpleSock::Recv(char* buffer, int bufferSize)
{
	int status;


    if(!m_isOpen) throw SimpleSock::Exception(0x0052, "SimpleSock::Recv: Listen() or Connect() method is mandatory before Recv()");

    switch(m_sockType)
    {
        case SOCK_DGRAM :
            status = recvfrom(m_sockHandle, buffer, bufferSize, 0, 0, 0);
            break;

        case SOCK_STREAM :
            status = recv(m_sockHandle, buffer, bufferSize, 0);
            break;

        default :
            throw SimpleSock::Exception(0x0050, "SimpleSock::Recv: unknown socket type");
    }

    if((status==-1)&&(!OperationOk()))
        throw SimpleSock::Exception(0x0051, "SimpleSock::Recv: recv[from] error", GetSocketError());

	return status;
}

bool SimpleSock::Recv(string& buffer, int sizeMax)
{
	int status;
	char buf[1536];
	bool bRet;


    if(!m_isOpen) throw SimpleSock::Exception(0x0052, "SimpleSock::Recv: Listen() or Connect() method is mandatory before Recv()");

    buffer = "";
    bRet = false;
    do
    {
        int sizBuf;

        sizBuf = sizeof(buf)-1;
        if((sizeMax!=-1)&&(sizBuf>sizeMax)) sizBuf=sizeMax;
        switch(m_sockType)
        {
            case SOCK_DGRAM :
                status = recvfrom(m_sockHandle, buf, sizBuf, 0, 0, 0);
                break;

            case SOCK_STREAM :
                status = recv(m_sockHandle, buf, sizBuf, 0);
                break;

            default :
                throw SimpleSock::Exception(0x0050, "SimpleSock::Recv: unknown socket type");
        }

        if((status==-1)&&(!OperationOk()))
            throw SimpleSock::Exception(0x0051, "SimpleSock::Recv: recv[from] error", GetSocketError());

        if(status!=-1)
        {
            buf[status]='\0';
            buffer += buf;
            bRet = true;
            if(sizeMax!=-1)
            {
                sizeMax -= status;
                if(sizeMax==0) status = -1;
            }
        }

    } while((status!=-1)&&(m_blocking==false));

	return bRet;
}

void SimpleSock::Blocking(bool blocking)
{
    int result;

	#ifdef __linux__
        int flags;
        flags = fcntl(m_sockHandle, F_GETFL);
        if(blocking)
            flags = (flags & ~O_NONBLOCK);
        else
            flags = (flags | O_NONBLOCK);
        result = fcntl(m_sockHandle, F_SETFL, flags);
    #else
        u_long iMode = !blocking;
        result = ioctlsocket(m_sockHandle, FIONBIO, &iMode);
        if(result!=0) result = -1;
	#endif

    if (result == -1)
        throw SimpleSockUDP::Exception(0x0031, "SimpleSockUDP::Blocking: ioctl error", GetSocketError());

    m_blocking = blocking;
}

int SimpleSock::GetSocketError()
{
    #ifdef __linux__
        return errno;
    #else
        return WSAGetLastError();
    #endif
}

bool SimpleSock::OperationOk()
{
    int err;

    err = GetSocketError();
    #ifdef __linux__
        if(err == EAGAIN || err == EWOULDBLOCK) return true;
    #else
        if(err == WSAEWOULDBLOCK) return true;
    #endif
    return false;
}

/***************************************************************************************************/
/***                                                                                             ***/
/*** Class SimpleSockUDP                                                                         ***/
/***                                                                                             ***/
/***************************************************************************************************/
SimpleSockUDP::SimpleSockUDP() : SimpleSock(SOCK_DGRAM)
{
}

void SimpleSockUDP::Open(int port)
{
    SimpleSock::Open(port, BroadcastAddress(""));
    //Open(port, INADDR_BROADCAST); //-> Erreur sur le sendto
}

void SimpleSockUDP::Open(int port, const string& ipAddress)
{
    SimpleSock::Open(port, inet_addr(ipAddress.c_str()));
}

void SimpleSockUDP::Listen(int port)
{
    SimpleSock::Listen(port, BroadcastAddress(""));
}

void SimpleSockUDP::Listen(int port, int address)
{
    SimpleSock::Listen(port, address);
}

void SimpleSockUDP::Listen(int port, const string& ipAddress)
{
    SimpleSock::Listen(port, inet_addr(ipAddress.c_str()));
}

string SimpleSockUDP::LocalAddress(const string& interfaceName="")
{
    int sockUDP;
    int arg = 1;
    struct in_addr netmaskAddr;
    struct in_addr interfaceAddr;
    bool bOK;


	if((sockUDP = socket(AF_INET, SOCK_DGRAM, 0)) == -1) return "127.0.0.1";
	bOK = ((setsockopt(sockUDP, SOL_SOCKET, SO_BROADCAST, (char*)&arg, sizeof(int))) != -1);
	if(bOK)
	{
        bOK = GetInterfaceInfo(interfaceName, sockUDP, &interfaceAddr, &netmaskAddr);
    }

    closesocket(sockUDP);
    if(bOK)
        return inet_ntoa(interfaceAddr);
    else
        return "127.0.0.1";
}

unsigned long SimpleSockUDP::BroadcastAddress(const string& interfaceName="")
{
    SOCKET sockUDP;
    int arg = 1;
    unsigned long addr = inet_addr("127.255.255.255");
    struct in_addr netmaskAddr;
    struct in_addr interfaceAddr;


	if((sockUDP = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) return addr;
	if((setsockopt(sockUDP, SOL_SOCKET, SO_BROADCAST, (char*)&arg, sizeof(int))) != -1)
	{
        if(GetInterfaceInfo(interfaceName, sockUDP, &interfaceAddr, &netmaskAddr))
            addr = (~netmaskAddr.s_addr)|interfaceAddr.s_addr;
    }

    closesocket(sockUDP);
    return addr;
}

#ifdef __linux__
bool SimpleSockUDP::GetInterfaceInfo(const string& interfaceName, unsigned int socket, struct in_addr *interfaceAddr, struct in_addr *netmaskAddr)
{
	struct ifreq interface;

    if(interfaceName=="")
    {
        bool ret;
        ret = GetInterfaceInfo("eth0", socket, interfaceAddr, netmaskAddr);
        if(ret) return true;
        return GetInterfaceInfo("egiga0", socket, interfaceAddr, netmaskAddr);
    }

	memset((char*)&interface, 0, sizeof(interface));
	interface.ifr_addr.sa_family = AF_INET;
	interface.ifr_broadaddr.sa_family = AF_INET;
    strcpy(interface.ifr_name,interfaceName.c_str());

	if ((ioctl(socket, SIOCGIFADDR, &interface))!=0) return false;
	memcpy(interfaceAddr, &((struct sockaddr_in*)&interface.ifr_broadaddr)->sin_addr, sizeof(struct in_addr));

	if ((ioctl(socket, SIOCGIFNETMASK, &interface))!=0) return false;
	memcpy(netmaskAddr, &((struct sockaddr_in*)&interface.ifr_broadaddr)->sin_addr, sizeof(struct in_addr));

	return true;
}
#else
#pragma GCC diagnostic ignored "-Wunused-parameter"
bool SimpleSockUDP::GetInterfaceInfo(const string& interfaceName, unsigned int socket, struct in_addr *interfaceAddr, struct in_addr *netmaskAddr)
{
	DWORD bytesReturned;
	u_long flags;
	INTERFACE_INFO localAddr[10];  // Assume there will be no more than 10 IP interfaces
	int numLocalAddr;
	int i;


	if ((WSAIoctl(socket, SIO_GET_INTERFACE_LIST, NULL, 0, &localAddr, sizeof(localAddr), &bytesReturned, NULL, NULL)) == SOCKET_ERROR) return false;

	numLocalAddr = (bytesReturned/sizeof(INTERFACE_INFO));
	for (i=0; i<numLocalAddr; i++)
	{
		flags = localAddr[i].iiFlags;
		if(!(flags & IFF_UP)) continue;
		if(!(flags & IFF_BROADCAST)) continue;
		if(!(flags & IFF_MULTICAST)) continue;
		if (flags & IFF_LOOPBACK) continue;
		if (flags & IFF_POINTTOPOINT) continue;

        memcpy(interfaceAddr, &localAddr[i].iiAddress.AddressIn.sin_addr, sizeof(struct in_addr));
        memcpy(netmaskAddr,   &localAddr[i].iiNetmask.AddressIn.sin_addr, sizeof(struct in_addr));

		break;
	}

	return true;
}
#pragma GCC diagnostic pop
#endif

/***************************************************************************************************/
/***                                                                                             ***/
/*** Class SimpleSockTCP                                                                         ***/
/***                                                                                             ***/
/***************************************************************************************************/
SimpleSockTCP::SimpleSockTCP() : SimpleSock(SOCK_STREAM)
{
}

void SimpleSockTCP::Connect(const string& ipAddress, int port)
{
    SimpleSock::Open(port, inet_addr(ipAddress.c_str()));
    if(connect(m_sockHandle,(sockaddr *) &m_sockAddress, sizeof(sockaddr)) == -1)
        throw SimpleSockTCP::Exception(0x0061, "SimpleSockTCP::Connect: unable to connect", GetSocketError());
}

void SimpleSockTCP::Listen(int port)
{
    SimpleSock::Listen(port, INADDR_ANY);
}

void SimpleSockTCP::Listen(int port, const string& ipAddress)
{
    SimpleSock::Listen(port, inet_addr(ipAddress.c_str()));
}

bool SimpleSockTCP::Accept(SimpleSockTCP* sockAccept)
{
	ARG_ACCEPT arg;
	SOCKET	sockClient;
	sockaddr_in	sockAddr;


    arg = sizeof sockAddr;
    sockClient = accept(m_sockHandle, (sockaddr *)&sockAddr, &arg);
    if(sockClient==0) return false;

    if(sockAccept->isOpen()) sockAccept->Close();
    sockAccept->SetSocket(sockClient, sockAddr, SOCK_STREAM);

    return true;
}

/***************************************************************************************************/
/***                                                                                             ***/
/*** Class SimpleSock::Exception                                                                 ***/
/***                                                                                             ***/
/***************************************************************************************************/
SimpleSock::Exception::Exception(int number, string const& message) throw()
{
    m_number = number;
    m_message = message;
    m_system = 0;
    SetWhatMsg();
}

SimpleSock::Exception::Exception(int number, string const& message, int system) throw()
{
    m_number = number;
    m_message = message;
    m_system = system;
    SetWhatMsg();
}

SimpleSock::Exception::~Exception() throw()
{
}

void SimpleSock::Exception::SetWhatMsg()
{
    ostringstream message;

    message << m_message;
    if(m_system!=0) message << " (system error " << m_system << ")";
    m_whatMsg = message.str();
}

const char* SimpleSock::Exception::what() const throw()
{
    return m_whatMsg.c_str();
}

int SimpleSock::Exception::GetNumber() const throw()
{
    return m_number;
}
