#include "ginxutil.h"
#include <pwd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <string>
#include <memory>
#include <ac/log/log.h>
#include <ac/log/datefilelogger.h>
#include <ac/log/udplogger.h>
#include <ac/log/nulllogger.h>
#include <libconfig/libconfig.h++>
#include <ac_lib/environment.h>

const char* option_format = "f:vh";

std::string  configfile;

static option long_options[] = {
    {"config-file", 1, 0, 'f'},    
    {"version", 0, 0, 'v'},
    {"help", 0, 0, 'h'},
    {NULL, 0, 0, 0}
};

void ShowUsage(const char* program) 
{
    printf("Usage: %s [OPTION]...\n\n", program);
    printf(
        "Mandatory arguments to long options are mandatory for short options too.\n"
        "  -f, --config-file=PATH         init program from a config file\n"
        "  -v, --version                  display version\n"
        "  -h, --help                     display this help and exit\n"
        "\n"
        );
}

#define _STR(s)     #s
void ShowVersion()
{
    printf("ginx server %s \n\n", _STR(GINX_VERSION));
}


int SwitchUser(const char* username)
{
    struct passwd * pw;
    if (getuid()== 0 || geteuid()==0) {
        if ((pw = getpwnam(username)) == 0) {
            AC_ERROR("can't find the user %s to switch to", username);
            return -1;
        }
        if (setgid(pw->pw_gid)<0 || setuid(pw->pw_uid)<0) {
            AC_ERROR("failed to switch to identity of user %s", username);
            return -1;
        }
    }
    return 0;
}

void InitConfig(int argc, char *argv[], libconfig::Config & config)
{
    while(true)
    {
        int option_index = 0;

        int c = getopt_long(argc, argv, option_format, long_options, &option_index);
        if (c == -1) break;

        switch(c)
        {
        case 0: // long option, not need process
            break;
        case 'f':
	 	configfile = optarg;
            config.readFile(optarg);
            if ( config.exists("App.ConfigPath") ) {
                config.lookup("App.ConfigPath") = optarg;
            }
            else {
                config.getRoot()["App"].add("ConfigPath", libconfig::Setting::TypeString) = optarg;
            }
            break;
        case 'v':
            ShowVersion(); exit(0);
        case 'h':
            ShowUsage(argv[0]); exit(0);
        }
    }
}

ac::Logger * CreateLogger(libconfig::Setting & log)
{
    const char * locallog = (const char*)log;

    if ( strlen(locallog) == 0 ) {
        return NULL;
    }

    if ( strncmp(locallog, "udp:", 4) == 0 ) {

        char szBuffer[256] = {0};
        strncpy(szBuffer, locallog+4, sizeof(szBuffer)-1);

        char * pIPBegin = szBuffer;
        char * pIPEnd = strstr(pIPBegin, ":");
        if ( !pIPEnd ) {
            return NULL;
        }
        *pIPEnd = 0;

        char * pPortBegin = pIPEnd + 1;
        if ( !pPortBegin ) {
            return NULL;
        }

        std::string sFormat = ac::Logger::DEFAULT_FORMAT;
        char * pFmtBegin = strstr(pPortBegin, ":");
        if ( pFmtBegin ) {
            sFormat.assign(pFmtBegin + 1);
        }

        return new ac::UdpLogger(ac::INetAddr(pIPBegin, atoi(pPortBegin)), sFormat);
    }
    else {
        char filename[256] = {0};
        strncpy(filename, locallog, sizeof(filename)-1);
        std::string format = ac::Logger::DEFAULT_FORMAT;
        char* fmt = strstr(filename, ":");
        if (fmt) {
            *fmt++ = '\0';
            format = fmt;
        }

        ac::DateFileLogger * pLogger = new ac::DateFileLogger(filename, format);
        if ( pLogger ) {
            pLogger->SetLogFile(filename);
        }
        return pLogger;
    }
}

void HandleExit(int signo)
{
    (void)signo;
    AC_INFO("recv signal[%d] stop environment", signo);
    ac_lib::StopEnvironment();
}
