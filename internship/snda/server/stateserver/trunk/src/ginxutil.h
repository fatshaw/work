#ifndef _GINX_UTIL_
#define _GINX_UTIL_
namespace libconfig{
    struct Config;
    struct Setting;
}
namespace ac{
    class Logger;
}



void ShowUsage(const char* program);

void ShowVersion();

int SwitchUser(const char* username);

void InitConfig(int argc, char *argv[], libconfig::Config & config);

ac::Logger * CreateLogger(libconfig::Setting & log);

void HandleExit(int signo);

#endif
