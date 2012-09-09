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

int tcp_socket(int sockfd) {
	//    if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info
	//        perror("gethostbyname");
	//        exit(1);
	//    }
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	return sockfd;
}

void tcp_connect(struct sockaddr_in* their_addr, int sockfd, struct hostent* he) {
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(PORT);
	their_addr.sin_addr = *((struct in_addr *) he->h_addr);
	memset(&(their_addr->sin_zero), '\0', 8); // zero the rest of the struct

	if (connect(sockfd, (struct sockaddr *) &their_addr, sizeof(struct sockaddr)) == TCP_ERROR) {
		perror("connect");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct hostent *he;
    struct sockaddr_in their_addr; // connector's address information

    char *request = getRequest(argv[1]);
    puts(request);

//    if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info
//        perror("gethostbyname");
//        exit(1);
//    }


    sockfd = tcp_socket(sockfd);

    he = gethostbyname("localhost");
	tcp_connect(&their_addr, sockfd, he);

    tcp_writeText(sockfd, request);

    char *text;
    tcp_readText(sockfd, text, 500);
    puts(text);

    free(request);

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









