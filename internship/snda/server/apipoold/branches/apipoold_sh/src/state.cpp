#include <sstream>
#include <string.h>  
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 

#include "apipoold_handler.h"
#include "AccessControler.h"
#include "phpservice_handler.h"

#include "log.h"
#include "options.h"

#include "state.h"

uint32_t ServerState::g_pulse_mainthread = 0;


void ServerState::serialize2shm(time_t current_time)
{
	static int serverstate_shmid = 0; 
	static char *p_serverstate_addr = NULL;
	if(p_serverstate_addr == NULL)
	{
		if((serverstate_shmid=shmget(Options::apipoold.shm_key,STATE_SHM_SIZE,IPC_CREAT | 0666))==-1) 
		{ 
			LOG_FATAL("[ServerState]Create Share Memory for serverstate Error: SHMKEY=%u\n\a", Options::apipoold.shm_key); 
			return;
		} 		
		p_serverstate_addr = (char*)shmat(serverstate_shmid,0,0); 
	}
	
	static int access_dump_shmid = 0; 
	static char *p_access_dump_addr = NULL;
	if(p_access_dump_addr == NULL)
	{
		if((access_dump_shmid = shmget(Options::apipoold.shm_key+1,STATE_SHM_SIZE,IPC_CREAT | 0666))==-1) 
		{ 
			LOG_FATAL("[ServerState]Create Share Memory for accessinfo Error: SHMKEY=%u\n\a", 
						Options::apipoold.shm_key+1); 
			return;
		} 
		p_access_dump_addr = (char*)shmat(access_dump_shmid,0,0); 
	}

	sprintf(p_serverstate_addr, "pulse = %u\n",
		g_pulse_mainthread);

	FcgiAgentHandler::getFcgiAgentHandlerInformation(p_serverstate_addr+strlen(p_serverstate_addr), 
					STATE_SHM_SIZE-strlen(p_serverstate_addr)-1);

	static time_t last_dump_time = 0;
	if(current_time - last_dump_time > 10)
	{
		last_dump_time = current_time;

		AccessControler::dumpVisitorHistory(p_access_dump_addr, 
									STATE_SHM_SIZE-1);
	}
	
	return;
}

