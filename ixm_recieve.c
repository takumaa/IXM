#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "ixm.h"

	char buf[BUFSIZ];
	int sockfd,new_sockfd;
int main() {
	StartIxm();
	return 0;
}

int StartIxm() {
	int ret = 0;
	struct ixm_service ixm;
	char arg1[20] = "172.30.143.115";/*"172.21.39.106";*/
	char arg2[10] = "12345";
	int j = 0;
	struct sockaddr_in serv,clnt;
	socklen_t sin_siz;
	int port;
	puts("StartIxm");
	if((sockfd = socket(PF_INET,SOCK_STREAM,0)) < 0) {
		perror("socket");
		exit(1);
	}
	printf("socket() called\n");
	serv.sin_family = PF_INET;
	port = strtol(arg2,NULL,10);
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
	while(1) {
		int i = 0;
		if((new_sockfd = accept(sockfd, (struct sockaddr*)&clnt, &sin_siz)) < 0) {
			perror("accept");
		}
		printf("connect from %s : %d\n",inet_ntoa(clnt.sin_addr),ntohs(clnt.sin_port));
		memset(buf, 0, BUFSIZ);
		while(i < 3) {
				recv(new_sockfd, buf, BUFSIZ, 0);
			if(i == 0) {
				IxmSetMode(&ixm,atoi(buf));
			}
			if(i == 1) {
				IxmSetFunc(&ixm,atoi(buf));
				if(ixm.mode == 0 && ixm.func == 0) {
					IxmStop();
					i = 3;
				}
			}
			if(i == 2) {
				if(loop_count > 0)
					IxmStop();
				IxmSetParam(&ixm,atoi(buf));
				if(stop_count > 0)
					return 0;
			}
			i++;
		}

		loop_count = 1;
		close(new_sockfd);
	}
	close(sockfd);
	return 0;
}

int IxmSetMode(struct ixm_service *ixm,int mode) {
	puts("IxmSetMode");
	char ch[11] = "IxmSetMode";
	write(new_sockfd,&ch,11);
	printf("%s\n",ch);
	sleep(10);
	ixm->mode = mode;
	KobukiModeSet(ixm);      //モードを書き換える
	switch (ixm->mode) {
	case IXM_MODE_WATCH:
		puts("mode : watch");
	break;
	case IXM_MODE_KEYOP:
		puts("mode : keyop");
	break;
	default:
		puts("IxmSetMode error");
	break;
	}
	return 0;
}

int IxmSetFunc(struct ixm_service *ixm,int func) {
	puts("IxmSetFunc");
	ixm->func = func;
	switch (ixm->mode) {
		case IXM_MODE_WATCH:
			switch (ixm->func) {
				case IXM_FUNC_FOLLOW:
					puts("func : follow");
				break;
				case IXM_FUNC_FALL:
					puts("func : fall");
				break;
				case IXM_FUNC_TEMPERTURE:
					puts("func : temperture");
				break;
				case IXM_FUNC_FOLLOW_AND_FALL:
					puts("func : follow and fal");
				break;
				case IXM_FUNC_FOLLOW_AND_TEMPERTURE:
					puts("func : follow and temperture");
				break;
				case IXM_FUNC_FALL_AND_TEMPERTURE:
					puts("func : fall and temperture");
				break;
				case IXM_FUNC_FOLLOW_AND_FALL_AND_TEMPERTURE:
					puts("func : follow and fall and temperture");
				break;
				default:
					puts("IxmSetFunc error");
				break;
			}
		break;
		case IXM_MODE_KEYOP:
			switch (ixm->func) {
				case IXM_FUNC_KEYOP:
					puts("func : keyop");
				break;
				default:
					puts("IxmSetFunc error");
				break;
			}
		break;
		default:
			puts("IxmSetFunc error");
		break;
	}
	return 0;
}

int IxmSetParam(struct ixm_service *ixm,float param) {
	puts("IxmSetParam");
	ixm->param = param;
	if(ixm->param > 0) {
 		switch(ixm->func) {
			case 1:
				printf("param : %f\n",ixm->param);
				KobukiDistanceSet(ixm);       //追従距離を書き換える
				IxmStart(ixm);
			break;
			default:
				puts("IxmSetParam error");
			break;
		}
	}
	else {
		IxmStart(ixm);
	}
	return 0;
}

void KobukiModeSet(struct ixm_service *ixm) {
	puts("ModeSet");
	FILE *fp;
	if((fp = fopen("/home/tomochin/.ros/ixm_mode.txt","w")) == NULL) {
		printf("mode_set_file open error!\n");
		exit(EXIT_FAILURE);
	}
	fprintf(fp,"%d",ixm->mode);
	fclose(fp);
	puts("ModeSet ok");
}

void KobukiDistanceSet(struct ixm_service *ixm) {
	puts("DistanceSet");
	FILE *fp;
	if((fp = fopen("/home/tomochin/.ros/ixm_distance.txt","w")) == NULL) {
		printf("distance_set_file open error!\n");
		exit(EXIT_FAILURE);
	}
	fprintf(fp,"%4.1f",ixm->param);
	fclose(fp);
	puts("DistanceSet ok");
}

void IxmStop() {
	puts("IxmStop");
	int i = 0;
	stop_count = 0;
	for(i = 0;i<7;i++) {
		if(child_process_id[i] != 0) {
			if(i == 0) {
				child_process_id[i] = child_process_id[i] + 2;
			}
			else if(i == 1) {
				child_process_id[i] = child_process_id[i] + 6;
			}
			else if(i == 2) {
				child_process_id[i] = child_process_id[i] + 4;
			}
			else {
				child_process_id[i] = child_process_id[i] + 3;
			}	
			kill(child_process_id[i],SIGINT);
			printf("kill process id : %d\n",child_process_id[i]);
			wait(NULL);
		}
	}
	for(i = 0;i<7;i++) 
		child_process_id[i] = 0;
	puts("IxmStop ok");
}
	
int IxmStart(struct ixm_service *ixm) {
	puts("IxmStart");
	switch(ixm->mode) {
		case IXM_MODE_WATCH:
			switch (ixm->func) {
				case IXM_FUNC_FOLLOW:
					KobukiStart();
					if(stop_count > 0) {
						return 0;
					}
					follow();
					if(stop_count > 0) {
						return 0;
					}
				break;
				case IXM_FUNC_FALL:
					fall();
					if(stop_count > 0) {
						return 0;
					}
				break;
				case IXM_FUNC_TEMPERTURE:
					temperture();
					if(stop_count > 0) {
						return 0;
					}
				break;
				case IXM_FUNC_FOLLOW_AND_FALL:
					KobukiStart();
					if(stop_count > 0) {
						return 0;
					}
					follow();
					if(stop_count > 0) {
						return 0;
					}
					fall();
					if(stop_count > 0) {
						return 0;
					}
				break;
				case IXM_FUNC_FOLLOW_AND_TEMPERTURE:
					KobukiStart();
					if(stop_count > 0) {
						return 0;
					}
					follow();
					if(stop_count > 0) {
						return 0;
					}
					temperture();
					if(stop_count > 0) {
						return 0;
					}
				break;
				case IXM_FUNC_FALL_AND_TEMPERTURE:
					KobukiStart();
					if(stop_count > 0) {
						return 0;
					}
					fall();
					if(stop_count > 0) {
						return 0;
					}
					temperture();
					if(stop_count > 0) {
						return 0;
					}
				break;
				case IXM_FUNC_FOLLOW_AND_FALL_AND_TEMPERTURE:
					KobukiStart();
					if(stop_count > 0) {
						return 0;
					}
					follow();
					if(stop_count > 0) {
						return 0;
					}
					fall();
					if(stop_count > 0) {
						return 0;
					}
					temperture();
					if(stop_count > 0) {
						return 0;
					}
				break;
				default:
					puts("IxmStart error");
				break;
			}
		break;
		case IXM_MODE_KEYOP:
			switch(ixm->func) {
				case IXM_FUNC_KEYOP:
					KobukiStart();
					if(stop_count > 0) {
						return 0;
					}
					keyop();
					if(stop_count > 0) {
						return 0;
					}
				break;
			}	
		break;
		default:
			puts("IxmStart error");
		break;
	}
}

void KobukiStart() {
	puts("KobukiStart");
	int pid1 = fork();
	if(pid1 == 0) {
		system("roscore");
		stop_count++;
	}
	else {
		sleep(1);
		child_process_id[0] = pid1;
		printf("roscore,%d\n",child_process_id[0] + 2);
		int pid2 = fork();
		if(pid2 == 0) {
			system("minimal");
			stop_count++;
		}
		else {
			child_process_id[1] = pid2;
			printf("roslaunch_minimal,%d\n",child_process_id[1] + 6);
		}
	}
}

void follow() {
	puts("follow");
	int pid3 = fork();
	if(pid3 == 0) {
		system("kinect_follow");
		stop_count++;
	}
	else {
		sleep(30);
		child_process_id[2] = pid3;
		printf("kinect_follow,%d\n",child_process_id[2] + 4);
		int pid4 = fork();
		if(pid4 == 0) {
			system("keyop");
			stop_count++;
		}
		else {
			child_process_id[3] = pid4;
			printf("roslaunch_keyop,%d\n",child_process_id[3] + 3);
		}
	}
}

void keyop() {
	puts("keyop");
	int pid7 = fork();
	if(pid7 == 0) {
		system("keyop");
		stop_count++;
	}
	else {
		child_process_id[6] = pid7;
		printf("roslaunch_keyop,%d\n",child_process_id[6] + 3);
	}
}

void fall() {
	puts("fall");
	int pid5 = fork();
	if(pid5 == 0) {
		system("kinect_fall");
		stop_count++;
	}
	else {
		child_process_id[4] = pid5;
		printf("kinect_fall,%d\n",child_process_id[4] + 3);
	}
}

void temperture() {
	puts("temperture");
	int pid6 =fork();
	if(pid6 == 0) {
		system("./temperture_send");
		stop_count++;
	}
	else {
		child_process_id[5] = pid6;
		printf("temperture,%d\n",child_process_id[5]);
		sleep(1);
	}
}

int IxmGetMode(struct ixm_service *ixm) {
	return ixm->mode;
}

int IxmGetFunc(struct ixm_service *ixm) {
	return ixm->func;
}

int IxmGetParam(struct ixm_service *ixm) {
	return ixm->param;
}
