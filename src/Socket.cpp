#include <array>
#include <algorithm>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/ioctl.h>
#include "Socket.hpp"

using namespace std;

CSocket::CSocket (int fd):
    myAddr({nullptr, &freeaddrinfo}),
    fd(fd)
{}

CSocket::CSocket (int port, const string & host):
    myAddr(GetAddrInfo(port, host)),
    fd(socket(myAddr->ai_family, myAddr->ai_socktype, myAddr->ai_protocol))
{
    if (fd <= ERROR) {
        throw runtime_error("Cannot create socket");
    }
    int tmp = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(tmp)) <= ERROR) {
        throw runtime_error("Cannot set address reusing");
    }
}

CSocket::~CSocket (void)
{
    close(fd);
}

void CSocket::Connect (void) const
{
    if (connect(fd, myAddr->ai_addr, myAddr->ai_addrlen) <= ERROR) {
        throw runtime_error("Cannot connect to server");
    }
}

void CSocket::Listen (void) const
{
    if (bind(fd, myAddr->ai_addr, myAddr->ai_addrlen) <= ERROR) {
        throw runtime_error("Cannot bind");
    }
    if (listen(fd, 10) == ERROR) {
        throw runtime_error("Cannot listen");
    }
}

int CSocket::Accept (void)
{
    struct sockaddr myAddr;
    socklen_t len = sizeof(myAddr);
    bzero(&myAddr, sizeof(myAddr));

    int clientFd = accept(fd, &myAddr, &len);

    if (clientFd <= ERROR) {
        throw runtime_error("Server accept error");
    }
    
    return clientFd;
}

void CSocket::Wait (pollfd * pollfds, nfds_t size) const
{
    if (poll(pollfds, size, -1) <= ERROR) {
        throw runtime_error("Server listening error");
    }
}

void CSocket::Send (const string & str) const
{
    if (send(fd, str.c_str(), str.size(), 0) <= ERROR) {
        throw runtime_error("Unable to send data");
    }
}

stringstream & CSocket::Receive (void)
{
    while (true) {
        array<char, 1024> tmp;
        
        ssize_t res = recv(fd, tmp.data(), tmp.max_size(), MSG_DONTWAIT);
        if (res == ERROR) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                break;
            }
            throw runtime_error("Unable to receive data");
        } else {
            buf.write(tmp.data(), res);
        }
    }
    
    return buf;
}

bool CSocket::IsOpen (void) const
{
    char c;
    return recv(fd, &c, 1, MSG_PEEK|MSG_DONTWAIT) != SHUTDOWN;
}

int CSocket::GetFd (void) const
{
    return fd;
}

bool CSocket::HasData (void) const
{
    int data;
    
    ioctl(fd, FIONREAD, &data);
    
    return data;
}

CSocket::pAddrInfo CSocket::GetAddrInfo (int port, const string & hostname) const
{
    struct addrinfo *myAddrPtr, hints;
    bzero(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(hostname.empty() ? nullptr : hostname.c_str(), to_string(port).c_str(), &hints, &myAddrPtr) <= ERROR) {
        throw runtime_error("Cannot get address");
    }
    
    return unique_ptr<addrinfo, decltype(&freeaddrinfo)>(myAddrPtr, &freeaddrinfo);
}
