/**
 * @file src/Command/Message.hpp
 */
#ifndef INC_COMMAND_MESSAGE_HPP
#define INC_COMMAND_MESSAGE_HPP

#include "Base.hpp"

namespace Command
{
    /**
     * @brief Handles text messages sent by users
     */
    class CMessage: public CBase
    {
        public:
            using CBase::CBase;
        
            void Execute (Server::CServer &, int) const override;
            void Execute (Client::CClient &) const override;
            
            const char static TYPE;
    };
    
}
#endif
