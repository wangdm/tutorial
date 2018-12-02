/*
 * alexa.h
 *
 *  Created on: Sep 27, 2017
 *      Author: wangdm
 */

#ifndef ALEXA_H_
#define ALEXA_H_

#define PROTOCOL_VERSION          0x01

#define LOGIN_SUCEESS                       0x00
#define INVALID_PARAMETER		  -4
#define INVALID_VERIFY_CODE                 0x01
#define INVALID_VERSION                     0x02
#define LOGIN_TIMEOUT                       0x03
#define INVALID_MSG_LEN                     0x04
#define INVALID_USERTAG                     0x05
#define INTERNAL_LOGIN_ERROR                0x06

#define MAX_MAC_LEN                 12
#define MAX_USERTOKEN_LEN           32
#define MAX_DEVTOKEN_LEN            32
#define MAX_SIGN_LEN                32
#define MAX_DEVTYPE_LEN             16
#define MAX_ROMVERSION_LEN          8
#define MAX_LOGINREP_LEN            12
#define MAX_MESSAGE_LEN             1024

#define MSG_TYPE_DEV_LOGIN        1  //设备登录
#define MSG_TYPE_DEV_LOGOUT       2  //设备注销登录
#define MSG_TYPE_DEV_HEARTBEAT    3  //设备心跳包
#define MSG_TYPE_LOGIN_RESPONSE   4  //设备登录
#define MSG_TYPE_LOGOUT_RESPONSE  5  //设备登录
#define MSG_TYPE_SVR_HEARTBEAT    6  //服务器心跳包
#define MSG_TYPE_ALEXA_CMD        7  //ALexa 命令包

#pragma pack(push)
#pragma pack(1)

struct stBaseMsg
{
    unsigned int m_uLength;
    unsigned char m_cVersion;
    unsigned char m_cMSGType;
};

struct stLoginMsg
{
	stBaseMsg m_Base;
    char m_cMac[MAX_MAC_LEN];
    char m_cUserToken[MAX_USERTOKEN_LEN];
    char m_cDevToken[MAX_DEVTOKEN_LEN];
    char m_cDevType[MAX_DEVTYPE_LEN];
    char m_cRomVersion[MAX_ROMVERSION_LEN];
    char m_cSign[MAX_SIGN_LEN];
};

struct stLoginResponseMsg
{
	stBaseMsg m_Base;
    char m_iCode;
    char m_cResponse[MAX_LOGINREP_LEN];
};

struct stLogoutMsg
{
	stBaseMsg m_Base;
    char m_cMac[MAX_MAC_LEN];
    char m_cUserToken[MAX_USERTOKEN_LEN];
    char m_cDevToken[MAX_DEVTOKEN_LEN];
};

struct stAlexaMsg
{
	stBaseMsg m_Base;
    char m_cCmd[0];
};

#pragma pack(pop)


//the return msg need to delete outside
int FillInBeatHeartMsg(const unsigned char ucMsgType, char *&rpcMsg, int &riMsgLen);
int FillInDevLoginMsg(unsigned char ucMsgType,
                      const char *kpcUserToekn, const char *kpcDevToekn,
                      const char *kpcMac, const char *kpcDevType, const char *kpcRomVersion,
                      char *&rpcMsg, int &riMsgLen);
int FillInDevLogoutMsg(unsigned char ucMsgType,
                      const char *kpcUserToekn, const char *kpcDevToekn,
                      const char *kpcMac, char *&rpcMsg, int &riMsgLen);
int FillInLoginResponseMsg(unsigned char ucMsgType, const char  kiCode,
                           char *&rpcMsg, int &riMsgLen);
int FillInLogoutResponseMsg(unsigned char ucMsgType, const char  kiCode,
                           char *&rpcMsg, int &riMsgLen);
int FillInAlexaMsg(unsigned char ucMsgType,
                      const char *kpcCmd, const int kiCmdLen,
                      char *&rpcMsg, int &riMsgLen);


#endif /* ALEXA_H_ */
