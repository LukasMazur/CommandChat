/**
 * @file src/Server/Channel.hpp
 */
#ifndef INC_SERVER_CHANNEL_HPP
#define INC_SERVER_CHANNEL_HPP

#include <memory>
#include <set>
#include "../Command/Base.hpp"

namespace Server
{
    class CUser;
    /**
     * @brief Represents channel on server.
     */
    class CChannel
    {    
        public:    
            /**
             * @param channel a channel name which is immutable
             */
            CChannel (const std::string &);
            /**
             * @brief Removes user from channel
             * @param user a shared_ptr to user to be removed
             * @param reason a reason why is user leaving
             */
            void RemoveUser (std::shared_ptr<CUser>, const std::string &);
            /**
             * @brief Add user to channel
             * @param user a shared_ptr to user to be added
             */
            void AddUser (std::shared_ptr<CUser>);
            /**
             * @brief checks if channel is empty
             * @return true if no user is in channel
             */
            bool IsEmpty () const;
            /**
             * @brief Sends command to users in channel
             * @param cmd sends command to all users in channel
             */
            void Send (const Command::CBase &) const;
        
        private:
            std::set<std::shared_ptr<CUser>> users;
            std::string channel;
    };
}
#endif
