/**
 * @file src/Server/Server.hpp
 */
#ifndef INC_SERVER_SERVER_HPP
#define INC_SERVER_SERVER_HPP

#include <map>
#include <string>
#include <memory>
#include "../Socket.hpp"

namespace Server
{
    class CUser;
    class CChannel;
    
    /**
     * @brief Serves client and channels. 
     * Clients are represented by descriptor associated with particular socket, channels by name
     */
    class CServer
    {
        public:
            /**
             * @param port Port noumber to listen on
             */
            CServer (int port);
            /**
             * @brief This class is supposed to exists in only one instantion
             */
            CServer (const CServer &) = delete;
            ~CServer (void);

            /**
             * @brief Main server loop. Runs indefinetly till quit or exception thrown
             */
            void Run (void);
            /**
             * @brief Adds a new user a set a default name for him
             * @param fd Descriptor of newly connected client
             */
            void AddUser (int);
            /**
             * @brief Remove existing user
             * @param fd Descriptor of user to be removed
             */
            void RemoveUser (int fd);
            /**
             * @brief Add user to channel, when channel doesn't exists a new one will be created
             * @param fd User fd
             * @param channelName
             */
            void Join (int, const std::string &);
            /**
             * @brief Leave user to channel, when user  is the last one in channel, channel will be destroyed
             * @param fd User fd
             * @param channelName
             * @param reason reason why user is leaving
             */
            void Leave (int, const std::string &, const std::string &);
            /**
             * @brief Finds channel by name
             * @param channelName
             * @return std::shared_ptr<CChannel>
             * @throw std::out_of_range if channel not found
             */
            std::shared_ptr<CChannel> FindChannel (const std::string &) const;
            /**
             * @brief Finds user by descriptor
             * @param fd User descriptor
             * @return std::shared_ptr<CUser>
             * @throw std::out_of_range if channel not found
             */
            std::shared_ptr<CUser> FindUser (int) const;
            /**
             * @brief Finds user by name
             * @param name
             * @return std::shared_ptr<CUser> or nullptr if user not found
             */
            std::shared_ptr<CUser> FindUser (const std::string &) const;
            
        private:
            constexpr static char NAME_PREFIX[] = "User";
            
            void ProcessUserEvent (const pollfd &);
            void ProcessKeyboardEvent (const pollfd &);
            void ProcessServerEvent (const pollfd &);
            void RemoveEmptyChannels ();

            std::map<int, std::shared_ptr<CUser>> users;
            std::map<std::string, std::shared_ptr<CChannel>> channels;
            std::vector<pollfd> fds;
            CSocket socket;
    };
}
#endif
