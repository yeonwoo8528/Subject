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

#define SERVERPORT 21
#define MAXBUF 1024

int main(int argc, char* argv[])
{
	int sockd, sockd2;
	int send, recv;
	int port;
	int port1, port2;
	struct sockaddr_in ftpServer, ftp2Server;
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
	ftpServer.sin_family = AF_INET;
	ftpServer.sin_addr.s_addr = inet_addr(argv[1]); //ip address
	ftpServer.sin_port = htons(SERVERPORT); //hostbyte to networkbyte
	
	/* connect to the server */
	returnStatus = connect(sockd, (struct sockaddr*)&ftpServer, sizeof(ftpServer));
	
	if (returnStatus == -1) //서버와 연결되지 않으면 종료
	{
		fprintf(stderr, "Could not connect to server!\n");
		exit(1);
	}

	/* open up a handle to our string to receive the contents */
	/* from the server */

	//USER PASS 전송
	strcpy(sendmessage, "USER elec\r\n");
	send = write(sockd, sendmessage, strlen(sendmessage));
	recv = read(sockd, recvmessage, MAXBUF);

	strcpy(sendmessage, "PASS comm\r\n");
	send = write(sockd, sendmessage, strlen(sendmessage));
	recv = read(sockd, recvmessage, MAXBUF);
	printf("%s\n", recvmessage);
	
	//PASV port 번호 발급
	strcpy(sendmessage, "PASV\r\n");
	send = write(sockd, sendmessage, strlen(sendmessage));
	recv = read(sockd, recvmessage, MAXBUF);
	printf("%s\n", recvmessage);

	//port 번호 추출 후 새로 만들기
	sscanf(recvmessage, "227 Entering Passive Mode (%*d,%*d,%*d,%*d,%d,%d)\n", &port1, &port2);
	port = port1*256 + port2;

	//RETR txt파일 읽어서 전송
	strcpy(sendmessage, "RETR welcome.txt\r\n");
	send = write(sockd, sendmessage, strlen(sendmessage));

	//new socket 생성
	sockd2 = socket(AF_INET, SOCK_STREAM, 0);

	//set up the new server information
	ftp2Server.sin_family = AF_INET;
	ftp2Server.sin_addr.s_addr = inet_addr(argv[1]);
	ftp2Server.sin_port = htons(port);

	//connet to the new server
	returnStatus = connect(sockd2, (struct sockaddr*)&ftp2Server, sizeof(ftp2Server));
	
 	if (returnStatus == -1) //새로운 서버와 연걸되지 않으면 종료
	{
		fprintf(stderr, "Could not connect to new server!\n");
		exit(1);
	}

	//txt파일 읽기
	while((recv = read(sockd2, recvmessage, MAXBUF)) > 0)
	{
		/* send the contents to stdout */
		write(1, recvmessage, recv);
	}

	//기존 socket read
	recv = read(sockd, recvmessage, MAXBUF);
	printf("%s\n", recvmessage);
	close(sockd);
	close(sockd2);
	return 0;
}
