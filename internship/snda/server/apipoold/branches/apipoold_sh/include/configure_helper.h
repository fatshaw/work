
#ifndef _CONFIGURE_HELPER_H_
#define _CONFIGURE_HELPER_H_


#include "configure.h"

int LoadConfigure_AdminServer(utils::Configure& configure);
int LoadConfigure_Apipoold(utils::Configure& configure, bool is_reload = false);
int LoadConfigure_System(utils::Configure& configure);
int LoadConfigure_phpService(utils::Configure& configure, bool is_reload = false);
int LoadConfigure_stateServer(utils::Configure& configure, bool is_reload);

#endif // _CONFIGURE_HELPER_H_
