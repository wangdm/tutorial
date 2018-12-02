/*
 * socket.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: wangdm
 */

#include "socket.h"
#include <stdio.h>
#include <memory.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

////////////////////////////////////////////////////////////////////////
/// Socket
////////////////////////////////////////////////////////////////////////
Socket::Socket() {
	fd = socket(AF_INET, SOCK_STREAM, 0);
	_ip = "0.0.0.0";
	_port = 0;
}


Socket::Socket(const std::string& ip, short port) {
	fd = socket(AF_INET, SOCK_STREAM, 0);
	_ip = inet_addr(ip.c_str());
	_port = port;
}


Socket::~Socket() {
	if(fd>0)
	{
		close(fd);
		fd = -1;
	}
}


int Socket::Connect()
{
	struct sockaddr_in sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sin_family      = AF_INET;
	sa.sin_addr.s_addr = htonl(_ip);			/* Server IP */
	sa.sin_port        = htons(_port);          /* Server Port number */

	int ret = connect(fd, (struct sockaddr*)&sa, sizeof(sa));
	if(ret == -1)
	{
		perror("Socket connect failed!\n");
	}
	return ret;
}


int Socket::Bind()
{
	struct sockaddr_in sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sin_family      = AF_INET;
	sa.sin_addr.s_addr = htonl(_ip);			/* Server IP */
	sa.sin_port        = htons(_port);          /* Server Port number */
	int ret = bind(fd, (struct sockaddr*)&sa, sizeof (sa));
	if(ret == -1)
	{
		perror("Socket bind failed!\n");
	}
	return ret;
}


int Socket::Listen()
{

}


Socket* Socket::Accept()
{

}


int Socket::Write(const char* buf, int len)
{
	return send(fd, buf, len ,0);
}


int Socket::Read(char* buf, int len)
{
	return recv(fd, buf, len, 0);
}


int Socket::GetFD()
{
	return fd;
}


void Socket::SetIP(std::string& ip)
{
	_ip = ip;
}


void Socket::SetPort(short port)
{
	_port = port;
}


////////////////////////////////////////////////////////////////////////
/// SSLSocket
////////////////////////////////////////////////////////////////////////
SSLSocket::SSLSocket()
{
	ctx = SSLContext::GetContext()->CreateSSLCtx();
	ssl = SSL_new(ctx);
	if(ssl != NULL)
	{
		SSL_set_fd(ssl, GetFD());
	}
}


SSLSocket::SSLSocket(const std::string& ip, short port)
	: Socket(ip, port)
{
	ctx = SSLContext::GetContext()->CreateSSLCtx();
	ssl = SSL_new(ctx);
	if(ssl != NULL)
	{
		SSL_set_fd(ssl, GetFD());
	}
}


SSLSocket::~SSLSocket()
{
	int fd = SSL_get_fd(ssl);
	SSL_shutdown(ssl);
	close(fd);
	fd = -1;
	SSL_free(ssl);
	ssl = NULL;
	SSL_CTX_free(ctx);
	ctx = NULL;
}


int SSLSocket::Connect()
{
	int ret = Socket::Connect();
	ret = SSL_connect(ssl);
	if(ret == -1)
	{
		ERR_print_errors_fp(stderr);
		return ret;
	}

	PrintCertificate();

	return ret;
}


int SSLSocket::Write(const char* buf, int len)
{
	int size = SSL_write(ssl, buf, len);
	int err = SSL_get_error(ssl, size);
	if(SSL_ERROR_WANT_READ == err || SSL_ERROR_WANT_WRITE == err)
	{
		ERR_print_errors_fp(stdout);
	}
	else if(SSL_ERROR_SYSCALL == err || SSL_ERROR_SSL == err)
	{
		ERR_print_errors_fp(stdout);
	}
	else if(SSL_ERROR_ZERO_RETURN == err)
	{
		ERR_print_errors_fp(stdout);
	}
	else if(SSL_ERROR_NONE == err )
	{
		return size;
	}
	else
	{
		ERR_print_errors_fp(stdout);
	}
	return size;
}


int SSLSocket::Read(char* buf, int len)
{
	int size = SSL_read(ssl, buf, len);
	int err = SSL_get_error(ssl, size);
	if(SSL_ERROR_WANT_READ == err || SSL_ERROR_WANT_WRITE == err)
	{
		ERR_print_errors_fp(stdout);
	}
	else if(SSL_ERROR_SYSCALL == err || SSL_ERROR_SSL == err)
	{
		ERR_print_errors_fp(stdout);
	}
	else if(SSL_ERROR_ZERO_RETURN == err)
	{
		ERR_print_errors_fp(stdout);
	}
	else if(SSL_ERROR_NONE == err )
	{
		return size;
	}
	else
	{
		ERR_print_errors_fp(stdout);
	}
	return size;
}


void SSLSocket::PrintCertificate()
{
	X509*    server_cert;
	char*    str;

	printf("SSL connection using %s\n", SSL_get_cipher (ssl));

	server_cert = SSL_get_peer_certificate(ssl);
	//CHK_NULL(server_cert);
	printf("Server certificate:\n");

	str = X509_NAME_oneline (X509_get_subject_name(server_cert),0,0);
	//CHK_NULL(str);
	printf("\t subject: %s\n", str);
	OPENSSL_free(str);

	str = X509_NAME_oneline (X509_get_issuer_name(server_cert),0,0);
	//CHK_NULL(str);
	printf("\t issuer: %s\n", str);
	OPENSSL_free(str);

	X509_free(server_cert);
}


////////////////////////////////////////////////////////////////////////
/// SSLContext
////////////////////////////////////////////////////////////////////////
SSLContext::SSLContext()
{
	SSL_library_init();
	SSL_load_error_strings();
}


SSLContext::~SSLContext()
{
}


SSL_CTX* SSLContext::CreateSSLCtx(SSLAgent agent)
{
	const SSL_METHOD *meth = NULL;
	if(agent == SSL_SERVER)
	{
		meth = SSLv23_server_method();
	}
	else
	{
		meth = SSLv23_client_method();
	}
	SSL_CTX* ctx = SSL_CTX_new(meth);
	return ctx;
}


SSL_CTX* SSLContext::CreateSSLCtx(std::string& pemfile, SSLAgent agent)
{
	const SSL_METHOD *meth = NULL;
	if(agent == SSL_SERVER){
		meth = SSLv23_server_method();
	}else{
		meth = SSLv23_client_method();
	}
	SSL_CTX* ctx = SSL_CTX_new (meth);
	if (!ctx) {
		ERR_print_errors_fp(stderr);
	}

	if (SSL_CTX_use_certificate_file(ctx, pemfile.c_str(), SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
	}
	if (SSL_CTX_use_PrivateKey_file(ctx, pemfile.c_str(), SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
	}
	if (!SSL_CTX_check_private_key(ctx)) {
		fprintf(stderr,"Private key does not match the certificate public key\n");
	}
	return ctx;
}


SSLContext* SSLContext::instance = NULL;
SSLContext* SSLContext::GetContext()
{
	if(instance==NULL)
	{
		instance = new SSLContext();
	}
	return instance;
}
