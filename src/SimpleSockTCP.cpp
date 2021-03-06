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
#include <exception>
#include "SimpleSockTCP.h"

#include <iostream>

using namespace std;

/***************************************************************************************************/
/***                                                                                             ***/
/*** Class SimpleSockTCP                                                                         ***/
/***                                                                                             ***/
/***************************************************************************************************/
SimpleSockTCP::SimpleSockTCP() : SimpleSock(SOCK_STREAM), m_ConnectTimeout(-1)
{
}

void SimpleSockTCP::Connect(const string& ipAddress, int port)
{
    SimpleSock::Open(port, inet_addr(ipAddress.c_str()));
    if(connect(m_sockHandle,(sockaddr *) &m_sockAddress, sizeof(sockaddr)) == 0) return;

    int error = GetSocketError();
    if((m_ConnectTimeout == -1)||(error != EINPROGRESS))
    {
        throw SimpleSockTCP::Exception(0x0061, "SimpleSockTCP::Connect: unable to connect", error);
    }

   	fd_set fsSock;
	struct timeval tvDelay;

    FD_ZERO(&fsSock);
    FD_SET(m_sockHandle, &fsSock);

    tvDelay.tv_sec=m_ConnectTimeout;
    tvDelay.tv_usec=0;

    select(m_sockHandle+1, nullptr, &fsSock, nullptr, &tvDelay);

    if(FD_ISSET(m_sockHandle, &fsSock)<=0)
        throw SimpleSockTCP::Exception(0x0062, "SimpleSockTCP::Connect: timeout for connect", GetSocketError());
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

void SimpleSockTCP::Listen(int port)
{
    SimpleSock::Listen(port, INADDR_ANY);
}

void SimpleSockTCP::Listen(int port, const string& ipAddress)
{
    SimpleSock::Listen(port, ipAddress);
}

void SimpleSockTCP::SetTimeout(int send, int recv, int connect)
{
    SimpleSock::SetTimeout(send, recv);
    m_ConnectTimeout = connect;
}
