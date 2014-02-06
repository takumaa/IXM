#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
	char arg1[20] = "172.21.39.101";
	char arg2[10] = "1234567";
	int sockfd, len, new_sockfd;
	char buf[BUFSIZ];
	struct sockaddr_in serv, clnt;
	socklen_t sin_siz;
	int port;
	int i = 0;
	FILE *fp;
	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	printf("socket() called\n");
	serv.sin_family = PF_INET;
	port = strtol(arg2, NULL, 10);
	serv.sin_port = htons(port);
	inet_aton(arg1,&serv.sin_addr);
	sin_siz = sizeof(struct sockaddr_in);
	if(bind(sockfd, (struct sockaddr*)&serv, sizeof(serv)) < 0) {
		perror("bind");
		exit(1);
	}
	printf("bind() called\n");
	if(listen(sockfd,SOMAXCONN) < 0) {
		perror("listen");
		exit(1);
	}
	printf("listen() called\n");
	if((new_sockfd = accept(sockfd, (struct sockaddr*)&clnt, &sin_siz)) < 0) {
		perror("accept");
	}
	printf("connect from %s: %d\n",inet_ntoa(clnt.sin_addr),ntohs(clnt.sin_port));
	memset(buf, 0, BUFSIZ);
	while(1) {
		recv(new_sockfd, buf, BUFSIZ, 0);
		if((fp=fopen("/home/takuma1/processing/sketch/test/temperture.txt","w")) == NULL) {
			printf("file open error!\n");
				exit(EXIT_FAILURE);
		}
		printf("%s\n",buf);
		fprintf(fp,"%s",buf);
		fclose(fp);
		i++;
	}
	close(new_sockfd);
	close(sockfd);
	return 0;
}
