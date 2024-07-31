#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
typedef unsigned int uint;

#define REGIST_OK "regist ok"
#define REGIST_FAILED "regist failed: name existed"
#define LOGIN_OK "login ok"
#define LOGIN_FAILED "login failed: name or password might existed or wrong"

#define SEARCH_USR_NO "no such person"
#define SEARCH_USR_ONLINE "online"
#define SEARCH_USR_OFFLINE "offline"

#define UNKOWN_ERROR "unkown Error"
#define EXISTED_FRIEND "friend exist"
#define ADD_FRIEND_OFFLINE "user offline"
#define ADD_FRIEND_NONEXIST "user not exit"
enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN = 0,
    ENUM_MSG_TYPE_REGIST_REQUEST,
    ENUM_MSG_TYPE_REGIST_RESPOND,
    ENUM_MSG_TYPE_LOGIN_REQUEST,
    ENUM_MSG_TYPE_LOGIN_RESPOND,
    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND,
    ENUM_MSG_TYPE_SEARCH_USR_REQUEST,
    ENUM_MSG_TYPE_SEARCH_USR_RESPOND,
    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,
    ENUM_MSG_TYPE_ADD_FRIEND_AGREE,
    ENUM_MSG_TYPE_ADD_FRIEND_REFUSE,
    ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST,
    ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND,
    // ENUM_MSG_TYPE_REQUEST,
    // ENUM_MSG_TYPE_ RESPOND,
    ENUM_MSG_TYPE_MAX = 0x00ffffff
};

struct PDU
{
    uint uiPDULen; // total unit number
    uint uiMsgType; // message type
    char caData[64];
    uint uiMsgLen; // message length
    int caMsg[]; //message
};

PDU *mkPDU(uint uiMsgLen);

#endif // PROTOCOL_H
