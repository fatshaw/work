#ifndef GINX_ACCEPTORMANAGER_H__
#define GINX_ACCEPTORMANAGER_H__

#include <vector>

namespace libconfig
{
class Setting;
}

namespace ac_lib
{
class NetworkAcceptor;
class NetworkOption;
namespace translator
{
class Translator;
}
}

class AcceptorManager
{
public:
    int Open(libconfig::Setting &);
    void Close();

private:
    int parseTranslator(int protocol, const std::string& serviceName, ac_lib::NetworkOption& option);
    std::vector<ac_lib::translator::Translator*> vTranslators_;
};

#endif
