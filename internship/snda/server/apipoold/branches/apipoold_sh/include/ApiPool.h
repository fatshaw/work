#pragma once
#include <string>
#include <vector>
#include <json/json.h>

#include "ApiPoolError.h"
#include "ApiPoolResponse.h"

namespace ApiPool
{

class Connector;
class Request;

class ApiPool
{
public:
    ApiPool();
    ~ApiPool();

private:
    ApiPool(const ApiPool&);
    ApiPool& operator = (const ApiPool&);

public:
    /**
    @brief 
    @fn
    @param request_src 请求来源
    @param service_name 服务名字
    @param api_name 服务Api接口名
    @param param_list 参数列表
    @param json_response 请求返回 json格式
    @return 错误码
    */
    Response request(const std::string& method_name,
		             const Json::Value& params,
                     unsigned int time_out = UINT_MAX);

    /**
    @brief 
    @fn 
    @param request_src 请求来源
    @param service_name 服务名字
    @param api_name 服务Api接口名
    @param param_list 参数列表
    @return 错误码
    */
    void notify(const std::string& method_name,
                const Json::Value& params,
                unsigned int time_out = UINT_MAX);

    /**
    */
    bool keepAlive();

    Response request2(const std::string& method_name,
        const Json::Value& params,
        unsigned int time_out = UINT_MAX,
        Request* pReq = NULL);

    Response rawRequest(const std::string& req, 
                        std::string& res, 
                        unsigned int time_out);

    void rawNotify(const std::string& req, 
                   unsigned int time_out);

public:
    void setServer(const std::string& server, short port, bool long_connect = false);

    void setNofify(bool bTrue);

public:
    static std::string getRequestSrc();

    static void setRequestSrc(const std::string& request_src);

private:
    Connector* connector_;

private:
    static std::string request_src_;
};

}; //namespace ApiPool
