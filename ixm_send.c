#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<termios.h>



struct ixm_service {
	int mode;
	int func;
	double param;
};

typedef struct ixm_service *ixm_t;


int ixm_send(int sockfd,struct ixm_service *ixm);

int main(int argc,char **argv) {
	
	struct ixm_service ixm;
	char arg1[32] = "172.30.143.115";/*"172.21.39.106";*/
	char arg2[10] = "12345";
	int sockfd;
	struct sockaddr_in serv;
	int port;
	ixm.mode = atoi(argv[1]);
	ixm.func = atoi(argv[2]);
	if(ixm.mode == 1 && (ixm.func == 1 || ixm.func == 4 || ixm.func == 5 || ixm.func == 7)) {
		printf("distance param : ");
		scanf("%lf",&ixm.param);
	}
	else {
		ixm.param = 0.0;
	}
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
	ixm_send(sockfd,&ixm);
	if(ixm.mode == 1 && (ixm.func == 3 || ixm.func == 5 || ixm.func == 6 || ixm.func == 7)){
		system("/home/takuma1/processing/sketch/test/output/test");
	}
	close(sockfd);
	return 0;
}

int ixm_send(int sockfd,struct ixm_service *ixm) {
	int i = 0;
	char buf[BUFSIZ];
	char ch[30];
	FILE *fp;
	while(i < 3) {
		if((fp=fopen("./ixm.txt","w")) == NULL) {
			printf("file open error!\n");
			exit(EXIT_FAILURE);
		}
		if(i == 0) {
			fprintf(fp,"%d",ixm->mode);
			printf("mode:%d\n",ixm->mode);
		}
		if(i == 1) {
			fprintf(fp,"%d",ixm->func);
			printf("func:%d\n",ixm->func);
			if(ixm->mode == 0 && ixm->func == 0)
				i = 3;
		}
		if(i == 2) {
				fprintf(fp,"%1.1f",ixm->param);
				if(ixm->param != 0.0)
					printf("param:%1.1f\n",ixm->param);
		}
		fclose(fp);
		if((fp=fopen("./ixm.txt","r")) == NULL) {
			printf("file open error!\n");
			exit(EXIT_FAILURE);
		}
		if(fgets(buf,BUFSIZ,fp) != NULL) {
			send(sockfd,buf,strlen(buf),0);
			read(sockfd,&ch,11);
			printf("%s\n",ch);
		}
		fclose(fp);
		i++;
	}
	if(ixm->mode == 2 && ixm->func == 1) {
		ixm_user_send();
	}
}

int ixm_user_send() {
	struct termios term, default_term;
	tcgetattr(fileno(stdin), &term);
	default_term = term;
	term.c_lflag &= ~ICANON;
	tcsetattr(fileno(stdin), TCSANOW, &term);
	char arg1[32] = "172.30.143.115";/*"172.21.39.106";*/
	char arg2[10] = "123456";
	char buf[BUFSIZ];
	int sockfd;
	struct sockaddr_in serv;
	int port;
	char c;
	FILE *fp;
	sleep(25);
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
	puts("keyop");
	while((c = getchar()) != EOF) {
		printf("\n");
		if((fp=fopen("./ixm_user.txt","w")) == NULL) {
			printf("file open error!\n");
			exit(EXIT_FAILURE);
		}
		fprintf(fp,"%c",c);
		fclose(fp);
		if((fp=fopen("./ixm_user.txt","r")) == NULL) {
			printf("file open error!\n");
			exit(EXIT_FAILURE);
		}
		if(fgets(buf,BUFSIZ,fp) != NULL) {
			fputs(buf,stdout);
			printf("\n");
			send(sockfd,buf,strlen(buf),0);
		}
		fclose(fp);
		if(c == 'q') {
			close(sockfd);
			return 0;
		}
	}
}
