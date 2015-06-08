#include "Channel.hpp"

using namespace std;
using namespace Client;

CChannel::CChannel (void):
    unread(0)
{}

void CChannel::AddMsg (const string & user, const string & msg)
{
    msgs.push_front(make_pair(user, msg));
    if (msgs.size() > MSG_MAX) {
        msgs.pop_back();
    }
    unread++;
}

const CChannel::listMsgs & CChannel::GetMsgs (void)
{
    unread = 0;
    return msgs;
}

int CChannel::GetUnread (void) const
{
    return unread;
}
