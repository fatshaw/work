#ifndef _PROTOCOL_H
#define _PROTOCOL_H

//用户消息最大长度为800byte，已和客户端商定
#define MAX_USER_MESSAGE_LEN (800+100)
//按照现在的编码方法，一个byte会被编成一个2位的16进制数
#define MAX_MESSAGE_STRUCT_LEN (MAX_USER_MESSAGE_LEN*2+400)
//一次最大取的消息的个数
#define MAX_FETCH_MSG_COUNT	50
//一次最大发回的响应的大小
#define MAX_SEND_DATA_LEN (MAX_MESSAGE_STRUCT_LEN*MAX_FETCH_MSG_COUNT)

#pragma pack(1)
typedef enum
{
	RET_OK = 0,
	RET_CONFIG_LOAD_FAIL = 1,
	RET_NET_LISTEN_FAIL  = 2,
	RET_NET_CONNECT_FAIL  = 3,
	RET_NET_MANAGER_START_FAIL  = 4,
	RET_NO_MEMORY  = 5,
	RET_CONTINUE	= 6,
	RET_ILLEGAL_CLIENT,
	RET_UNKOWN_SOURCE,
	RET_UKNOWN_CMD,
	RET_APP_NET_SEND_ERROR,
	RET_APP_NET_RECV_ERROR,
	RET_UNKNOW_PARAM,
	RET_SYS_BUSY,
	RET_FAIL,
	RET_QUEUE_PUSH_FAIL,
	RET_QUEUE_POP_FAIL,
	RET_DB_ERROR,
	RET_DATA_NOT_SATISFIED,
}ERR_CODE;


//! 协议命令字
enum WEB_CMD
{
	WEB_GET_USER_MSG = 1,
	WEB_GET_SPECIED_USER_MSG = 2,
	WEB_DEL_USER_MSG = 3,
	WEB_DEL_SPECIED_USER_MSG = 4,
	WEB_GET_SUM = 5,
	WEB_GET_SPECIED_USER_SUM = 6,
};

//! 协议命令字
enum CONNECTD_CMD
{
	CONNECTD_USER_MSG = 1,
	CONNECTD_KEEP_ALIVE = 0,
};

/*
typedef struct 
{
	uint16_t len;
	uint16_t cmd;
	uint16_t ver;
	uint32_t uin;		//在KeepAlive包时仅为占位符
}CONNECTD_HDR;*/

struct FCGI_AGENT_HDR
{
	uint32_t len;
	uint16_t  cmd;
	uint16_t  ver;
	uint32_t seq;
};

typedef struct 
{
	char version;
	char type;
	unsigned short requestID;
	unsigned short contentLength;
	char paddingLength;
	char reserved;
	
}FCGI_HDR;
/*
typedef struct 
{
	uint16_t length;
	uint16_t cmd;
	uint16_t record_id;
	
}CONNECTD_HDR;
*/

enum
{
	SAVE_FLAG_DISABLE = 0,	//0代表从来没打开过在线消息记录功能
	SAVE_FLAG_ENABLE = 1,	//1代表当前在线消息记录功能是打开的
	SAVE_FLAG_DISABLE_AGAIN = 2,	//2代表曾经打开过在线消息记录功能，但现在是关闭的。
};

enum
{
	REC_STATUS_AVAILABE = 0,
	REC_STATUS_UNAVAILABE = 1,
	REC_STATUS_DELETED = 2,
};

/*bitmask(uint32)+ ver(uint16) + msg_seq(uint32) + msg*/
#define SKIP_BYTE_USER_MSG	(sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint32_t)+sizeof(uint16_t))


#pragma pack()

#endif  /* _PROTOCOL_H */
