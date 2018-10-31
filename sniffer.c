
/*
    Programa para capturar pacotes da rede via socket.

    Autor : Alex Alves

    para capturar pacotes UDP execute :

    ./sniffer

    Para capturar pacotes tcp execute :

    ./sniffer 1
*/


#include<stdio.h> 
#include<stdlib.h>    
#include<string.h>
#include<netinet/ip.h> 
#include<netinet/tcp.h>   
#include<netinet/udp.h>      
#include<sys/socket.h>
#include<arpa/inet.h>
 
void AlocarContador(int **cont ); 
void AlocarBuffer(unsigned char  **Buffer, int t); 
void Verificar(char* mensagem, int valor);
int  Criar_socket(int tipo); 
void Capturar_ip(unsigned char*  Buffer, int Tamanho);
void Pacote_tcp(unsigned char*  Buffer, int Tamanho);
void Pacote_udp(unsigned char* Buffer , int Tamanho);
void Processar(unsigned char* Buffer , int Tamanho , int Tipo ,int *num_tcp, int *num_udp);

void MostrarIP(int n1, int n2, int n3);
void MostrarUDP(int n1, int n2, int n3, int n4);
void MostrarDado(char n1, char* m, char* m2, char* m3);
void MostrarDado2(char n1, char* m);

struct dado {
    char tipo;
    char matricula[8]; 
    char tamanho[2];   
    char nome[50];
};

struct dado2 {
    char tipo;
    char matricula[8];
};


int main(int argc, char** argv)
{
    int sock_raw, tamanho_endereco ,tamanho_mensagem,tipo;
    
	int *num_tcp,*num_udp;
	unsigned char *buffer = (unsigned char *)malloc(65536); 
	struct sockaddr endereco;
    struct in_addr in;

	AlocarContador(&num_tcp);
	AlocarContador(&num_udp);
    *num_tcp=0;
    *num_udp=0;

    printf("Iniciando captura...\n");
    if (argc != 2) {
        tipo=0;
    }else {
        tipo = atoi(argv[1]);
    }
	sock_raw = Criar_socket(tipo);
    
	Verificar("Erro ao criar o RAW Socket",sock_raw);
   
    while(1)
    {

        tamanho_endereco = sizeof(endereco);
        //Recebendo pacotes
        tamanho_mensagem = recvfrom(sock_raw , buffer ,65536 , 0 , &endereco , &tamanho_endereco);
		
		Verificar("Erro ao receber pacotes",tamanho_endereco);
		
        Processar(buffer ,tamanho_mensagem,tipo, num_tcp, num_udp);
        printf(" Pacotes TCP : %d UDP : %d \n", *num_tcp, *num_udp);
    }
   // close(sock_raw);
    printf("Terminado");
    return 0;
}
 
 
void AlocarContador(int **cont )
{
	*cont = (int *)malloc(sizeof(int));

} 
 
void AlocarBuffer(unsigned char  **Buffer, int t)
{
	*Buffer = (unsigned char *)malloc(4096); 
} 

void  Verificar(char* mensagem, int valor)
{
	if ( valor < 0 ){

        printf("%s %d\n",mensagem, valor);
		exit(1);
    }
} 

int Criar_socket(int tipo)
{
	int sock_raw;
	
	if(tipo==1){
		//Criando o RAW socket TCP
		sock_raw = socket(AF_INET , SOCK_RAW , IPPROTO_TCP);
		Verificar("Erro ao criar o RAW Socket",sock_raw);
	}
	else{
		//Criando o RAW socket UDP
		sock_raw = socket(AF_INET , SOCK_RAW , IPPROTO_UDP);
		Verificar("Erro udp ao criar o RAW Socket",sock_raw);
	}
	return sock_raw;
	
}


void Capturar_ip(unsigned char*  Buffer, int Tamanho)
{
    struct sockaddr_in source,dest;  
    struct iphdr *ip = (struct iphdr *)Buffer;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ip->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ip->daddr;
    
    printf("\n");
    printf("IP Header\n");
    printf("   - Versao do IP          : %d\n",(unsigned int)ip->version);
    printf("  -  Tipo de servico       : %d\n",(unsigned int)ip->tos);
    printf("   - IP Tamanho total      : %d\n",ntohs(ip->tot_len));
    printf("   - Identificacao         : %d\n",ntohs(ip->id));
    printf("   - Protocolo             : %d\n",(unsigned int)ip->protocol);
    printf("   - Checksum              : %d\n",ntohs(ip->check));
    printf("   - IP de origem          : %s\n",inet_ntoa(source.sin_addr));
    printf("   - IP de destinho        : %s\n",inet_ntoa(dest.sin_addr));
    
}

void Pacote_udp(unsigned char* Buffer , int Tamanho)
{

    struct udphdr *udp = (struct udphdr*)(Buffer + 20);

    char *t = (char*)(Buffer +28);

    if (*t=='1'){
        struct dado *p = (struct dado*)(Buffer +28);

        Capturar_ip(Buffer,Tamanho);  
        MostrarUDP(ntohs(udp->source),ntohs(udp->dest),ntohs(udp->len),ntohs(udp->check));
        MostrarDado(p->tipo,p->matricula,p->tamanho,p->nome);

    }else if (*t=='2'){
        struct dado2 *p = (struct dado2*)(Buffer +28);

        Capturar_ip(Buffer,Tamanho);  
        MostrarUDP(ntohs(udp->source),ntohs(udp->dest),ntohs(udp->len),ntohs(udp->check));
        MostrarDado2(p->tipo,p->matricula);
   
    }
   
}


void Pacote_tcp(unsigned char*  Buffer, int Tamanho)
{

    /*
        A funcao esta incompleta 
    */


    struct iphdr *ip = (struct iphdr *)Buffer;

    struct tcphdr *tcp=(struct tcphdr*)(Buffer + 20);

    Capturar_ip(Buffer,Tamanho);
       
    printf("\n");
    printf("TCP Header\n");
    printf("   - Porta de origem       : %d\n",ntohs(tcp->source));
    printf("   - Porta de destino       : %d\n",ntohs(tcp->dest));


}


void Processar(unsigned char* Buffer , int Tamanho , int Tipo, int *num_tcp, int *num_udp)
{
    struct iphdr *ip = (struct iphdr*)Buffer;
	// Verificando o tipo de pacote recebido
    switch (ip->protocol) 
    {
        case 6:  //TCP 
            ++(*num_tcp);
            Pacote_tcp(Buffer,Tamanho);
            break;
         
        case 17: //UDP
            ++(*num_udp);
            Pacote_udp(Buffer,Tamanho);
            break;
         
        default:      
            break;
    }
    
}  

void MostrarIP(int n1, int n2, int n3)
{

}
void MostrarUDP(int n1, int n2, int n3, int n4)
{

    printf("\nUDP Header\n");
    printf("   - Porta de origem  : %d\n" ,n1);
    printf("   - Porta de destino : %d\n" ,n2);
    printf("   - UDP Length       : %d\n" ,n3);
    printf("   - UDP Checksum     : %d\n" ,n4);
}
void MostrarDado(char n1, char* m, char* m2, char* m3)
{
    printf("\nDados do pacote \n");
    printf(" - Tipo      : %c\n",n1);
    printf(" - Matricula : %s\n",m);
    printf(" - Tamanho   : %s\n",m2);
    printf(" - Nome      : %s\n",m3);
    printf("\n **************************************************** \n ");
}
void MostrarDado2(char n1, char* m){
    printf("\nDados do pacote \n");
    printf(" - Tipo      : %c\n",n1);
    printf(" - Matricula : %s\n",m);
    printf("\n **************************************************** \n ");
}

