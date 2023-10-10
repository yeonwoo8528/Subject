#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVERPORT 80
#define MAXBUF 1024

int main(int argc, char* argv[])
{
	int sockd;
	int send, recv;
	int port;
	struct sockaddr_in webServer;
	char sendmessage[MAXBUF]; //문자열 송신
	char recvmessage[MAXBUF]; //문자열 수신
	int returnStatus;
	
	if (argc != 3) //ip주소, port넘버 입력
	{
		fprintf(stderr, "Usage: %s <ip address> <port number>\n", argv[0]);
		exit(1);
	}

	/* create a socket */
	sockd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockd == -1) //소켓이 생성되지 않으면 종료
	{
		fprintf(stderr, "Could not create socket!\n");
		exit(1);
	}

	/* set up the server information */
	webServer.sin_family = AF_INET;
	webServer.sin_addr.s_addr = inet_addr(argv[1]); //ip address
	webServer.sin_port = htons(SERVERPORT); //hostbyte to networkbyte
	
	/* connect to the server */
	returnStatus = connect(sockd, (struct sockaddr*)&webServer, sizeof(webServer));
	
	if (returnStatus == -1) //서버와 연결되지 않으면 종료
	{
		fprintf(stderr, "Could not connect to server!\n");
		exit(1);
	}

	/* send the port number we want to the server */
	port = atoi(argv[2]);

	/* open up a handle to our string to receive the contents */
	/* from the server */
	strcpy(sendmessage, "GET / HTTP/1.1\r\nHOST: 223.194.7.95\r\n\r\n");
	send = write(sockd, sendmessage, strlen(sendmessage));

	if (send == -1)
	{
		fprintf(stderr, "Could not send message.\n");
		send = 1;
	}

	/* read the message from the socket as long as there is data */
	recv = read(sockd, recvmessage, MAXBUF);
	
	if (recv == -1)
	{
		fprintf(stderr, "Could not receive message to Socket.\n");
		recv = 1;
	}

	printf("%s\n", recvmessage); //message 출력
	close(sockd);
	return 0;
}
