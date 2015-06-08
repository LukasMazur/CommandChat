#include "Leave.hpp"
#include "Factory.hpp"

using namespace Command;

void CLeave::Execute(Client::CClient & client) const
{
    if (client.GetName() == GetField("name")) {
        client.Leave(GetField("channel"));
    } else {
        client.AddMessage(GetField("channel"), make_pair(GetField("name"), "Has left (" + GetField("reason") + ")"));
    }
}

void CLeave::Execute (Server::CServer & server, int fd) const
{
    server.Leave(fd, GetField("channel"), GetField("reason"));
}

const char CLeave::TYPE = CFactory::Register<CLeave>('L');
