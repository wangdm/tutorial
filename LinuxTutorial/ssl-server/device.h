/*
 * device.h
 *
 *  Created on: Sep 27, 2017
 *      Author: wangdm
 */

#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <iostream>
#include <string>

#include <pthread.h>

#include "socket.h"

class Device
{
public:
	Device();
	~Device();

	int Connect(const std::string& ip, short port);
	int Disconnect();

	int Start();
	int Stop();

protected:
	static void* MessageThread(void* arg);
	static void* HeartBeatThread(void* arg);

private:
	void MessageLoop();
	void doHeartbeat();

public:
	std::string name;
	std::string mac;
	std::string userToken;
	std::string devToken;
	std::string type;
	std::string rom;

private:
	bool quit;
	bool connected;
	bool login;

	pthread_t message_thread;
	pthread_t heart_thread;

	Socket* socket;
};

#endif /* DEVICE_H_ */
