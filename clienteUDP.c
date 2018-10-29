
/*
	Programa para enviar mensagens via socket UDP

	Autor : Alex Alves.
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct tipo_1 {
    int tipo;
    char matricula[9];
    char tamanho[3];
    char nome[41];
};

struct tipo_2 {
    char tipo;
    char matricula[8];
};

int main( )
{
    int sockfd;
    int len;
    socklen_t len_recv;
    struct sockaddr_in address;
    int result;

    

    struct tipo_1 mensagem;
    struct tipo_2 mensagem2;

    // Preenchendo mensagem do tipo 1
    //strcpy(mensagem.tipo, "1");
    mensagem.tipo = 1;
    printf(" mensagem tipo %d\n",mensagem.tipo);
    strcpy(mensagem.matricula, "12345678");
    strcpy(mensagem.tamanho, "10");
    strcpy(mensagem.nome, " Alan Ribeiro");


printf(" matricula %s\n",mensagem.matricula);
printf(" tamanho %s\n",mensagem.tamanho);
printf(" nome %s\n",mensagem.nome);
    // preenchendo mensagem do tipo 2
   // strcpy(mensagem2.tipo, "2");
    mensagem2.tipo = '2';
    strcpy(mensagem2.matricula, "90345612");

    unsigned short porta = 1234;
    
    sockfd  = socket(AF_INET, SOCK_DGRAM,0);  // criacao do socket
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");  // localhost
    //address.sin_addr.s_addr = inet_addr("10.13.96.16"); // euler
    // address.sin_addr.s_addr = inet_addr("10.13.127.255"); // broadcast
    address.sin_port = htons(porta);
    
    len = sizeof(address);
    printf("O cliente8 vai começar a enviar os dados \n");
    for(int i=0;i<4;i++)
    {
        printf("Enviando %d \n", i);
        sendto(sockfd, &mensagem,sizeof(mensagem),0,(struct sockaddr *) &address, len);
       // sendto(sockfd, &mensagem2,sizeof(mensagem2),0,(struct sockaddr *) &address, len);
        sleep(1);
    }
    close(sockfd);
    exit(0);
}

