#pragma once

namespace ApiPool
{

#define API_POOL_ERROR_LIST(V) \
    /*Ok*/ \
    V(OK, 200, "Ok") \
    /*Client Error*/ \
    V(ERR_STATUS_BAD_REQUEST, 400, "Bad Client Request") \
    V(ERR_STATUS_UNAUTHORIZED, 401, "Request Source Authorization Check Failed") \
    V(ERR_STATUS_FORBIDDEN, 403, "Request Forbidden") \
    V(ERR_STATUS_NOT_FOUND, 404, "Service Not Found") \
    V(ERR_STATUS_BAD_PACKET_DATA, 405, "Request Packet Data is not authentic") \
    V(ERR_STATUS_BAD_SERVICE_NAME, 406, "Request Service Name is error") \
    V(ERR_REQUEST_TIMEOUT, 407, "Request Timeout") \
    /*Server Error*/ \
    V(ERR_STATUS_INTERNAL, 500, "Internal Server Error") \
    V(ERR_STATUS_NOT_IMPLEMENTED, 501, "Service Not Implemented") \
    V(ERR_STATUS_BAD_GATEWAY, 502, "Bad Gateway") \
    V(ERR_STATUS_UNAVAILABLE, 503, "Service Unavailable") \
    V(ERR_INTERNAL_ERROR, 504, "Internal Error") \
    V(ERR_PROTOCOL_ERROR, 505, "Request Invalid Protocol") \
    /*SDK Error*/ \
    V(ERR_UNKNOWN, 600, "Unknown Error") \
    V(ERR_REQ, 601, "Request Format Error") \
    V(ERR_RPC, 602, "RPC Error") \
    V(ERR_RES, 603, "Response Format Error") \
    V(ERR_TIMEOUT, 604, "Client Time Out") \
    V(ERR_CONNECT_FAIL, 605, "Cannot Connect Server") \
    V(ERR_CONNECTION_ERROR, 606, "Connection Error") \
    V(ERR_SERVER_CLOSE, 607, "Server Close The Connect") \
    V(ERR_CLIENT_CLOSE, 608, "Client Close") \


    enum Error
    {
#define API_POOL_ENUM(enumeration, code, description) \
    enumeration = code, 
    API_POOL_ERROR_LIST(API_POOL_ENUM)
#undef API_POOL_ENUM
    };


    inline const char* translateError(unsigned int error)
    {
        switch(error) 
        {
#define API_POOL_DESCRIPTION(enumeration, code, description) \
            case code : return description;
            API_POOL_ERROR_LIST(API_POOL_DESCRIPTION)
#undef API_POOL_DESCRIPTION
            default: return "unknown error";
        }
    };

} //namespace ApiPool
