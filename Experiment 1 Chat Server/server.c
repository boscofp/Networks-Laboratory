#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

struct user
{
        int sd;
        int id;
};

struct regusers
{
	int id;
	char password[20];
};

struct regusers list[5];

struct user users[20];
int sockfd,newsockfd,portno,errorCode,i=0,j;
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;

pthread_t threads[20];
int threadErrorCode;
long t;

void error(const char *msg)
{
        perror(msg);
        exit(0);
}

void *chat(void * x)
{
	int sndr,snid;	
	struct user * client;
	client=(struct user *) x;	
	char buffer[256],temp[256];
        
        while(1)
        {	
        	bzero(buffer,256);
        	read(client->sd,buffer,256);
        	write(1,buffer,256);
        	sscanf(buffer,"%*s %d %[^\n]",&snid,temp);
        	for(j=0;j<=i;j++)
        		if(users[j].id==snid)
        		{
        			sndr=users[j].sd;
        			break;
        		}
        	if(j>i)
        		{
        		sprintf(buffer,"no such user");
        		write(client->sd,buffer,256);
        		}
        	else
        	{
        		sprintf(buffer,"%s %d %s","From user",client->id,temp);
        		write(1,temp,30);
        		write(sndr,buffer,256); 
        		
        	}
        }
}

int main(int argc, char *argv[])
{	
	
	//Names of all registered users. Could put it in a file
	list[0].id=0;
	sprintf(list[0].password,"jake\n");
	list[1].id=1;
	sprintf(list[1].password,"jake\n");
	list[2].id=2;
	sprintf(list[2].password,"jake\n");
	list[3].id=3;
	sprintf(list[3].password,"jake\n");
	list[4].id=4;
	sprintf(list[4].password,"jake\n");
	
	
	int id;
	char buffer[256];
        if (argc < 2) {
                fprintf(stderr,"ERROR, no port provided\n");
                exit(1);
        }

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
                error("ERROR opening socket");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[1]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
        if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
                error("ERROR on binding");
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        if (newsockfd < 0)
        	error("ERROR on accept");
        while(1) 
        {
                clilen = sizeof(cli_addr);
                newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
                if (newsockfd < 0)
                        error("ERROR on accept");
                
                
                bzero(buffer,256);
          	sprintf(buffer,"Username :");
          	write(newsockfd,buffer,256);
          	bzero(buffer,256);
          	read(newsockfd,buffer,256);
          	sscanf(buffer,"%*s %d",&id);
          	
          	          	
          	bzero(buffer,256);
          	sprintf(buffer,"Password :");
          	write(newsockfd,buffer,256);
          	bzero(buffer,256);
          	read(newsockfd,buffer,256);
          	
          	write(1,buffer,256);
          	
          	for(j=0;j<5;j++)
          	{	
          		if(list[j].id==id && strcmp(list[j].password,buffer)==0) 
          			break;
          	}
          	if(j==5)
          	{
          		bzero(buffer,256);
          		sprintf(buffer,"Invalid");
          		write(newsockfd,buffer,256);
          	}
          	else
          	{	
          		users[i].id=id;			
                	users[i].sd=newsockfd;
                	printf("\n User %d logged in \n",id);
            
                	for(j=0;j<i;j++)
                	{	
                		bzero(buffer,256);
          			sprintf(buffer,"user%d online",users[j].id);
        			write(newsockfd,buffer,256);
        			bzero(buffer,256);
          			sprintf(buffer,"user%d online",id);
        			write(users[j].sd,buffer,256);
        		}
                	threadErrorCode=pthread_create(&threads[t],NULL,chat,(void *) &users[i]);
                	i++;
                }
        }
        return 1;
}

