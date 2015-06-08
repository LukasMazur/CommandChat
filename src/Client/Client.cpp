#include <algorithm>
#include <utility>
#include <cctype>
#include <iterator>
#include <unistd.h>
#include <signal.h>
#include "Client.hpp"
#include "../Command/Base.hpp"
#include "../Command/ChangeName.hpp"
#include "../Command/Message.hpp"
#include "../Command/Join.hpp"
#include "../Command/Leave.hpp"
#include "../Command/Factory.hpp"

using namespace std;
using namespace Client;

CClient::CClient (const string & host, int port):
    socket(port, host)
{
    socket.Connect();
    initscr();
    cbreak();
    noecho();
    halfdelay(5);
    curs_set(0);
    
    // this cannot be done in initialization because initscr() has to be called before creating windows
    channelsWin = CWindow(LINES - 3, COLS * 0.20, 0, 0);
    msgsWin = CWindow(LINES - 3, 0, 0, COLS * 0.20); // rounding up to fill the gap
    inputWin = CWindow(3, 0, LINES - 3, 0);
    
    msgsWin.EnableKeypad();
    ShowWindow();
}

CClient::~CClient (void)
{
    curs_set(1);
    endwin();
}

void CClient::Run ()
{
    while (true) {
        int c = msgsWin.GetCh();

        if (c == '\n') {
            ProcessInput();
        } else if (isprint(c)) {
            input += c;
        } else if (c == KEY_BACKSPACE && !input.empty()) {
            input.pop_back();
        }
        
        PrintInput();
        
        if (!socket.IsOpen()) {
            return;
        }
        if (socket.HasData()) {
            ProcessCommand();
        }
    }
}

void CClient::ProcessCommand ()
{
    if (!socket.IsOpen()) {
        throw runtime_error("Exit");
    }
    try {
        Command::CFactory::Create(socket.Receive())->Execute(*this);
    } catch (Command::CInvalidFormatException e) {
        AddMessage(currentChannel, make_pair("", "Invalid Command"));
    }
    
    ShowWindow();
}

void CClient::Leave (const string & channel)
{
    channels.erase(channel);
    
    currentChannel = channels.empty() ? string() : channels.cbegin()->first;
}

void CClient::AddMessage (const string & channelName, const pair<string, string> & msg)
{
    if (!channels.count(channelName)) {
        currentChannel = channelName;
    }
    channels[channelName].AddMsg(msg.first, msg.second);
}

void CClient::SetName (const string & newName)
{
    name = newName;
    AddMessage(currentChannel, make_pair("", "Your new name is: " + name));
}

string CClient::GetName () const
{
    return name;
}

void CClient::ProcessInput ()
{
    auto cmd = input.front() == COMMAND_PREFIX ? ParseCommand() : 
            Command::CFactory::Create(Command::CMessage::TYPE, {{"channel", currentChannel}, {"msg", input}});
        
    if (cmd) {
        socket.Send(cmd->Serialize());
    }
    
    input.clear();
    PrintInput();    
    ShowWindow();
}

unique_ptr<Command::CBase> CClient::ParseCommand ()
{
    istringstream is(input);
    const vector<string> tmp((istream_iterator<string>(is)), (istream_iterator<string>()));
    
    try{
        char c = input.at(1);
        input.clear();
        if (c == 'Q') {
            throw runtime_error("Exit");
        }
        if (c == 'C') {
            currentChannel = tmp.at(1);
            return nullptr;
        }
        if (c == Command::CJoin::TYPE) {
            return Command::CFactory::Create(Command::CJoin::TYPE, {{"channel", tmp.at(1)}});
        }
        if (c == Command::CLeave::TYPE) {
            string chan = currentChannel;
            Leave(currentChannel);
            return Command::CFactory::Create(Command::CLeave::TYPE, {{"channel", chan}, {"reason", tmp.at(1)}});
        }
        if (c == Command::CChangeName::TYPE) {
            return Command::CFactory::Create(Command::CChangeName::TYPE, {{"name", tmp.at(1)}});
        }
        AddMessage(currentChannel, make_pair("", "Invalid format"));
    } catch (out_of_range e) {
        AddMessage(currentChannel, make_pair("", "Invalid format"));
    }
    
    return nullptr;
}

void CClient::PrintInput (void)
{
    inputWin.Clear();
    inputWin.PrintLine(name + DELIMITER + ' ' + input);
    Print();
}

void CClient::ShowWindow (void)
{
    channelsWin.Clear();
    msgsWin.Clear();
    channelsWin.MoveTo(CWindow::DIR::TOP).MoveTo(CWindow::DIR::START);
    
    for (auto & i : channels) {
        bool isCurrent = i.first == currentChannel;
        if (isCurrent) {
            PrintCurrentChannel(i.second);
        }
        channelsWin.PrintLine((i.second.GetUnread() ? '(' + to_string(i.second.GetUnread()) + ") " : string()) + i.first, isCurrent ? A_BOLD : A_NORMAL);
    }
    
    Print();
}

void CClient::PrintCurrentChannel (CChannel & channel)
{
    msgsWin.MoveTo(CWindow::DIR::BOTTOM);
    for (const auto & i : channel.GetMsgs()) {
        msgsWin.Print(i.first + DELIMITER + ' ' + i.second);
        msgsWin.MoveTo(CWindow::DIR::UP);
    }
}

void CClient::Print (void) const
{
    channelsWin.Refresh();
    msgsWin.Refresh();
    inputWin.Refresh();
    doupdate();
}
