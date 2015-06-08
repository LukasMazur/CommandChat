/**
 * @file src/Command/ChangeName.hpp
 */
#ifndef INC_COMMAND_CHANGENAME_HPP
#define INC_COMMAND_CHANGENAME_HPP

#include "Base.hpp"

namespace Command
{
    /**
     * @brief Handles changing name of user
     */
    class CChangeName: public CBase
    {
        public:
            using CBase::CBase;
            
            void Execute (Server::CServer &, int) const override;
            void Execute (Client::CClient &) const override;
            
            const char static TYPE;
    };
    
}
#endif
