#include "ApiPoolResponse.h"
#include "ApiPoolProtocol.h"

namespace ApiPool
{

Response::Response(void)
{

}

Response::~Response(void)
{

}

bool Response::parsePackage(const std::string& package)
{
    clear();

    std::string json_string;
    if (! Protocol::parsePackage(json_string, package))
        return false;

    Json::Reader reader;
    Json::Value root;
    if (! reader.parse(json_string, root))
        return false;

    setResult(root.get(Protocol::getResultKey(), Json::Value::null));
    setError(root.get(Protocol::getErrorKey(), Json::Value::null));
    setId(root.get(Protocol::getIdKey(), Json::Value::null));
    setErrorInfo(root.get(Protocol::getErrorInfoKey(), Json::Value::null));
    return true;
}

bool Response::makePackage(std::string& package)
{
    Json::Value root;
    root[Protocol::getResultKey()] = json_result_;
    root[Protocol::getErrorKey()] = json_error_;
    root[Protocol::getIdKey()] = json_id_;
    root[Protocol::getErrorInfoKey()] = json_info_;

    std::string json_string;
    Json::FastWriter writer;
    json_string = writer.write(root);
    return Protocol::makePackage(package, json_string);
}

void Response::clear()
{
    json_result_ = Json::Value::null;
    json_error_ = Json::Value::null;
    json_id_ = Json::Value::null;
    json_info_ = Json::Value::null;
    json_result_decode_ = Json::Value::null;
}

std::string Response::toString()
{
    std::string result;
    makePackage(result);
    return result;
}

Json::Value Response::getValue() const
{
    Json::Value root;
    root[Protocol::getResultKey()] = json_result_;
    root[Protocol::getErrorKey()] = json_error_;
    root[Protocol::getIdKey()] = json_id_;
    root[Protocol::getErrorInfoKey()] = json_info_;

    return root;
}

const Json::Value& Response::nullValue()
{
    return Json::Value::null;
}

inline const Json::Value& Response::getResult()
{
    return json_result_;
};

inline void Response::setResult(const Json::Value& value)
{
    json_result_ = value;
    if (json_result_.isString()) {
        Json::Reader reader;
        reader.parse(json_result_.asString(), json_result_decode_);
    }
};

const Json::Value& Response::getError()
{
    return json_error_;
};

void Response::setError(const Json::Value& value)
{
    json_error_ = value;
};

const Json::Value& Response::getId()
{
    return json_id_;
};

void Response::setId(const Json::Value& value)
{
    json_id_ = value;
};

const Json::Value& Response::getErrorInfo()
{
    return json_info_;
};

void Response::setErrorInfo(const Json::Value& value)
{
    json_info_ = value;
};

Json::Value Response::getAppErrorCode()
{
    if (json_result_decode_.isNull())
        return 0;

    return json_result_decode_.get(Protocol::getAppErrorCodeKey(), Json::Value::null);
};

void Response::setAppErrorCode(const Json::Value& value)
{
    if (json_result_decode_.isNull())
        return;

    json_result_decode_[Protocol::getAppErrorCodeKey()] = value;
};

Json::Value Response::getAppErrorInfo()
{
    if (json_result_decode_.isNull())
        return 0;

    return json_result_decode_.get(Protocol::getAppErrorInfoKey(), Json::Value::null);
};

void Response::setAppErrorInfo(const Json::Value& value)
{
    if (json_result_decode_.isNull())
        return;

    json_result_decode_[Protocol::getAppErrorInfoKey()] = value;
};

Json::Value Response::getAppData()
{
    if (json_result_decode_.isNull())
        return 0;

    return json_result_decode_.get(Protocol::getAppDataKey(), Json::Value::null);
};

void Response::setAppData(const Json::Value& value)
{
    if (json_result_decode_.isNull())
        return;

    json_result_decode_[Protocol::getAppDataKey()] = value;
};

Json::Value Response::getAppDebugInfo()
{
    if (json_result_decode_.isNull())
        return 0;

    return json_result_decode_.get(Protocol::getAppDebugInfoKey(), Json::Value::null);
};

void Response::setAppDebugInfo(const Json::Value& value)
{
    if (json_result_decode_.isNull())
        return;

    json_result_decode_[Protocol::getAppDebugInfoKey()] = value;
};

void Response::setErrorAndErrorInfo(Error error)
{
    if (error == OK) {
        setError(nullValue());
        setErrorInfo(nullValue());
    } else {
        setError(error);
        setErrorInfo(translateError(error));
    }
}

} // namespace ApiPool

