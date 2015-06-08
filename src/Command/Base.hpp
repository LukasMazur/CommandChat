/**
 * @file src/Command/Base.hpp
 */
#ifndef INC_COMMAND_BASE_HPP
#define INC_COMMAND_BASE_HPP

#include <string>
#include <memory>
#include <map>
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"

namespace Server
{
    class CServer;
}

namespace Client
{
    class CClient;
}
    

namespace Command
{
    /**
     * @brief Abstract base class for commands encapsulation between server and client
     * All commands sent through network are encapsulated by one of derived classes
     * server and client has to no information about string being sent through network
     * all serialization is done in CBase and CFactory
     */
    class CBase
    {
        public:
            /**
             * @param type representing type of command, do no pass directly, instead use CCmd::TYPE
             * @param fields fields of command
             */
            CBase (char, const std::map<std::string, std::string> &);
            CBase (const CBase &) = delete;
            
            /**
             * @brief Prepare string for sending to socket
             * @return std::string with formatted command
             */
            std::string Serialize (void) const;
            /**
             * @brief Process incoming command on server side
             * @param server
             * @param fd Descriptor of user who sent the message
             */
            virtual void Execute (Server::CServer &, int) const = 0;
            /**
             * @brief Process incoming command on client side
             * @param client
             */
            virtual void Execute (Client::CClient &) const = 0;
        
        protected:            
            /**
             * @brief Traslated out_of_range to CInvalidFormatException when invalid format
             * @param str
             * @return field from fields member
             * @throw std::CInvalidFormatException if field is not found
             */
            std::string GetField (const std::string &) const;
            
            const char type;
            const std::map<std::string, std::string> fields;
    };
}

#endif
