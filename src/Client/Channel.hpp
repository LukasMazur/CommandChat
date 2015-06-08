#ifndef INC_CLIENT_CHANNEL_HPP
#define INC_CLIENT_CHANNEL_HPP

#include <string>
#include <list>
#include <set>

namespace Client
{
    /**
     * @brief Class representing channel in client
     */
    class CChannel
    {
        public:
            using listMsgs = std::list<std::pair<std::string, std::string>>;
            
            CChannel (void);
            /**
             * @brief Adds a message to channel
             * @param user name of author
             * @param msg message
             */
            void AddMsg (const std::string &, const std::string &);
            /**
             * @brief Returns messages and mark channel as read
             * @return list of messages
             */
            const listMsgs & GetMsgs (void);
            /**
             * @brief Get number of unread messages
             * @return number on unread messages in channel
             */
            int GetUnread (void) const;
            
        private:
            int unread;
            listMsgs msgs;
            
            constexpr static int MSG_MAX = 30;
    };
}
#endif
