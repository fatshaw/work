#pragma once

#include <json/json.h>

namespace JsonUtil
{

bool valueToString(const Json::Value& value, std::string& result);

std::string valueToString(const Json::Value& value);

}; //namespace JsonUtil

