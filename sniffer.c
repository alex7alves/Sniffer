
/*
    Programa para capturar pacotes da rede via socket.

    Autor : Alex Alves
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
int Criar_socket(int tipo); 
void Capturar_ip(unsigned char*  Buffer, int Tamanho);
void Pacote_tcp(unsigned char*  Buffer, int Tamanho);
void Pacote_udp(unsigned char* Buffer , int Tamanho);
void Processar(unsigned char* Buffer , int Tamanho , int Tipo ,int *num_tcp, int *num_udp);

struct dado {
    int tipo;
    char matricula[9];
    char tamanho[3];
    char nome[41];
 };

FILE *logfile;
struct sockaddr_in source,dest;
int main(int argc, char** argv)
{
    int sock_raw, tamanho_endereco ,tamanho_mensagem,tipo;

    
	int *num_tcp,*num_udp;
	unsigned char *buffer = (unsigned char *)malloc(65536); //Its Big!
	struct sockaddr endereco;
    struct in_addr in;
	//int tamanho_buffer=25536;
	//AlocarBuffer(&buffer,tamanho_buffer);
	AlocarContador(&num_tcp);
	AlocarContador(&num_udp);
    *num_tcp=0;
    *num_udp=0;

    logfile=fopen("log.txt","w");
    if(logfile==NULL) printf("Erro ao abrir o arquivo.");
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
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)Buffer;
    iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
    
    fprintf(logfile,"\n");
    fprintf(logfile,"IP Header\n");
    fprintf(logfile,"   - Versao do IP          : %d\n",(unsigned int)iph->version);
    fprintf(logfile,"   - Tipo de servico  : %d\n",(unsigned int)iph->tos);
    fprintf(logfile,"   - IP Tamanho total   : %d  Bytes(Tamanho do pacote)\n",ntohs(iph->tot_len));
    fprintf(logfile,"   - Identificacao    : %d\n",ntohs(iph->id));
    fprintf(logfile,"   - Protocolo : %d\n",(unsigned int)iph->protocol);
    fprintf(logfile,"   - Checksum : %d\n",ntohs(iph->check));
    fprintf(logfile,"   - IP de origem        : %s\n",inet_ntoa(source.sin_addr));
    fprintf(logfile,"   - IP de destinho   : %s\n",inet_ntoa(dest.sin_addr));
    
}

void Pacote_udp(unsigned char* Buffer , int Tamanho)
{
     
    unsigned short iphdrlen;

    struct iphdr *iph = (struct iphdr *)Buffer;
    iphdrlen = iph->ihl*4;
     
    //struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen);
    //struct dado *p = (struct dado*)(Buffer + iphdrlen + sizeof(struct udphdr ));
    struct udphdr *udph = (struct udphdr*)(Buffer + 20);
    struct dado *p = (struct dado*)(Buffer +28);
    fprintf(logfile,"\n------------------ Pacote UDP ------------------  \n");
     
    Capturar_ip(Buffer,Tamanho);          
     
    fprintf(logfile,"\nUDP Header\n");
    fprintf(logfile,"   - Porta de origem      : %d\n" , ntohs(udph->source));
    fprintf(logfile,"   - Porta de destino : %d\n" , ntohs(udph->dest));
    fprintf(logfile,"   - UDP Length       : %d\n" , ntohs(udph->len));
    fprintf(logfile,"   - UDP Checksum     : %d\n" , ntohs(udph->check));
       


    printf("\nUDP Header\n");
    printf("   - Porta de origem  : %d\n" , ntohs(udph->source));
    printf("   - Porta de destino : %d\n" , ntohs(udph->dest));
    printf("   - UDP Length       : %d\n" , ntohs(udph->len));
    printf("   - UDP Checksum     : %d\n" , ntohs(udph->check));


    fprintf(logfile,"\n **************************************************** ");




    fprintf(logfile,"\n Dados do pacote \n");
    fprintf(logfile,"  - Tipo      : %d\n", p->tipo);
    fprintf(logfile,"  - Matricula : %s\n", p->matricula);
    fprintf(logfile,"  - Tamanho   : %s\n", p->tamanho);
    fprintf(logfile,"  - Nome      : %s\n", p->nome);
       

    printf("\n Dados do pacote \n");
    printf(" - Tipo      : %d\n", p->tipo);
    printf(" - Matricula : %s\n", p->matricula);
    printf(" - Tamanho   : %s\n", p->tamanho);
    printf(" - Nome      : %s\n", p->nome);

    fprintf(logfile,"\n **************************************************** ");
    
}


void Pacote_tcp(unsigned char*  Buffer, int Tamanho)
{
    unsigned short iphdrlen;
    
    struct iphdr *iph = (struct iphdr *)Buffer;
    iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen);
  //  struct pacote *p = (struct pacote)(Buffer + iphdrlen + sizeof(struct tcphdr ));           

    fprintf(logfile,"\n\n------------------ Pacote TCP ------------------  \n");
         
    Capturar_ip(Buffer,Tamanho);
       
    fprintf(logfile,"\n");
    fprintf(logfile,"TCP Header\n");
    fprintf(logfile,"   - Porta de origem       : %u\n",ntohs(tcph->source));
    fprintf(logfile,"   - Porta de destino : %u\n",ntohs(tcph->dest));
    fprintf(logfile,"   - Sequence Number    : %u\n",ntohl(tcph->seq));
    fprintf(logfile,"   - Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
    fprintf(logfile,"   - Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
    fprintf(logfile,"   - Urgent Flag          : %d\n",(unsigned int)tcph->urg);
    fprintf(logfile,"   - Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
    fprintf(logfile,"   - Push Flag            : %d\n",(unsigned int)tcph->psh);
    fprintf(logfile,"   - Reset Flag           : %d\n",(unsigned int)tcph->rst);
    fprintf(logfile,"   - Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
    fprintf(logfile,"   - Finish Flag          : %d\n",(unsigned int)tcph->fin);
    fprintf(logfile,"   - Window         : %d\n",ntohs(tcph->window));
    fprintf(logfile,"   - Checksum       : %d\n",ntohs(tcph->check));
    fprintf(logfile,"   - Urgent Pointer : %d\n",tcph->urg_ptr);
    fprintf(logfile,"\n");
    fprintf(logfile,"\n **************************************************** ");
    
}


void Processar(unsigned char* Buffer , int Tamanho , int Tipo, int *num_tcp, int *num_udp)
{
    struct iphdr *iph = (struct iphdr*)Buffer;
	// Verificando o tipo de pacote recebido
    switch (iph->protocol) 
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

 

