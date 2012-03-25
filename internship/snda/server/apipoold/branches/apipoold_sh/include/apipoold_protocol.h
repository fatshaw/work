#ifndef _APIPOOLD_PROTOCOL_H
#define _APIPOOLD_PROTOCOL_H
#include <stdint.h>

/***********************************************************/
/****<<apipoold与phpfcgiagent服务器通讯协议>>****************/
/***********************************************************/

enum
{
	CMD_FCGIAGENT_KEEPALIVE = 0,
	CMD_FCGIAGENT_RPC = 1,
};





#endif  /* _APIPOOLD_PROTOCOL_H */
