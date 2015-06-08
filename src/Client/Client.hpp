/**
 * @file src/Client/Client.hpp
 */
#ifndef INC_CLIENT_CLIENT_HPP
#define INC_CLIENT_CLIENT_HPP

#include <string>
#include <map>
#include <array>
#include <memory>
#include <list>
#include "../Socket.hpp"
#include "Window.hpp"
#include "Channel.hpp"

namespace Command {
    class CBase;
}

namespace Client
{
    /**
     * @brief Represents client.
     */
    class CClient
    {
        public:
            /**
             * @param host hostname
             * @param port
             */
            CClient (const std::string &, int);
            CClient (const CClient &) = delete;
            ~CClient (void);
        
            /**
             * @brief Listen on socket and wait for user input
             */
            void Run ();
            /**
             * @brief Leaves the channel
             * @param channel
             */
            void Leave (const std::string &);
            /**
             * @brief Adds new message to channel
             * @param channelName
             * @param msg pair of user and message
             */
            void AddMessage (const std::string &, const std::pair<std::string, std::string> &);
            /**
             * @brief Sets new name of user
             * @param newName
             */
            void SetName (const std::string &);
            /**
             * @brief Get name of user
             * @return string name
             */
            std::string GetName () const;
        
        private:
            /**
             * @brief Reads a command from network and executes it
             */
            void ProcessCommand (void);
            /**
             * @brief Load input from user
             * User input is completely handled by application
             */
            void ProcessInput (void);
            /**
             * @brief Print current input from user
             */
            void PrintInput (void);
            /**
             * @brief Prints window and its content
             */
            void ShowWindow (void);
            /**
             * @brief Parse command from user input
             * @return nullptr if command is invalid or is not sent to server
             */
            std::unique_ptr<Command::CBase> ParseCommand (void);
            /**
             * @brief Prints messages in current channel
             */
            void PrintCurrentChannel (CChannel & channel);
            /**
             * @brief Refreshes whe window
             */
            void Print (void) const;
            
            std::map<std::string, CChannel> channels;
            CSocket socket;
            std::string name;
            std::string input;
            std::string currentChannel;
            CWindow channelsWin;
            CWindow msgsWin;
            CWindow inputWin;
            
            constexpr static char COMMAND_PREFIX = '/';
            constexpr static char DELIMITER = '>';
    };
    
}
#endif
