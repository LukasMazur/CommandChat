#include "Join.hpp"
#include "Factory.hpp"

using namespace Command;

void CJoin::Execute(Client::CClient & client) const
{
    if (client.GetName() == GetField("name")) {
        client.AddMessage(GetField("channel"), make_pair(GetField("name"), "You have joined to channel " + GetField("channel")));
    } else {
        client.AddMessage(GetField("channel"), make_pair(GetField("name"), "Has joined"));
    }
}

void CJoin::Execute (Server::CServer & server, int fd) const
{
    server.Join(fd, GetField("channel"));
}

const char CJoin::TYPE = CFactory::Register<CJoin>('J');
