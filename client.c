#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>


int main(int argc, char *argv[]){
  char message[255],ch;
  int client_score=0,total_client=0,dice=6,total_server=0,server_score=0,n;
  int server, portNumber;
  struct sockaddr_in servAdd;     // server socket address
  
 if(argc != 3){
    printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
     fprintf(stderr, "Cannot create socket\n");
     exit(1);
  }

  servAdd.sin_family = AF_INET;
  sscanf(argv[2], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);

  if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
  fprintf(stderr, " inet_pton() has failed\n");
  exit(2);
}

  
 if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
  fprintf(stderr, "connect() has failed, exiting\n");
  exit(3);
 }
 read(server, message, 255);
 fprintf(stderr, "%s", message);
 

 while(1){
	read(server, message, 255);
	fprintf(stderr, "%s", message);

if(n=read(server, message, 255))
	   {
  server_score=atoi(message);	
  fprintf(stderr, " \n\n Server Score: %d\n", server_score);
  total_server+=server_score;
  
  fprintf(stderr, " Server Total Score: %d\n",total_server);
  if(total_server>=100)
			{
				read(server, message, 255);
				fprintf(stderr, "%s", message);
				close(server);
				exit(0);
			}
  fprintf(stderr, "\n Press Enter Key to roll your Dice...\n");
  
	   }
	  again: 
	  //fprintf(stderr,"Input is invalid, Press Enter-key");
	  ch =fgetc(stdin);
	  if(ch == 0x0A)
	  {
  
        client_score = rand()%dice+1;
		
		total_client+=client_score;
		fprintf(stderr, " Client Score: %d\n",client_score);
		fprintf(stderr," Client Total Score: %d\n\n\n",total_client);
		sprintf(message,"%d",client_score);
		
  
  if(message[0] == '$'){
    close(server);
    exit(0);
  }

  write(server, message, strlen(message)+1);
  if(total_client>=100)
			{
				read(server, message, 255);
				fprintf(stderr, "%s", message);
				close(server);
				exit(0);
			}
	  }
	  else
	  {
		  
		  goto again;
	  }
}
}
