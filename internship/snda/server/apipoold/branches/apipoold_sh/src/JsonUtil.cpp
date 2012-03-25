#include "JsonUtil.h"

namespace JsonUtil
{

bool valueToString(const Json::Value& value, std::string& result)
{
    switch ( value.type() )
    {
    case Json::nullValue:
        result = "null";
        break;
    case Json::intValue:
        result = Json::valueToString( value.asInt() );
        break;
    case Json::uintValue:
        result = Json::valueToString( value.asUInt() );
        break;
    case Json::realValue:
        result = Json::valueToString( value.asDouble() );
        break;
    case Json::stringValue:
        result = value.asCString();
        break;
    case Json::booleanValue:
        result = Json::valueToString( value.asBool() );
        break;
    case Json::arrayValue:
        {
            std::string string_value;
            int size = value.size();
            for ( int index =0; index < size; ++index )
            {
                if ( index > 0 )
                    result += ",";
                valueToString(value[index], string_value);
                result += string_value;
            }
        }
        break;
    case Json::objectValue:
        {
            Json::FastWriter writer;
            result = writer.write(value);
        }
        break;
    }

    return true;
}

std::string valueToString(const Json::Value& value)
{
    std::string result;
    valueToString(value, result);
    return result;
}

}; //namespace JsonUtil

