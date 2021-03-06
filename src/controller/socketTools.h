/*
 * server.h
 *
 *  Created on: Aug 27, 2012
 *      Author: ivan
 */

#ifndef SERVER_H_
#define SERVER_H_

#define TCP_ERROR -1   // error code use in the functions

#endif /* SERVER_H_ */

int tcp_socket(void);
int tcp_connect(int sockfd, char *ip, int port);
int tcp_close(int socket);

int tcp_writeText(int socket, char *text);
int tcp_readText(int socket, char *text, int maxTextSize);

char* getHeaderSize(char *buffer, int *nbytes);
int getFileType(char *buffer);//From the header
int tcp_pocessServerResponse(int socket);

void printWbePage(char *buffer, int size);
int startParagraph(char a, char b, char c);
int endParagraph(char a, char b, char c);






