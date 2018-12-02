/*
 * socket.h
 *
 *  Created on: Sep 27, 2017
 *      Author: wangdm
 */

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <string>

#include <openssl/ssl.h>

#include <netinet/in.h>

class Socket
{
public:
	Socket();
	Socket(const std::string& ip, short port);
	virtual ~Socket();

	virtual int Connect();

	virtual int Bind();
	virtual int Listen();
	virtual Socket* Accept();

	virtual int Write(const char* buf, int len);
	virtual int Read(char* buf, int len);

	virtual int GetFD();

	virtual void SetIP(std::string& ip);
	virtual void SetPort(short port);

private:
	int fd;

	in_addr_t _ip;
	in_port_t _port;

};


class SSLSocket : public Socket
{
public:
	SSLSocket();
	SSLSocket(const std::string& ip, short port);
	virtual ~SSLSocket();

	virtual int Connect();

	virtual int Bind();
	virtual int Listen();
	virtual Socket* Accept();

	virtual int Write(const char* buf, int len);
	virtual int Read(char* buf, int len);

	virtual void PrintCertificate();

private:
	SSL_CTX* ctx;
	SSL* ssl;
};


class SSLContext
{
	enum SSLAgent{
		SSL_CLIENT = 0,
		SSL_SERVER,
	};

private:
	SSLContext();

public:
	~SSLContext();

	SSL_CTX* CreateSSLCtx(SSLAgent agent = SSL_CLIENT);
	SSL_CTX* CreateSSLCtx(std::string& pemfile, SSLAgent agent = SSL_SERVER);

	static SSLContext* GetContext();

private:
	static SSLContext* instance;
};


#endif /* SOCKET_H_ */
