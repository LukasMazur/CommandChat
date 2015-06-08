/**
 * @file src/Command/Factory.hpp
 */
#ifndef INC_COMMAND_FACTORY_HPP
#define INC_COMMAND_FACTORY_HPP

#include <string>
#include <unordered_map>
#include <memory>
#include <sstream>
#include "Base.hpp"

namespace Command
{
    /**
     * @brief Factory class for creating and parsing commands objects
     * No changing of this class is needed when adding new commands.
     * This class provides means to register new CBase derived class without modifaction of this class
     */
    class CFactory
    {
        public:
            /**
            * @brief This class is static only
            */
            CFactory () = delete;

            /**
             * @brief it is used when message has been received from network and has to be serialized
             * @param strem stream data received from network
             * @throw std::CInvalidFormatException if received data is invalid
             */
            static std::unique_ptr<CBase> Create (std::stringstream &);
            /**
             * @brief it is used mainly when command is created for sending to other party
             * @param char type should not be used directly, use CCmd::TYPE
             * @param fields
             * @throw std::CInvalidFormatException if received data is invalid 
             */
            static std::unique_ptr<CBase> Create (char, const std::map<std::string, std::string> &);
            /**
             * @tparam one of CBase derived classes
             * @param type this type has to be unique for every class registered with this class
             * @return same char as pass as type
             */
            template <typename T>
            char static Register (char type);
            
            constexpr static char DELIMITER = '\0';
            constexpr static char CMD_END = '\n';
            
        private:
            using pCreate = std::function<std::unique_ptr<CBase>(char, std::map<std::string, std::string>)>;
            using tokens = std::pair<char, std::map<std::string, std::string>>;
    
            static std::string GetLine (std::stringstream &);
            static tokens Tokenize (const std::string &);
            
            template <typename T>
            static std::unique_ptr<CBase> CreateType (char type, const std::map<std::string, std::string> & fields);
            
            static std::unordered_map<char, pCreate> & GetTypes ();
    };
    
    // template method implementation ----------------------------------
    template <typename T>
    char CFactory::Register (char type)
    {
        return GetTypes().insert(make_pair(type, &CreateType<T>)).first->first;
    }
    
    template <typename T>
    std::unique_ptr<CBase> CFactory::CreateType (char type, const std::map<std::string, std::string> & fields)
    {
        return std::unique_ptr<CBase>(new T(type, fields));
    }
    // -----------------------------------------------------------------
    
    class CInvalidFormatException: public std::runtime_error
    {
        public:
            CInvalidFormatException (const std::string &);
    };
}
#endif

