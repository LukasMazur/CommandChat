#include <stdexcept>
#include <sstream>
#include <algorithm>
#include "Factory.hpp"

using namespace std;
using namespace Command;

unique_ptr<CBase> CFactory::Create (stringstream & stream)
{
    auto tok = Tokenize(GetLine(stream));
    
    return Create (tok.first, tok.second);
}

unique_ptr<CBase> CFactory::Create (char type, const map<string, string> & fields)
{
    try {
        return GetTypes().at(type)(type, fields);
    } catch (out_of_range) {
        throw CInvalidFormatException("Invalid message identifier");
    }
}

string CFactory::GetLine (stringstream & buf)
{
    string str;

    if (getline(buf, str, CMD_END) && !buf.eof()) {
        return str;
    }
    buf.write(str.c_str(), str.size()); // message is not complete yet
    return string();
}

CFactory::tokens CFactory::Tokenize (const string & cmd)
{
    auto begin = cmd.cbegin() + 1; // ommiting the first character indicating command
    map<string, string> retVal;
    
    while (begin != cmd.cend()) {
        auto it = find(begin, cmd.cend(), DELIMITER);
        if (it == cmd.cend()) {
            //return retVal;
            throw CInvalidFormatException(cmd);
        }
        auto end = find(it + 1, cmd.cend(), DELIMITER);
        retVal.insert(make_pair(string(begin, it), string(it + 1, end)));
        begin = end + 1;
    }
    
    return make_pair(cmd.front(), retVal);
}

unordered_map<char, CFactory::pCreate> & CFactory::GetTypes ()
{
    static unordered_map<char, pCreate> types;
    
    return types;
}

CInvalidFormatException::CInvalidFormatException (const string & str):
    runtime_error(str)
{}

constexpr char CFactory::DELIMITER;
constexpr char CFactory::CMD_END;
