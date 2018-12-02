/*
 * server.c
 *
 *  Created on: Sep 22, 2017
 *      Author: wangdm
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main()
{
	int serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if(serverfd < 0)
	{
		printf("client socket failed\n");
		return -1;
	}

	struct sockaddr_in serveraddr = {0};
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(1989);

    int ret = connect(serverfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr_in));
    if(ret < 0)
    {
    	printf("connect server failed!\n");
    	return -1;
    }

    while(1)
    {
    	char * msg = "hello server";
    	send(serverfd, msg, strlen(msg), 0);
    }

	return 0;
}
