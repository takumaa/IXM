#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>


int main() {
	char *arg[2];
	char arg1[20] = "172.21.39.101";
	char arg2[10] = "1234567";
	int sockfd,len;
	char buf[BUFSIZ];
	struct sockaddr_in serv;
	int port;
	int i = 0;
	FILE *fp;
	if((sockfd = socket(PF_INET,SOCK_STREAM,0)) < 0) {
		perror("socket");
		exit(1);
	}
	serv.sin_family = PF_INET;
	port = strtol(arg2,NULL,10);
	serv.sin_port = htons(port);
	inet_aton(arg1,&serv.sin_addr);
	if(connect(sockfd,(struct sockaddr*)&serv,sizeof(serv)) < 0) {
		perror("connect");
		exit(1);
	}

	while(1) {
		if((fp=fopen("/dev/ttyACM0","r")) == NULL) {
			printf("file open error!\n");
			exit(EXIT_FAILURE);
		}
		if(fgets(buf,BUFSIZ,fp) != NULL) {
			len = send(sockfd,buf,strlen(buf),0);
				sleep(1);
		}
		fclose(fp);
		i++;
	}	
		
	close(sockfd);
	return 0;
}
