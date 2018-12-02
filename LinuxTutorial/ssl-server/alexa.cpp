/*
 * alexa.c
 *
 *  Created on: Sep 27, 2017
 *      Author: wangdm
 */


#include "alexa.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>


int FillInBeatHeartMsg(const unsigned char ucMsgType, char *&rpcMsg, int &riMsgLen)
{
    int iRet = 0;

	stBaseMsg *pMsg = new stBaseMsg;

	memset(pMsg, 0, sizeof(stBaseMsg));
	pMsg->m_uLength = htonl(sizeof(stBaseMsg));
	pMsg->m_cVersion = PROTOCOL_VERSION;
	pMsg->m_cMSGType = ucMsgType;
	rpcMsg = (char *)pMsg;
	riMsgLen = sizeof(stBaseMsg);

    return iRet;
}

int FillInDevLoginMsg(unsigned char ucMsgType, const char *kpcUserToekn, const char *kpcDevToekn,
                      const char *kpcMac, const char *kpcDevType, const char *kpcRomVersion,
                      char *&rpcMsg, int &riMsgLen)
{
    int iRet = 0;

	stLoginMsg *pMsg = new stLoginMsg;

	memset(pMsg, 0, sizeof(stLoginMsg));
	pMsg->m_Base.m_uLength = htonl(sizeof(stLoginMsg));
	pMsg->m_Base.m_cVersion = PROTOCOL_VERSION;
	pMsg->m_Base.m_cMSGType = ucMsgType;
	strncpy(pMsg->m_cUserToken, kpcUserToekn, sizeof(pMsg->m_cUserToken));
	strncpy(pMsg->m_cDevToken, kpcDevToekn, sizeof(pMsg->m_cDevToken));
	strncpy(pMsg->m_cMac, kpcDevType, sizeof(pMsg->m_cMac));
	strncpy(pMsg->m_cDevType, kpcMac, sizeof(pMsg->m_cDevType));
	strncpy(pMsg->m_cRomVersion, kpcRomVersion, sizeof(pMsg->m_cRomVersion));
	rpcMsg = (char *)pMsg;
	riMsgLen = sizeof(stLoginMsg);

    return iRet;
}

int FillInDevLogoutMsg(unsigned char ucMsgType, const char *kpcUserToekn, const char *kpcDevToekn,
                      const char *kpcMac, char *&rpcMsg, int &riMsgLen)
{
    int iRet = 0;

	stLoginMsg *pMsg = new stLoginMsg;

	memset(pMsg, 0, sizeof(stLogoutMsg));
	pMsg->m_Base.m_uLength = htonl(sizeof(stLogoutMsg));
	pMsg->m_Base.m_cVersion = PROTOCOL_VERSION;
	pMsg->m_Base.m_cMSGType = ucMsgType;
	strncpy(pMsg->m_cUserToken, kpcUserToekn, sizeof(pMsg->m_cUserToken));
	strncpy(pMsg->m_cDevToken, kpcDevToekn, sizeof(pMsg->m_cDevToken));
	strncpy(pMsg->m_cDevType, kpcMac, sizeof(pMsg->m_cDevType));
	rpcMsg = (char *)pMsg;
	riMsgLen = sizeof(stLoginMsg);

    return iRet;
}

int FillInLoginResponseMsg(unsigned char ucMsgType, const char kiCode,
                           char *&rpcMsg, int &riMsgLen)
{
	int iRet = 0;

	stLoginResponseMsg *pMsg = new stLoginResponseMsg;

	memset(pMsg, 0, sizeof(stLoginResponseMsg));
	pMsg->m_Base.m_uLength = htonl(sizeof(stLoginResponseMsg));
	pMsg->m_Base.m_cVersion = PROTOCOL_VERSION;
	pMsg->m_Base.m_cMSGType = ucMsgType;
	pMsg->m_iCode = kiCode;
	rpcMsg = (char *)pMsg;
	riMsgLen = sizeof(stLoginResponseMsg);

    return iRet;
}

int FillInLogoutResponseMsg(unsigned char ucMsgType, const char  kiCode,
                           char *&rpcMsg, int &riMsgLen)
{
	int iRet = 0;

	stLoginResponseMsg *pMsg = new stLoginResponseMsg;

	memset(pMsg, 0, sizeof(stLoginResponseMsg));
	pMsg->m_Base.m_uLength = htonl(sizeof(stLoginResponseMsg));
	pMsg->m_Base.m_cVersion = PROTOCOL_VERSION;
	pMsg->m_Base.m_cMSGType = ucMsgType;
	pMsg->m_iCode = kiCode;
	rpcMsg = (char *)pMsg;
	riMsgLen = sizeof(stLoginResponseMsg);

	return iRet;
}

int FillInAlexaMsg(unsigned char ucMsgType, const char *kpcCmd, const int kiCmdLen,
                      char *&rpcMsg, int &riMsgLen)
{
	int iRet = 0;

	int size = sizeof(stAlexaMsg) + kiCmdLen;
	char* p = new char[size];
	stAlexaMsg *pMsg = (stAlexaMsg *)p;

	memset(pMsg, 0, sizeof(stAlexaMsg));
	pMsg->m_Base.m_uLength = htonl(size);
	pMsg->m_Base.m_cVersion = PROTOCOL_VERSION;
	pMsg->m_Base.m_cMSGType = ucMsgType;
	memcpy(pMsg->m_cCmd, kpcCmd, kiCmdLen);
	rpcMsg = (char *)pMsg;
	riMsgLen = size;

	return iRet;
}
