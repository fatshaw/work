#ifndef _STATE_H
#define _STATE_H
#include <stdint.h>

#define STATE_SHM_SIZE 102400

class ServerState
{
public:
	// ------------------------------
	
	static uint32_t g_pulse_mainthread;
	static void serialize2shm(time_t current_time);
};

#endif  /* _STATE_H */
