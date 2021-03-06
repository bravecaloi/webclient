/*
 * server.c

 *
 *  Created on: Aug 27, 2012
 *      Author: ivan
 */

#include "socketTools.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdbool.h>

enum {
	text, image
} filetype_t;

/**
 * The return value from socket is the file descriptor for the new socket, or -1 in case of error.
 */
int tcp_socket(void) {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == TCP_ERROR) {
		perror("socket");
	}

	return sockfd;
}

int tcp_connect(int sockfd, char *ip, int port) {
	struct sockaddr_in their_addr;

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(port);
	their_addr.sin_addr = *((struct in_addr *) gethostbyname(ip)->h_addr);
	memset(&(their_addr.sin_zero), '\0', 8); // zero the rest of the struct

	int connection = connect(sockfd, (struct sockaddr*) &their_addr, sizeof(struct sockaddr));
	if (connection == TCP_ERROR) {
		perror("connect");
	}

	return EXIT_SUCCESS;
}

/**
 * Sends text to the socketClient
 */
int tcp_writeText(int socket, char *text) {
	if (send(socket, text, strlen(text), 0) == TCP_ERROR) {
		printf("[WRITE ERROR] - %s", text);
		perror("send");
	}

	return EXIT_SUCCESS;
}

int header = true;

FILE* saveImage(int nbytes, FILE* fp, char* buffer) {
	if (fp == NULL ) {
		fp = fopen("/home/ivan/Desktop/image.jpg", "w+");
	}
	fwrite(buffer, sizeof(char), nbytes, fp);
	return fp;
}

/**
 * Prints the IS parsing as browser to the std output
 */
int tcp_pocessServerResponse(int socket) {
	int max = 2000;
	char *buffer = malloc(max);

	FILE *fp = NULL;

	int filetype;

	int nbytes = max;
	while ((nbytes = read(socket, buffer, max)) != 0) {

		if (nbytes == TCP_ERROR && errno == EINTR) {
			tcp_pocessServerResponse(socket);
		}

		char *headerSize = getHeaderSize(buffer, &nbytes);
		if ( headerSize != 0) {
			filetype = getFileType(buffer);
			buffer = headerSize;
		}

		switch (filetype) {
		case text:
			printWbePage(buffer, nbytes);
			break;
		case image:
			fp = saveImage(nbytes, fp, buffer);
			break;

		default:
			break;
		}

		buffer = malloc(max);
	}

	free(buffer);

	if(fp != NULL){
		fclose(fp);
	}

	return EXIT_SUCCESS;
}

/**
 * Gets the file type from the header, and trims the header
 */
int getFileType(char *buffer) {

	if (strstr(buffer, "Content-Type: image/")) {
		return image;
	}

	return text;
}

char *firstLine = "HTTP/1.0 302 Found\n";

char* getHeaderSize(char *buffer, int *nbytes) {
	int i;
	for (i = 0; i < strlen(firstLine); ++i) {
		if (buffer[i] != firstLine[i]) {
			return 0;
		}
	}

	char *p = strstr(buffer, "\n\n") + 2;
	*nbytes = *nbytes - (p - buffer);

	return p;
}

int print = false;
void printWbePage(char *buffer, int size) {
	int i;
	for (i = 0; i < size - 3; ++i) {

		if (startParagraph(buffer[i], buffer[i + 1], buffer[i + 2])) {
			print = true;
			i = i + 3;
			printf("\n");
		}

		if (endParagraph(buffer[i], buffer[i + 1], buffer[i + 2])) {
			print = false;
			printf("\n");
		}

		if (print) {
			printf("%c", buffer[i]);
		}
	}
}

int startParagraph(char a, char b, char c) {
	if (a == '<' && b == 'p' && c == '>') {
		return true;
	}

	return false;
}

int endParagraph(char a, char b, char c) {
	if (a == '<' && b == '/' && c == 'p') {
		return true;
	}

	return false;
}

/**
 * Reads text into the char *text parameter
 */
int tcp_readText(int socket, char *text, int maxTextSize) {
	int numbytes;

	numbytes = recv(socket, text, maxTextSize - 1, 0);

	if (numbytes == TCP_ERROR) {

		if (errno == EINTR) {
			tcp_readText(socket, text, maxTextSize);
			return EXIT_SUCCESS;
		}

		perror("recv");
		return EXIT_FAILURE;
	}
	text[numbytes] = '\0';

	return EXIT_SUCCESS;
}

/**
 * Closes the socket
 */
int tcp_close(int socket) {
	close(socket);

	return EXIT_SUCCESS;
}
