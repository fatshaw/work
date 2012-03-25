#pragma once

#include <json/json.h>

#include "ApiPoolError.h"
#include "ApiPoolProtocol.h"

namespace ApiPool
{

class Response
{
public:
    Response(void);
    ~Response(void);
    
public:
    static const Json::Value& nullValue();

public:
    bool parsePackage(const std::string& package);
    bool makePackage(std::string& package);

public:
    void clear();

public:
    std::string toString();

public:
    Json::Value getValue() const;

public:
    const Json::Value& getResult();
    void setResult(const Json::Value& value);

    const Json::Value& getError();
    void setError(const Json::Value& value);

    const Json::Value& getId();
    void setId(const Json::Value& value);

    const Json::Value& getErrorInfo();
    void setErrorInfo(const Json::Value& value);

    Json::Value getAppErrorCode();
    void setAppErrorCode(const Json::Value& value);

    Json::Value getAppErrorInfo();
    void setAppErrorInfo(const Json::Value& value);

    Json::Value getAppData();
    void setAppData(const Json::Value& value);

    Json::Value getAppDebugInfo();
    void setAppDebugInfo(const Json::Value& value);

public:
    void setErrorAndErrorInfo(Error error);

public:
    Json::Value json_result_;
    Json::Value json_error_;
    Json::Value json_id_;
    Json::Value json_info_;
    Json::Value json_result_decode_;
};

} //namespace ApiPool

