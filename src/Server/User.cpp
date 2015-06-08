#include "User.hpp"
#include "../Command/Factory.hpp"
#include "../Command/ChangeName.hpp"

using namespace std;
using namespace Server;

constexpr char CUser::NAME_PREFIX[];

CUser::CUser (int fd, const string & name):
    socket(fd),
    name(name)
{
    SendMsg(*Command::CFactory::Create(Command::CChangeName::TYPE, {{"name", name}}));
}

void CUser::SendToAllChannels (const Command::CBase & cmd) const
{
    for (const auto & i : channels) {
        i->Send(cmd);
    }
}
    
unique_ptr<Command::CBase> CUser::ReadMsg (void)
{
    return Command::CFactory::Create(socket.Receive());
}

void CUser::SendMsg (const Command::CBase & cmd) const
{
    socket.Send(cmd.Serialize());
}

void CUser::SetName (const string & newName)
{
    if (newName.substr(0, sizeof(NAME_PREFIX) - 1) != NAME_PREFIX) {
        name = newName;
    }
    SendMsg(*Command::CFactory::Create(Command::CChangeName::TYPE, {{"name", name}}));
}

string CUser::GetName () const
{
    return name;
}

void CUser::LeaveChannel (shared_ptr<CChannel> channel)
{
    channels.erase(channel);
}

void CUser::LeaveAllChannels ()
{
    for (auto & i : channels) {
        i->RemoveUser(shared_from_this(), "User has left");
    }
    channels.clear();
}

void CUser::Join (shared_ptr<CChannel> channel)
{
    channels.insert(channel);
}

bool CUser::IsConnected (void) const
{
    return socket.IsOpen();
}
