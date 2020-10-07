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

#ifndef SIMPLESOCKUDPMOCK_H
#define SIMPLESOCKUDPMOCK_H

#include <queue>
#include <string>
#include <mutex>
#include "Plateforms.h"

using namespace std;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"


class SimpleSockUDP
{
    public:
        class Exception;

        SimpleSockUDP()
        {
            m_sockAddress.sin_family = AF_INET;
            m_sockAddress.sin_port=0;
            m_sockAddress.sin_addr.s_addr=0;
            m_isOpen = false;
            if(!SimpleSockUDP::m_initSocket) SimpleSockUDP::InitSocket();
        };
        unsigned long BroadcastAddress(const string& interfaceName)
        {
            return inet_addr("127.255.255.255");
        };
        std::string LocalAddress(const string& interfaceName)
        {
            return "127.0.0.1";
        };
        void SetNetworkInterface(const string& networkInterface)
        {
        };
        void SetSocket(SOCKET sockHandle, struct sockaddr_in sockAddress, int type)
        {
            m_sockAddress = sockAddress;
            m_isOpen = true;
        };
        void Open(int port)
        {
            Open(port, INADDR_BROADCAST);
        };
        void Open(int port, const string& ipAddress)
        {
            Open(port, inet_addr(ipAddress.c_str()));
        };
        void Open(int port, unsigned long ipAddress)
        {
            if((g_PortInUse!=-1)&&(g_PortInUse==port))
                throw  SimpleSock::Exception(0x0025, "SimpleSockUDP_Mock::Listen: bind error");

            memset(&m_sockAddress, 0, sizeof(m_sockAddress));
            m_sockAddress.sin_family = AF_INET;
            m_sockAddress.sin_port=htons(port);
            m_sockAddress.sin_addr.s_addr=ipAddress;
            m_isOpen = true;
        };
        bool isOpen()
        {
            return m_isOpen;
        };
        void Close()
        {
            m_isOpen = false;
        };
        void Listen(int port)
        {
            Open(port);
        };
        void Listen(int port, const string& ipAddress)
        {
            Open(port, ipAddress);
        };
        void Listen(int port, unsigned long ipAddress)
        {
            Open(port, ipAddress);
        };
        bool WaitRecv(int delay)
        {
            if(delay<100)
            {
                if(SomethingToRecv()) return true;
                Plateforms::delay(delay);
            }
            else
            {
                for(int i=0; i<10; i++)
                {
                    if(SomethingToRecv()) return true;
                    Plateforms::delay(delay/10);
                }
            }

            if(SomethingToRecv()) return true;
            return false;
        };
        void Send(const string& buffer)
        {
            if(SimpleSockUDP::g_ExceptionOnNextSend)
            {
                SimpleSockUDP::g_ExceptionOnNextSend = false;
                throw logic_error("Mock exception");
            }
            SetSend(buffer);
        };
        void Send(const char* buffer, size_t bufferSize)
        {
            SetSend(buffer);
        };
        bool Recv(string& buffer, int sizeMax=-1)
        {
            buffer = GetRecv();
            if(buffer=="") return false;
            return true;
        };
        unsigned Recv(char* buffer, size_t bufferSize)
        {
            int status;
            string tmp;


            if(bufferSize==0) throw logic_error("SimpleSockUDP::Recv: Invalid buffer size");
            if(buffer==nullptr) throw logic_error("SimpleSockUDP::Recv: Invalid buffer pointer");
            if(!m_isOpen) throw logic_error("SimpleSockUDP::Recv: Listen() or Connect() method is mandatory before Recv()");

            tmp = GetRecv();
            strncpy(buffer, tmp.c_str(), bufferSize);
            status = tmp.size();

            return status;
        };
        void Blocking(bool blocking)
        {

        };
        int GetPort()
        {
            return ntohs(m_sockAddress.sin_port);
        };
        string GetAddress()
        {
            return "127.0.0.1";
        };

        static void SetPortInUse(int port)
        {
            g_PortInUse = port;
        };

        static string GetLastSend(int delay)
        {
            string tmp;
            int idelay;

            for(idelay=0; idelay<=delay; idelay++)
            {
                SimpleSockUDP::g_MockSendMutex.lock();
                if(!SimpleSockUDP::g_MockSend.empty())
                {
                    tmp = SimpleSockUDP::g_MockSend.front();
                    SimpleSockUDP::g_MockSend.pop();
                }
                SimpleSockUDP::g_MockSendMutex.unlock();
                if(tmp!="") break;
                Plateforms::delay(100);
            }
            return tmp;
        };
        static void SetNextRecv(string value)
        {
            SimpleSockUDP::g_MockRecvMutex.lock();
            if(SimpleSockUDP::g_MockRecv!="")
            {
                std::stringstream ss;
                ss << SimpleSockUDP::g_MockRecv << endl << value;
                SimpleSockUDP::g_MockRecv = ss.str();
            }
            else
                SimpleSockUDP::g_MockRecv += value;
            SimpleSockUDP::g_MockRecvMutex.unlock();
        };
        static void ExceptionOnNextSend()
        {
            SimpleSockUDP::g_ExceptionOnNextSend = true;
        };

    private:
        bool m_isOpen;
        struct sockaddr_in m_sockAddress;

        static bool SomethingToRecv()
        {
            bool tmp = false;

            SimpleSockUDP::g_MockRecvMutex.lock();
            if(SimpleSockUDP::g_MockRecv!="") tmp = true;
            SimpleSockUDP::g_MockRecvMutex.unlock();
            return tmp;
        };
        static string GetRecv()
        {
            string tmp;

            SimpleSockUDP::g_MockRecvMutex.lock();
            tmp = SimpleSockUDP::g_MockRecv;
            SimpleSockUDP::g_MockRecv = "";
            SimpleSockUDP::g_MockRecvMutex.unlock();
            return tmp;
        };
        static void SetSend(string buffer)
        {
            SimpleSockUDP::g_MockSendMutex.lock();
            SimpleSockUDP::g_MockSend.push(buffer);
            SimpleSockUDP::g_MockSendMutex.unlock();
        };
        void InitSocket()
        {
            SimpleSockUDP::m_initSocket = true;

            #ifdef WIN32
            WSADATA	WsaData;
            if (WSAStartup(MAKEWORD(2,0), &WsaData) != 0)
                throw logic_error("WSAStartup error");
            #endif // WIN32
        };

        static string g_MockRecv;
        static queue<string> g_MockSend;
        static mutex g_MockRecvMutex;
        static mutex g_MockSendMutex;
        static bool m_initSocket;
        static bool g_ExceptionOnNextSend;
        static int g_PortInUse;
};

class SimpleSockUDP::Exception: public std::exception
{
public:
    Exception(int number, std::string const& message) throw() : m_number(number), m_message(message), m_system(0), m_whatMsg()
    {
        SetWhatMsg();
    };
    
    Exception(int number, std::string const& message, int system) throw() : m_number(number), m_message(message), m_system(system), m_whatMsg()
    {
        SetWhatMsg();
    };

    ~Exception() throw()
    {
    };
    
    const char* what() const throw()
    {
      return m_whatMsg.c_str();
    };

    int GetNumber() const throw()
    {
      return m_number;
    };

    private:
        void SetWhatMsg()
        {
            ostringstream message;
        
            message << m_message;
            if(m_system!=0) message << " (system error " << m_system << ")";
            m_whatMsg = message.str();
        };

        int m_number;
        std::string m_message;
        int m_system;
        std::string m_whatMsg;
};

#pragma GCC diagnostic pop
#endif // SIMPLESOCKUDPMOCK_H
