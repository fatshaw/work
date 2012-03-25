#include "ApiPoolProtocol.h"
#include "ApiPoolUtil.h"

namespace ApiPool
{

int Protocol::checkPackageHeader(const char* package, size_t len, size_t* p_parse_package_len)
{
    size_t parse_package_len = 0;
    AssignBeforeReturn<size_t, size_t> assign(p_parse_package_len, &parse_package_len);

    //检查是否是16进制字符
    const size_t head_size = (len > 6) ? (6) : (len);
    int header[6] = {0};
    for (size_t i=0; i<head_size; i++) {
        int c = tolower(package[i]);
        if (c < '0' || c > 'f'|| (c > '9' && c < 'a'))
            return -1;
        header[i] = c;
    }

    if (head_size != 6)
        return 1;

    size_t base = 16 * 16 * 16 * 16 * 16;
    for (size_t i=0; i<head_size; i++) {
        int c = header[i];
        int d = (c > '9') ? (c - 'a' + 10) : (c - '0');
        parse_package_len += d * base;
        base /= 16;
    }

    parse_package_len += 6;
    return (parse_package_len <= len) ? (0) : (1);
}

bool Protocol::checkPackage(const char* package, size_t len, size_t* p_parse_package_len)
{
    int check_header = checkPackageHeader(package, len, p_parse_package_len);
    return (check_header >= 0);
}

bool Protocol::checkPackage(const std::string& package, size_t* p_parse_package_len)
{
    return checkPackage(package.c_str(), package.size(), p_parse_package_len);
}

bool Protocol::parsePackage(std::string& content, const char* package, size_t len)
{
    size_t parse_package_len = 0;
    int check_header = checkPackageHeader(package, len, &parse_package_len);
    if (check_header != 0)
        return false;

    content.assign(package + 6, parse_package_len - 6);
    return true;
}

bool Protocol::parsePackage(std::string& content, const std::string& package)
{
    return parsePackage(content, package.c_str(), package.size());
}

bool Protocol::makePackage(std::string& package, const char* content, size_t len)
{
    size_t package_len = len;
    if (package_len > 0xFFFFFF)
        return false;
    char str_len[16] = {0};
#ifdef _WIN32
    sprintf_s(str_len, sizeof(str_len) - 1, "%06x", package_len);
#else
    snprintf(str_len, sizeof(str_len) - 1, "%06x", (unsigned int)package_len);
#endif
    package.assign(content, len);
    package = str_len + package;
    return true;
}

bool Protocol::makePackage(std::string &package, const std::string &content)
{
    return makePackage(package, content.c_str(), content.size());
}

const char* Protocol::getRequestSrcKey()
{
    return "src";
}

const char* Protocol::getMethodKey()
{
    return "method";
}

const char* Protocol::getParamsKey()
{
    return "params";
}

const char* Protocol::getIdKey()
{
    return "id";
}

const char* Protocol::getVersionKey()
{
    return "version";
}

const char* Protocol::getResultKey()
{
    return "result";
}

const char* Protocol::getErrorKey()
{
    return "error";
}

const char* Protocol::getErrorInfoKey()
{
    return "errorinfo";
}

const char* Protocol::getAppErrorCodeKey()
{
    return "apperrcode";
}

const char* Protocol::getAppErrorInfoKey()
{
    return "errorinfo";
}

const char* Protocol::getAppDataKey()
{
    return "appresdata";
}

const char* Protocol::getAppDebugInfoKey()
{
    return "debuginfo";
}

const char* Protocol::getKeepAliveKey()
{
    return "alive";
}

bool Protocol::parseServiceNameApiName(const std::string& method, std::string& service_name, std::string& api_name)
{
    size_t dot_pos = method.find('.');
    if (dot_pos != std::string::npos) {
        service_name.assign(method.begin(), method.begin() + dot_pos);
        api_name.assign(method.begin() + dot_pos + 1, method.end());
    } else {
        service_name = method;
        api_name = "";
    }

    return (dot_pos != std::string::npos);
}

class AlwaysTrueValidator :
    public Validator 
{
public:
    virtual bool validate(const std::string& value)
    {
        return true;
    }
};

class NoDotValidator :
    public Validator
{
public:
    virtual bool validate(const std::string& value)
    {
        return value.find('.') == std::string::npos;
    }
};

class MaxSizeValidator :
    public Validator
{
public:
    MaxSizeValidator(size_t max_size)
        :
    max_size_(max_size)
    {};

public:
    virtual bool validate(const std::string& value)
    {
        return (value.size() < max_size_);
    };

private:
    size_t max_size_;
};

class MinSizeValidator :
    public Validator
{
public:
    MinSizeValidator(size_t min_size)
        :
    min_size_(min_size)
    {};

public:
    virtual bool validate(const std::string& value)
    {
        return (value.size() >= min_size_);
    };

private:
    size_t min_size_;
};

class AndValidator :
    public Validator
{
public:
    AndValidator(Validator* validator1, Validator* validator2)
    :
    validator1_(validator1),
    validator2_(validator2)
    {};

public:
    virtual bool validate(const std::string& value)
    {
        return (validator1_->validate(value) && validator2_->validate(value));
    };

private:
    Validator* validator1_;
    Validator* validator2_;
};

class OrValidator :
    public Validator
{
public:
    OrValidator(Validator* validator1, Validator* validator2)
    :
    validator1_(validator1),
    validator2_(validator2)
    {};

public:
    virtual bool validate(const std::string& value)
    {
        return (validator1_->validate(value) || validator2_->validate(value));
    };

private:
    Validator* validator1_;
    Validator* validator2_;
};

class NotValidator :
    public Validator
{
public:
    NotValidator(Validator* validator)
        :
    validator_(validator)
    {};

public:
    virtual bool validate(const std::string& value)
    {
        return (! validator_->validate(value));
    };

private:
    Validator* validator_;
};

Validator* Protocol::getRequestSrcValidator()
{
    static AlwaysTrueValidator validator;
    return &validator;
}

Validator* Protocol::getMethodValidator()
{
    static AlwaysTrueValidator validator;
    return &validator;
}

Validator* Protocol::getServiceNameValidator()
{
    static NoDotValidator no_dot_validator;
    static MaxSizeValidator max_size_validator(33);
    static MinSizeValidator min_size_validator(1);
    static AndValidator and_0_validator(&no_dot_validator, &max_size_validator);
    static AndValidator validator(&and_0_validator, &min_size_validator);
    return &validator;
}

Validator* Protocol::getApiNameValidator()
{
    static NoDotValidator no_dot_validator;
    static MaxSizeValidator max_size_validator(33);
    static MinSizeValidator min_size_validator(1);
    static AndValidator and_0_validator(&no_dot_validator, &max_size_validator);
    static AndValidator validator(&and_0_validator, &min_size_validator);
    return &validator;
}

} //namespace ApiPool
