/**
 * @file src/Command/Leave.hpp
 */
#ifndef INC_COMMAND_LEAVE_HPP
#define INC_COMMAND_LEAVE_HPP

#include "Base.hpp"

namespace Command
{
    /**
     * @brief Leaves user from the channel
     */
    class CLeave: public CBase
    {
        public:
            using CBase::CBase;
            
            void Execute (Server::CServer &, int) const override;
            void Execute (Client::CClient &) const override;
            
            const char static TYPE;
    };
    
}
#endif
