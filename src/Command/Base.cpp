#include <algorithm>
#include <sstream>
#include <stdexcept>
#include "Base.hpp"
#include "Factory.hpp"

using namespace std;
using namespace Command;

CBase::CBase (char type, const map<string, string> & fields):
    type(type),
    fields(fields)
{}

string CBase::Serialize (void) const
{
    ostringstream os;
    
    os << type;
    for (const auto & i : fields) {
        os << i.first << CFactory::DELIMITER << i.second << CFactory::DELIMITER;
    }
    os << CFactory::CMD_END;
    
    return os.str();
}

string CBase::GetField (const string & str) const
{
    try {
        return fields.at(str);
    } catch (out_of_range e) {
        throw CInvalidFormatException("Field " + str + " not found");
    }
}
