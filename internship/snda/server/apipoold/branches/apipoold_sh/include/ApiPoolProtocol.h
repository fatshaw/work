#pragma once

#include <string>

namespace ApiPool
{

class Validator
{
public:
    virtual bool validate(const std::string& value) = 0;
	virtual ~Validator(){};
};

class Protocol
{
    //package
public:
    static int checkPackageHeader(const char* package, size_t len, size_t* p_parse_package_len = NULL);

public:
    static bool checkPackage(const char* package, size_t len, size_t* p_parse_package_len = NULL);
    static bool checkPackage(const std::string& package, size_t* p_parse_package_len = NULL);
    static bool parsePackage(std::string& content, const char* package, size_t len);
    static bool parsePackage(std::string& content, const std::string& package);
    static bool makePackage(std::string& package, const char* content, size_t len);
    static bool makePackage(std::string& package, const std::string& content);

    //Key
public:
    static const char* getRequestSrcKey();
    static const char* getMethodKey();
    static const char* getParamsKey();
    static const char* getIdKey();
    static const char* getVersionKey();
    static const char* getResultKey();
    static const char* getErrorKey();
    static const char* getErrorInfoKey();
    static const char* getAppErrorCodeKey();
    static const char* getAppErrorInfoKey();
    static const char* getAppDataKey();
    static const char* getAppDebugInfoKey();
    static const char* getKeepAliveKey();

public:
    static bool parseServiceNameApiName(const std::string& method, std::string& service_name, std::string& api_name);

    //validator
public:
    static Validator* getRequestSrcValidator();
    static Validator* getMethodValidator();
    static Validator* getServiceNameValidator();
    static Validator* getApiNameValidator();
};

} //namespace ApiPool
