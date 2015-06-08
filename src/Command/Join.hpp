/**
 * @file src/Command/Join.hpp
 */
#ifndef INC_COMMAND_JOIN_HPP
#define INC_COMMAND_JOIN_HPP

#include "Base.hpp"

namespace Command
{
    /**
     * @brief Joins user to the channel
     */
    class CJoin: public CBase
    {
        public:
            using CBase::CBase;
            
            void Execute (Server::CServer &, int) const override;
            void Execute (Client::CClient &) const override;
            
            const char static TYPE;
    };
    
}
#endif
