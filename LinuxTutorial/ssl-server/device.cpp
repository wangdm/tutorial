/*
 * device.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: wangdm
 */

#include <string.h>

#include "device.h"
#include "alexa.h"


Device::Device()
{
	name = "";
	mac = "";
	userToken = "";
	devToken = "";
	type = "";
	rom = "";

	connected = false;
	login = false;
	quit = false;

	message_thread = 0;
	heart_thread = 0;

	socket = NULL;
}


Device::~Device()
{
	if(message_thread>0 || heart_thread >0)
	{
		Stop();
	}

	if(connected)
	{
		Disconnect();
	}

	if(socket!=NULL)
	{
		delete socket;
	}
}


////////////////////////////////////////////////////////////////////////
/// Public Method
////////////////////////////////////////////////////////////////////////
int Device::Connect(const std::string& ip, short port)
{
	if(!connected)
	{
		socket = new SSLSocket(ip, port);
		socket->Connect();
		connected = true;

		int size = sizeof(stLoginMsg);
		void* buffer = new char[size];
		memset(buffer, 0, size);

		stLoginMsg* msg = (stLoginMsg*)buffer;
		msg->m_Base.m_cVersion = PROTOCOL_VERSION;
		msg->m_Base.m_cMSGType = MSG_TYPE_DEV_LOGIN;
		msg->m_Base.m_uLength = size;
		strncpy(msg->m_cMac, mac.c_str(), MAX_MAC_LEN);
		strncpy(msg->m_cUserToken, userToken.c_str(), MAX_USERTOKEN_LEN);
		strncpy(msg->m_cDevToken, devToken.c_str(), MAX_DEVTOKEN_LEN);
		strncpy(msg->m_cDevType, type.c_str(), MAX_DEVTYPE_LEN);
		strncpy(msg->m_cRomVersion, rom.c_str(), MAX_ROMVERSION_LEN);
		strncpy(msg->m_cSign, mac.c_str(), MAX_SIGN_LEN);
		size = socket->Write((const char*)msg, size);

		memset(buffer, 0, size);
		size = socket->Read((char*)buffer, size);
		if(size > 0)
		{
			stLoginResponseMsg* rep = (stLoginResponseMsg*)buffer;
			if(rep->m_Base.m_cMSGType == MSG_TYPE_LOGIN_RESPONSE){
				std::cout<<"Login success."<<std::endl;
			}else{
				std::cout<<"Login failed."<<std::endl;
			}
		}
	}
	return 0;
}


int Device::Disconnect()
{
	if(connected)
	{
		delete socket;
		socket = NULL;
		connected = false;
	}

	return 0;
}


int Device::Start()
{
	if(!connected)
	{
		return -1;
	}

	quit = false;
	int ret = 0;
	ret = pthread_create(&message_thread, NULL, MessageThread, this);
	if(ret == -1){
		std::cout<<"create message thread failed!"<<std::endl;
	}

	ret = pthread_create(&heart_thread, NULL, HeartBeatThread, this);
	if(ret == -1){
		std::cout<<"create heartbeat thread failed!"<<std::endl;
	}

	return 0;
}


int Device::Stop()
{
	quit = true;

	if(heart_thread>0)
	{
		pthread_join(heart_thread, NULL);
		heart_thread = 0;
	}

	if(message_thread>0)
	{
		pthread_join(message_thread, NULL);
		message_thread = 0;
	}

	if(connected)
	{
		Disconnect();
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////
/// Protected Method
////////////////////////////////////////////////////////////////////////
void* Device::MessageThread(void* arg)
{
	Device* dev = (Device*)arg;

	char* recvbuf = new char[MAX_MESSAGE_LEN];
	int recvsize = 0;

	while(!dev->quit)
	{
		recvsize = dev->socket->Read(recvbuf, MAX_MESSAGE_LEN);
		if(recvsize>0){

		}
	}

	delete[] recvbuf;

	return 0;
}


void* Device::HeartBeatThread(void* arg)
{
	Device* dev = (Device*)arg;

	while(!dev->quit)
	{
		dev->doHeartbeat();
		sleep(2);
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////
/// Private Method
////////////////////////////////////////////////////////////////////////
void Device::MessageLoop()
{
	return;
}


void Device::doHeartbeat()
{
	return;
}


