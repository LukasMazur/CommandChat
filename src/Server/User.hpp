/**
 * @file src/Server/User.hpp
 */
#ifndef INC_SERVER_USER_HPP
#define INC_SERVER_USER_HPP

#include <memory>
#include <set>
#include <memory>
#include <iostream>
#include "../Socket.hpp"
#include "Channel.hpp"
#include "../Command/Base.hpp"

namespace Server
{
    class CUser: public std::enable_shared_from_this<CUser>
    {
        public:
            /**
             * @brief constructs new object from existing descriptor and user name
             * @param fd file descriptor
             * @param name
             */
            CUser (int, const std::string &);
            CUser (const CUser &) = delete;
            ~CUser (void) = default;
        
            /**
             * @brief sends command to all channels where user is present
             * @param cmd command to be sent
             */
            void SendToAllChannels (const Command::CBase &) const;
            /**
             * @brief reads data from socket and converts it to Command::CBase derived object
             * @return command received from user
             */
            std::unique_ptr<Command::CBase> ReadMsg (void);
            /**
             * @brief takes Command::CBase derived object and converts it to string and send
             * @param cmd command to be sent
             */
            void SendMsg (const Command::CBase &) const;
            /**
             * @brief Sends message to user
             * @param name
             */
            void SetName (const std::string &);
            /**
             * @brief Get name of user
             * @return name of user
             */
            std::string GetName (void) const;
            /**
             * @brief adds user to a channel
             * @param shared_ptr to channel
             */
            void Join (std::shared_ptr<CChannel>);
            /**
             * @brief removes user from a channel
             * @param shared_ptr to a channel
             */
            void LeaveChannel (std::shared_ptr<CChannel>);
            /**
             * @brief Leaves all channels
             */
            void LeaveAllChannels (void);
            /**
             * @brief Checks if user is connected
             * @return true if user is connected
             */
            bool IsConnected (void) const;
            
            constexpr static char NAME_PREFIX[] = "User";

        private:
            CSocket socket;
            std::string name;
            std::set<std::shared_ptr<CChannel>> channels;
    };

}
#endif
