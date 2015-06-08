#include "Message.hpp"
#include "Factory.hpp"
#include "../Server/User.hpp"
#include "../Server/Channel.hpp"

using namespace std;
using namespace Command;

void CMessage::Execute(Client::CClient & client) const
{
    client.AddMessage(GetField("channel"), make_pair(GetField("name"), GetField("msg")));
}

void CMessage::Execute (Server::CServer & server, int fd) const
{
    auto tmp = fields;
    
    tmp["name"] = server.FindUser(fd)->GetName();
    try {
        server.FindChannel(GetField("channel"))->Send(*CFactory::Create(TYPE, tmp));
    } catch (out_of_range) { 
        CInvalidFormatException(GetField("channel"));
    }
}

const char CMessage::TYPE = CFactory::Register<CMessage>('M');
