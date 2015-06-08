#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "../Command/Factory.hpp"

using namespace std;
using namespace Server;

CServer::CServer (int port):
    socket(port, {})
{
    socket.Listen();
    fds.push_back({socket.GetFd(), POLLIN});
    fds.push_back({STDIN_FILENO, POLLIN});
    signal(SIGPIPE, SIG_IGN);
}

CServer::~CServer (void)
{
    for (auto & i : users) {
        i.second->LeaveAllChannels();
    }
    users.clear();
}

void CServer::Run (void)
{
    while (true) {
        socket.Wait(fds.data(), fds.size());
        fds.reserve(fds.size() + 1);
        for (const auto & i : fds) {
            if (i.fd == socket.GetFd()) {
                ProcessServerEvent(i);
            } else if (i.fd == STDIN_FILENO) {
                ProcessKeyboardEvent(i);
            } else {
                ProcessUserEvent(i);
            }
        }
    }
}

void CServer::Join (int fd, const string & channelName)
{
    auto user = users.at(fd);
        
    if (!channels[channelName]) {
        channels[channelName] = make_shared<CChannel>(channelName);
    }

    user->Join(channels[channelName]);
    channels[channelName]->AddUser(user);
}

void CServer::ProcessUserEvent (const pollfd & pfd)
{
    if (pfd.revents & POLLIN) {
        if (!users[pfd.fd]->IsConnected()) {
            RemoveUser(pfd.fd);
            return;
        }
        try {
            users[pfd.fd]->ReadMsg()->Execute(*this, pfd.fd);
        } catch (Command::CInvalidFormatException e) {
            clog << '#' << pfd.fd << " Invalid command: " << e.what() << endl;
        }
    }
}

void CServer::ProcessServerEvent (const pollfd & pfd)
{
    if (pfd.revents & POLLIN) {
        fds.push_back({socket.Accept(), POLLIN});
        AddUser(fds.back().fd);
    }
}

void CServer::ProcessKeyboardEvent (const pollfd & pfd)
{
    if (!(pfd.revents & POLLIN)) {
        return;
    }
    
    string str;
    getline(cin, str);
    if (str == "channels") {
        for (const auto & i : channels) {
            cout << i.first << endl;
        }
    } else if (str == "users") {
        for (const auto & i : users) {
            cout << '#' << i.first << " " << i.second->GetName() << endl;
        }
    } else if (str == "quit") {
        throw runtime_error("Exit");
    } else {
        cout << "Unknown command" << endl;
    }
}

void CServer::AddUser (int fd)
{    
    auto user = make_shared<CUser>(fd, CUser::NAME_PREFIX + to_string(fd));
    users.insert(make_pair(fd, user));
}

void CServer::RemoveUser (int fd)
{
    users.at(fd)->LeaveAllChannels();
    fds.erase(find_if(fds.begin(), fds.end(), [fd] (decltype(fds)::const_reference val) {return fd == val.fd;}));
    users.erase(fd);
    RemoveEmptyChannels();
}

void CServer::Leave (int fd, const string & channelName, const string & reason)
{
    if (!channels.count(channelName)) {
        return;
    }
    
    auto & channel = channels[channelName];    
    auto & user = users.at(fd);
    
    user->LeaveChannel(channel);
    channel->RemoveUser(user, reason);
    if (channel->IsEmpty()) {
        channels.erase(channelName);
    }
}

void CServer::RemoveEmptyChannels ()
{
    for (auto it = channels.begin(); it != channels.end(); ) {
        if (it->second->IsEmpty()) {
            channels.erase(it++);
        } else {
            ++it;
        }
    }
}

shared_ptr<CChannel> CServer::FindChannel (const string & channel) const
{
    return channels.at(channel);
}

shared_ptr<CUser> CServer::FindUser (int fd) const
{
    return users.at(fd);
}

shared_ptr<CUser> CServer::FindUser (const string & name) const
{
    auto it = find_if(users.cbegin(), users.cend(), [&name] (decltype(users)::const_reference user) {return user.second->GetName() == name;});
    
    if (it == users.cend()) {
        return nullptr;
    }
    return it->second;
    
}
