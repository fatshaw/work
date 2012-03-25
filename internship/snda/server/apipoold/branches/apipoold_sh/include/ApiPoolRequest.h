#pragma once

#include <json/json.h>

#include "ApiPoolError.h"
#include "ApiPoolProtocol.h"

namespace ApiPool
{

class Request
{
public:
    Request();
    ~Request();

    typedef std::pair<std::string, std::string> Param;
    typedef std::vector<Param> ParamList;

public:
    std::string getRequestSrc();
    std::string getServiceName();
    std::string getApiName();
    Json::Value getId();
    std::string getVersion();
    void setId(const Json::Value& value);
    size_t getParamSize();
    Json::Value getParam(const std::string& key);
    ParamList getParams();

public:
    bool isKeepAlive();

public:
    std::string getIp();
    void setIp(const std::string& ip);
    size_t getPort();
    void setPort(size_t port);
    size_t getNetId();
    void setNetId(size_t net_id);

public:
    Json::Value getValue();

public:
    bool parsePackage(const char* buffer, size_t buffer_len);
    bool parsePackage(const std::string& package);
    bool makePackage(std::string& package);

public:
    std::string toString();
    void clear();

protected:
    void loadRequestSrc();
    void loadId();
    void loadVersion();
    void loadServiceNameApiName();
    void loadParamList();

public:
    bool build(const std::string& request_src,
               const std::string& method_name,
               const Json::Value& params,
               Error* error = 0);

    bool buildKeepAlive();

private:
    Json::Value json_root_;
    std::string request_src_;
    std::string service_name_;
    std::string api_name_;
    Json::Value id_;
    std::string version_;
    Json::Value params_;
    bool request_src_loaded_;
    bool service_name_api_name_loaded_;
    bool params_loaded_;
    bool id_loaded_;
    bool version_loaded_;
    std::string ip_;
    size_t port_;
    size_t net_id_;
};

} //namespace ApiPool
