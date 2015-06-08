#include "Channel.hpp"
#include "User.hpp"
#include "../Command/Factory.hpp"
#include "../Command/Join.hpp"
#include "../Command/Leave.hpp"

using namespace std;
using namespace Server;

CChannel::CChannel (const string & channel):
    channel(channel)
{}

void CChannel::RemoveUser (shared_ptr<CUser> user, const string & reason)
{    
    if (users.find(user) != users.cend()) {
        users.erase(user);
        auto cmd = Command::CFactory::Create(Command::CLeave::TYPE, {{"name", user->GetName()}, {"reason", reason}, {"channel", channel}});
        Send(*cmd);
    }
}

void CChannel::AddUser (shared_ptr<CUser> user)
{    
    if (users.insert(user).second) {
        auto cmd = Command::CFactory::Create(Command::CJoin::TYPE, {{"name", user->GetName()}, {"channel", channel}});
        Send(*cmd);
    }
}

bool CChannel::IsEmpty() const
{
    return users.empty();
}

void CChannel::Send (const Command::CBase & cmd) const
{
    for (const auto & i : users) {
        i->SendMsg(cmd);
    }
}
