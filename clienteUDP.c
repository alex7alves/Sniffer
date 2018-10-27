
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct dado {
        int idade;
        char nome[40];
 };


int main( )
{
    int sockfd;
    int len;
    socklen_t len_recv;
    struct sockaddr_in address;
    int result;

    

    struct dado pessoa;

    pessoa.idade=20;
    strcpy(pessoa.nome, "Jose Silva");

    unsigned short porta = 1234;
    
    sockfd  = socket(AF_INET, SOCK_DGRAM,0);  // criacao do socket
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");  // localhost
    //address.sin_addr.s_addr = inet_addr("10.13.96.16"); // euler
    // address.sin_addr.s_addr = inet_addr("10.13.127.255"); // broadcast
    address.sin_port = htons(porta);
    
    len = sizeof(address);
    printf("O cliente8 vai começar a enviar os dados \n");
    for(int i=0;i<10;i++)
    {
        printf("Enviando %d \n", i);
        sendto(sockfd, &pessoa,sizeof(pessoa),0,(struct sockaddr *) &address, len);
        sleep(1);
    }
    close(sockfd);
    exit(0);
}

