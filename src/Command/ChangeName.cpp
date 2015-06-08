#include "ChangeName.hpp"
#include "Factory.hpp"
#include "../Server/User.hpp"

using namespace Command;

void CChangeName::Execute (Client::CClient & client) const
{
    client.SetName(GetField("name"));
}

void CChangeName::Execute (Server::CServer & server, int fd) const
{
    auto tmp = server.FindUser(GetField("name"));
    if (!tmp) {
        server.FindUser(fd)->SetName(GetField("name"));
    }
}

const char CChangeName::TYPE = CFactory::Register<CChangeName>('N');
