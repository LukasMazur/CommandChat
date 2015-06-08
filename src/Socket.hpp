/**
 * @file src/Socket.hpp
 */
#ifndef INC_SOCKET_HPP
#define INC_SOCKET_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <utility>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>

/**
 * @brief Provides encapsulation of various unix calls relating to sockets and networking
 */
class CSocket
{
    public:
        /**
         * @brief This constructor suppose that socket is already opened
         * @param fd file descriptor of existing socket
         */
        CSocket (int);
        /**
         * @brief This constructor opens socket
         * @param port 
         * @param host can be ether ip address or domain name
         */
        CSocket (int, const std::string &);
        CSocket (const CSocket &) = default;
        ~CSocket ();
        
        /**
         * @brief Send data to socket as-is
         * @param str Data to be sent
         */
        void Send (const std::string &) const;
        /**
         * @brief Receive raw data as-is from socket
         * @return Buffer with unformated data
         */
        std::stringstream & Receive (void);
        /**
         * @brief Connect to remote host, this function internally creates socket and binds
         */
        void Connect (void) const;
        /**
         * @brief Listens on socket, this function internally creates socket and binds
         */
        void Listen (void) const;
        /**
         * @brief Accept new connection when listening
         * @return File descriptor of new connection
         */
        int Accept (void);
        /**
         * @brief Wait for event on file descritpors
         * @param pollfds pointer to a c-array
         * @param size number of items
         */
        void Wait (pollfd *, nfds_t) const;
        /**
         * @return File descriptor of socket
         */
        int GetFd (void) const;
        
        /**
         * @return true if connection is still open
         */
        bool IsOpen (void) const;
        
        /**
         * @return true if data is waiting
         */
        bool HasData (void) const;
    
    private:
        using pAddrInfo = std::unique_ptr<addrinfo, decltype(&freeaddrinfo)>;
        
        /**
         * @return unique_ptr to addrinfo struct
         */
        pAddrInfo GetAddrInfo (int port, const std::string & hostname = std::string()) const;
        
        const pAddrInfo myAddr;
        const int fd;
        std::stringstream buf;
        
        constexpr static int ERROR = -1;
        constexpr static int SHUTDOWN = 0;
};

#endif
