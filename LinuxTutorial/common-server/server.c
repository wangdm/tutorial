/*
 * server.c
 *
 *  Created on: Sep 22, 2017
 *      Author: wangdm
 */


#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct clientinfo_t{
	int fd;
	struct sockaddr_in addr;
	pthread_t thread;
};

void * client_handler(void* arg)
{
	struct clientinfo_t* clientinfo = (struct clientinfo_t*)arg;
	char recvbuf[128];
	while(1)
	{
		int recvsize = recv(clientinfo->fd, recvbuf, 128, 0);
		if(recvsize<0)
		{
			break;
		}else if(recvsize==0){
			continue;
		}else{
			send(clientinfo->fd, recvbuf, recvsize, 0);
		}
	}

	free(clientinfo);

	return NULL;
}

int main()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serveraddr = {0};
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(1989);
	int ret = bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

	ret = listen(listenfd, 0);

	do{
		struct clientinfo_t* clientinfo = (struct clientinfo_t*)malloc(sizeof(struct clientinfo_t));
	    socklen_t address_len;
	    clientinfo->fd = accept(listenfd, (struct sockaddr *)&(clientinfo->addr), &address_len );
	    if(clientinfo->fd > 0)
	    {
	    	printf("client %s connected...\n", inet_ntoa (clientinfo->addr.sin_addr));
	    	pthread_create(&clientinfo->thread, NULL, client_handler, clientinfo);
	    }else{
	    	free(clientinfo);
	    	printf("server accept failed!\n");
	    	break;
	    }

	}while(1);

	close(listenfd);

	return 0;
}
