/*ee
 * webserver.c
 *
 *  Created on: Aug 27, 2012
 *      Author: ivan
 */
/*
** client.c -- a stream socket client demo
*/

#include "../src/controller/socketTools.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 8080 // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

char *getRequest(char *page);

int main(int argc, char *argv[])
{
    int sockfd;

	char* ip = argv[1];
	char* page = argv[2];

	char *request = getRequest(page);

    sockfd = tcp_socket();
	tcp_connect(sockfd, ip, PORT);
    tcp_writeText(sockfd, request);

    tcp_printWebPage(sockfd);

    close(sockfd);

    return 0;
}

char *getRequest(char *page){
	char *header1 = "GET /";

	char *header2 = " HTTP/1.1\n"
				   "Host: localhost:8080\n"
				   "Connection: keep-alive\n"
				   "Accept: */*\n"
				   "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.89 Safari/537.1\n"
				   "Accept-Encoding: gzip,deflate,sdch\n"
				   "Accept-Language: es,en-US;q=0.8,en;q=0.6\n"
				   "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3\n";

	char *all = malloc(strlen(header1) + strlen(header2) + strlen(page) + 1);

	strcat(all, header1);
	strcat(all, page);
	strcat(all, header2);

	return all;
}









