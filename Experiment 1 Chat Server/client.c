#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

int sockfd,portno,n;
struct sockaddr_in serv_addr;
struct hostent *server;

pthread_t threads[20];
int threadErrorCode;
long t;

void error(const char *msg)
{
        perror(msg);
        exit(0);
}

void * chatwrite(void * empty)
{
	char buffer[256];
	while(1)
	{
		bzero(buffer,256);
		fgets(buffer,256,stdin);
		n=write(sockfd,buffer,strlen(buffer));
	}
}

void chatread()
{
	char buffer[256];
	while(1)
	{
		bzero(buffer,256);
		n=read(sockfd,buffer,256);
		write(1,buffer,sizeof(buffer));
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	 if (argc < 3) {
                fprintf(stderr,"usage %s hostname port\n", argv[0]);
                exit(0);
        }
        portno = atoi(argv[2]);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
                error("ERROR opening socket");
        server = gethostbyname(argv[1]);
        if (server == NULL) {
                fprintf(stderr,"ERROR, no such host\n");
                exit(0);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
                error("ERROR connecting");
        threadErrorCode=pthread_create(&threads[t],NULL,chatwrite,NULL);
        chatread();
        
        return 1;
}
        
