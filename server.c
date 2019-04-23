#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

void child(int);

int main(int argc, char *argv[]){  
  int sd, client, portNumber;
  struct sockaddr_in servAdd; 
  
  // server socket address
  
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }

  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);
  
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
  listen(sd, 5);

  while(1){
    client = accept(sd, NULL, NULL);
    printf("Got a client\n");

    if(!fork())
      child(client);

    close(client);
  }
}

void child(int sd){
	char message[255];
	int server_score =0,client_score=0,total_server=0,dice =6,total_client=0;
	long int ss=0;
	write(sd, "\n* * * * * * * * * * * Let's Start the Game * * * * * * * * * * *\n\n",70);

	while(1){
		if(total_server>=100 || total_client>=100)
		{
			if(total_server>=100)
			{
				write(sd,"Game over: You lost the game",255);
			}
			else
			{
				write(sd,"Congrats!!! You won the game",255);
			}
			close(sd);
			exit(0);
		}
		else
		{
		sleep(1);
		write(sd," Game on: You can now play dice\n",33);
		sleep(1);
		server_score = (int) time(&ss)%dice + 1;
		sprintf(message,"%d\n",server_score);
		total_server+=server_score;
		write(sd, message, strlen(message)+1);
		if(total_server>=100)
			{
				write(sd,"Game over: You lost the game",255);
				close(sd);
				exit(0);
			}
		

		if(!read(sd, message, 255)){
			close(sd);
			fprintf(stderr, "Bye, my client is dead, wait for a new one\n");
			exit(0);
		}
		client_score = atoi(message);
		total_client+=client_score;
		fprintf(stderr, "Client send back: %s\n", message);
		}
	}
}