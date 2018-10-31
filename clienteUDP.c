
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
    char tipo;
    char matricula[8];
    char tamanho[2];
    char nome[50];
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
    mensagem.tipo ='1';
    strcpy(mensagem.matricula, "1234567");
    strcpy(mensagem.tamanho, "7\0");
    strcpy(mensagem.nome, "Alan Ribeiro");


    mensagem2.tipo ='2';
    strcpy(mensagem2.matricula, "9034561");

    unsigned short porta = 1234;
    
    sockfd  = socket(AF_INET, SOCK_DGRAM,0);  
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");  // localhost
    address.sin_port = htons(porta);
    
    len = sizeof(address);
    printf("O cliente vai começar a enviar os dados \n");
    for(int i=0;i<4;i++)
    {
        printf("Enviando %d \n", i);
        sendto(sockfd, &mensagem,sizeof(mensagem),0,(struct sockaddr *) &address, len);
        sendto(sockfd, &mensagem2,sizeof(mensagem2),0,(struct sockaddr *) &address, len);
        sleep(1);
    }
    close(sockfd);
    exit(0);
}

