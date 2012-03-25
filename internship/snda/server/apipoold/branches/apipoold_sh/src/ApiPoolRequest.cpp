#include "ApiPoolRequest.h"
#include "ApiPoolProtocol.h"
#include "ApiPoolUtil.h"
#include "ApiPoolVersion.h"
#include "JsonUtil.h"

namespace ApiPool
{

Request::Request()
:
request_src_loaded_(false),
service_name_api_name_loaded_(false),
params_loaded_(false),
id_loaded_(false)
{
};

Request::~Request()
{
};

void Request::clear()
{
    json_root_.clear();
    request_src_.clear();
    service_name_.clear();
    api_name_.clear();
    version_.clear();
    params_.clear();
    request_src_loaded_ = false;
    service_name_api_name_loaded_ = false;
    params_loaded_ = false;
    id_loaded_ = false;
    version_loaded_ = false;
};

void Request::loadRequestSrc()
{
    if (! request_src_loaded_) {
        request_src_.clear();
        Json::Value v = json_root_.get(Protocol::getRequestSrcKey(), Json::Value::null);
        request_src_ = JsonUtil::valueToString(v);
        request_src_loaded_ = true;
    }
}

void Request::loadId()
{
    if (! id_loaded_) {
        id_.clear();
        id_ = json_root_.get(Protocol::getIdKey(), Json::Value::null);
        id_loaded_ = true;
    }
}

void Request::loadVersion()
{
    if (! version_loaded_) {
        version_.clear();
        Json::Value v = json_root_.get(Protocol::getVersionKey(), Json::Value::null);
        version_ = JsonUtil::valueToString(v);
        version_loaded_ = true;
    }
}

void Request::loadServiceNameApiName()
{
    if (! service_name_api_name_loaded_) {
        service_name_.clear();
        api_name_.clear();
        Json::Value v = json_root_.get(Protocol::getMethodKey(), Json::Value::null);
        if (v.isString()) {
            std::string method = v.asCString();
            Protocol::parseServiceNameApiName(method, service_name_, api_name_);
        }
        service_name_api_name_loaded_ = true;
    }
}

void Request::loadParamList()
{
    if (! params_loaded_) {
        params_.clear();
        params_ = json_root_.get(Protocol::getParamsKey(), Json::Value::null);
        params_loaded_ = true;
    }
}

std::string Request::getRequestSrc()
{
    loadRequestSrc();

    return request_src_;
}

std::string Request::getServiceName()
{
    loadServiceNameApiName();

    return service_name_;
}

std::string Request::getApiName()
{
    loadServiceNameApiName();

    return api_name_;
}

Json::Value Request::getId()
{
    loadId();

    return id_;
}

std::string Request::getVersion()
{
    loadVersion();

    return version_;
}

void Request::setId(const Json::Value& value)
{
    json_root_[Protocol::getIdKey()] = value;

    id_loaded_ = true;
}

size_t Request::getParamSize()
{
    loadParamList();

    return params_.size();
}

Json::Value Request::getParam(const std::string& key) 
{
    loadParamList();

    return params_.get(key, Json::Value::null);
}

Request::ParamList Request::getParams()
{
    loadParamList();

    ParamList result;
    Json::Value::iterator iter = params_.begin(),
        last = params_.end();
    for (; iter!=last; iter++) {
        result.push_back(Param(JsonUtil::valueToString(iter.key()), JsonUtil::valueToString(*iter)));
    }

    return result;
}

bool Request::isKeepAlive()
{
    Json::Value v = json_root_.get(Protocol::getVersionKey(), Json::Value::null);
    return ! v.isNull();
}

Json::Value Request::getValue()
{
    return json_root_;
};

std::string Request::getIp()
{
	return ip_;
}

void Request::setIp(const std::string& ip)
{
	ip_ = ip;
}

size_t Request::getPort()
{
	return port_;
}

void Request::setPort(size_t port)
{
	port_ = port;
}

size_t Request::getNetId()
{
	return net_id_;
}

void Request::setNetId(size_t net_id)
{
	net_id_ = net_id;
}

bool Request::build(const std::string& request_src,
                    const std::string& method_name,
                    const Json::Value& params,
                    Error* error)
{
    Error err = OK;
    AssignBeforeReturn<Error, Error> assign_error(error, &err);

    if (! Protocol::getRequestSrcValidator()->validate(request_src)) {
        err = ERR_REQ;
        return false;
    }

    if (! Protocol::getMethodValidator()->validate(method_name)) {
        err = ERR_REQ;
        return false;
    }

    clear();

    json_root_[Protocol::getRequestSrcKey()] = Json::Value(request_src);
    json_root_[Protocol::getMethodKey()] = Json::Value(method_name);
    json_root_[Protocol::getIdKey()] = Json::Value::null;
    json_root_[Protocol::getVersionKey()] = ApiPool::getVersion();
    json_root_[Protocol::getParamsKey()] = params;

    return true;
}

bool Request::buildKeepAlive()
{
    clear();
    json_root_[Protocol::getKeepAliveKey()] = Json::Value((bool)true);
    return true;
}

bool Request::parsePackage(const char* buffer, size_t buffer_len)
{
	clear();

	std::string json_string;
	if (! Protocol::parsePackage(json_string, buffer, buffer_len))
		return false;

    Json::Reader reader;
    return reader.parse(json_string, json_root_);
}

bool Request::parsePackage(const std::string& package)
{
    return parsePackage(package.c_str(), package.length());
}

bool Request::makePackage(std::string& package)
{
    Json::FastWriter writer;
    std::string json_string = writer.write(json_root_);
    if (! Protocol::makePackage(package, json_string))
        return false;

    return true;
}

std::string Request::toString()
{
    std::string package;
    makePackage(package);
    return package;
};

} //namespace ApiPool
