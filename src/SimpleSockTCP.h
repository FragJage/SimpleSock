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

#ifndef SIMPLESOCKTCP_H
#define SIMPLESOCKTCP_H

#include "SimpleSock.h"

/// \brief    Very simple class to manage TCP sockets
/// \details  Class allows you to easily manage TCP sockets with very few methods.
class SimpleSockTCP : public SimpleSock
{
    public:
        /// \brief    Constructor of SimpleSockTCP
        /// \details  Constructor of SimpleSockTCP, optionally can load configuration file \a filename, by Load method. If the Load method fails, an exception is raised.
        SimpleSockTCP();

        /// \brief    Open a connexion
        /// \param    port          Port number.
        /// \param    ipAddress     IP Address of the host.
        /// \details  Open a connexion to the IP Address "ipAddress" on the port "port".
        void Connect(const std::string& ipAddress, int port);

        /// \brief    Accept an incoming connections
        /// \param    sockAccept    Socket of the incoming connection.
        /// \details  Accept an incoming connection and set the sockAccept.
        bool Accept(SimpleSockTCP* sockAccept);

        /// \brief    Listen for incoming connections
        /// \param    port          Port number.
        /// \details  Listen for incoming connections on the port "port".
        void Listen(int port);
        void Listen(int port, const std::string& ipAddress);

        #ifdef SIMPLESOCKTCP_MOCK
        static std::string GetLastSend(int delay);
        static void SetNextRecv(std::string value);
        #endif

    private:
        #ifdef SIMPLESOCKTCP_MOCK
        static bool SomethingToRecv();
        static std::string GetRecv();
        static void SetSend(std::string buffer);

        static std::string g_MockRecv;
        static std::queue<std::string> g_MockSend;
        static std::mutex g_MockRecvMutex;
        static std::mutex g_MockSendMutex;
        #endif
};

#endif // SIMPLESOCKTCP_H
