
#include "apipoold_configure.h"

#include "AccessControler.h"
#include "options.h"
#include "configure_helper.h"
#include "configure.h"
#include "log.h"

int LoadApipooldConfigure(const char *file, bool is_reload, string sub_item)
{
	utils::Configure configure;

	int rc = configure.load(file);
	if (0 != rc) {
		LOG_FATAL("Configure error, configure.load error");
		return -1;
	}
	
	bool reload_all = false;
	if(!is_reload || sub_item == "all")
	{
		reload_all = true;
	}
	
	if(reload_all || (is_reload && sub_item == "system"))
	{
		if (0 != LoadConfigure_System(configure)) {
			return -1;
		}
	}
	if(is_reload == false){
		if (0 != LoadConfigure_AdminServer(configure)) {
			return -1;
		}
	}

	if(reload_all || (is_reload && sub_item == "apipool")){
		if (0 != LoadConfigure_Apipoold(configure, is_reload)) {
			return -1;
		}
	}


	if(reload_all || (is_reload && sub_item == "phpservice"))
	{
		if (0 != LoadConfigure_phpService(configure, is_reload)) {
			return -1;
		}
	}

	if(reload_all || (is_reload && sub_item == "stateserver"))
	{
		if (0 != LoadConfigure_stateServer(configure, is_reload)) {
			return -1;
		}
	}

	if(is_reload && sub_item == "accessconf")
	{
		if (0 != AccessControler::loadVisitorConfigure()) {
			return -1;
		}
	}
	return 0;
}



