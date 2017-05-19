#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#define REQUEST_TOKEN 10
#define RELEASE_TOKEN 20
#define REPLY_ALLOW 30
#define ACK 40

struct SToken{
    char flag;
    int valor;
};

typedef struct request *treq;
struct request{
    int rec;
    char cod;
    int tempo;
    int PID;
};

typedef struct Sitem *Titem;
struct Sitem{
    int PID;
    int tempo;
    socklen_t addrlen;
    struct sockaddr_in caddr;
    Titem next;
};

typedef struct Sfila *Tfila;
struct Sfila{
    int tamanho;
    int N;
    Titem head;
};

Tfila init( int N){
  Tfila fila = (Tfila) malloc(sizeof(struct Sfila));
  fila->tamanho = 0;
  fila->N = N;
  fila->head = NULL;
  return fila;
}

int EstaNaFila(Tfila fila, Titem Cliente){
    Titem aux = fila->head;
    while(aux != NULL){
        if(aux->PID == Cliente->PID)
            return 1;
        aux = aux->next;
    }
    return 0;
}

int insere(Tfila fila, int PID, int tempo, struct sockaddr_in caddr, socklen_t addrlen){
  if(fila->tamanho < fila->N){
    Titem novo = malloc(sizeof(struct Sitem));
    novo->PID = PID;
    novo->tempo = tempo;
    novo->addrlen = addrlen;
    novo->caddr = caddr;
    if(!EstaNaFila(fila, novo)){
        if(fila->tamanho == 0){
          novo->next = NULL;
          fila->head = novo;
        }
        else{
          if(fila->head->tempo > tempo){
            novo->next = fila->head;
            fila->head = novo;
          }
          else{
            Titem aux = fila->head;
            while(aux->next != NULL && aux->next->tempo < tempo)
              aux = aux->next;
            novo->next = aux->next;
            aux->next = novo;
          }
        }
        fila->tamanho++;
        return 1;
     }
  }
  return 0;
}

Titem removeItem(Tfila fila){
    Titem item = malloc(sizeof(struct Sitem));
    if(fila->head != NULL){
        Titem aux = fila->head;
        fila->head = fila->head->next;
        item->addrlen = aux->addrlen;
        item->caddr = aux->caddr;
        item->PID = aux->PID;
        item->tempo = aux->tempo;
        free(aux);
        fila->tamanho--;
        return item;
    }
    return item;
}

void imprime(Tfila fila){
    Titem aux = fila->head;
    while(aux != NULL){
        printf("{%d,%d}->", aux->PID, aux->tempo);
        aux = aux->next;
    }
    printf("\n____________________________________________________\n");
}

int main(int argc, char **argv) {
	if (argc != 2){
		printf("uso: %s <porta>\n", argv[0]);
		return 0;
	}
    printf("\n\n");
    long int TEMPO;

    TEMPO = 0;
    Tfila fila;
    fila = init(10);
    struct SToken Token;

    Token.valor = 30;
    Token.flag = -1;

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		perror ("socket()");
		return -1;
	}

	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(atoi(argv[1]));
	srv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
		perror("bind()");
		return -1;
	}

	treq req = (treq)malloc(sizeof(struct request));
	treq ack = (treq)malloc(sizeof(struct request));
	treq rep = (treq)malloc(sizeof(struct request));
	size_t reqlen = sizeof(struct request);
	struct sockaddr_in caddr;
	socklen_t addrlen;
	addrlen = sizeof(caddr);
	int nr, ns;
	Titem cliente;
    int len=20;
    char client_ip[len];
	while (1) {
        bzero(client_ip, len);
        addrlen = sizeof(caddr);
		// PRINTS
        printf("_________________fila__________________\n");
        imprime(fila);
        printf("\nAguardando mensagem:\n");
		nr = recvfrom(sock, req, reqlen, 0, (struct sockaddr *)&caddr,	&addrlen);
		if (nr < 0) {
			perror("recvfrom()");
			return -1;
		}

        inet_ntop(AF_INET, &(caddr.sin_addr), client_ip, len);
		printf("Mensagem recebida: req->cod = %d\tfrom %s\t(%d, %d)\n", req->cod, client_ip, req->PID, req->tempo);
		TEMPO = (TEMPO > req->tempo) ? (TEMPO+1)%INT_MAX : (req->tempo+1)%INT_MAX;

		// REQ = 100 -> REQUEST_TOKEN
        // REQ = 200 -> RELEASE_TOKEN
        // REQ = 300 -> REPLY_ALLOW
        // REQ = 400 -> ACK

        //ENVIA ACK
        ack->cod = ACK;
        ack->PID = req->PID;
        ack->tempo = TEMPO;
        printf("Enviando ACK (ack->cod = %d, ack->PID = %d, ack->tempo=%d)\n", ack->cod, ack->PID, ack->tempo);
		ns = sendto(sock, ack, reqlen, 0, (struct sockaddr *)&caddr, addrlen);
		if (ns < 0) {
			perror("sendto()");
			return -1;
		}
		sleep(1);
        //VERIFICA TIPO DA MSG
        if(req->cod == REQUEST_TOKEN){       //      //REQUEST_TOKEN
            printf("TRATANDO REQUISIÇÃO DE TOKEN:\n");
            if(Token.flag == -1){          //Verifica TOKEN. EM USO?             //NẪO! RETORNA TOKEN
                printf("\tRecebido REQUEST_TOKEN & Fila Vazia. (%d, %d)\n", req->PID, req->tempo);
                Token.flag = req->PID;
                rep->cod = REPLY_ALLOW;
                rep->PID = req->PID;
                rep->rec = Token.valor;
                rep->tempo = (++TEMPO)%INT_MAX;
                printf("\tEnviando REPLY_ALLOW to %d(%s)\n\t(cod, Token, tempo)=(%d, %d, %d)\n", rep->PID, client_ip, rep->cod, rep->rec, rep->tempo);
                ns = sendto(sock, rep, sizeof(rep), 0, (struct sockaddr *)&caddr, addrlen);
                if (ns < 0) {
                    perror("sendto()");
                    return -1;
                }
            }
            else{//SIM!  //Insere na fila
                printf("\tEnfileirando: Cliente (%d, %d)\n", req->PID, req->tempo);
                insere(fila, req->PID, req->tempo, caddr, addrlen);
            }
        }
        else if(req->cod == RELEASE_TOKEN){ //      //RELEASE TOKEN
            // ATUALIZAR TOKEN
            Token.valor = req->rec;
            Token.flag = -1;
            printf("Recebido RELEASE_TOKEN. Novo Valor: (%d)\n", Token.valor );
            if(fila->tamanho == 0){//      //VERIFICA FILA
                printf("\tFila vazia, Reiniciando...\n");
                continue;
            }
            else{
                printf("Removendo primeiro da fila:\n");
                cliente = removeItem(fila);
                rep->cod = REPLY_ALLOW;
                rep->PID = cliente->PID;
                rep->rec = Token.valor;
                rep->tempo = (++TEMPO)%INT_MAX;
                Token.flag = cliente->PID;
                bzero(client_ip, len);
                inet_ntop(AF_INET, &(cliente->caddr.sin_addr), client_ip, len);
                printf("Enviando para prox da fila: %d(%s) (Token, tempo)=(%d, %d)\n", rep->PID, client_ip, rep->tempo, rep->rec);
                ns = sendto(sock, rep, sizeof(rep), 0, (struct sockaddr *)&cliente->caddr, cliente->addrlen);
                if (ns < 0) {
                    perror("sendto()");
                    return -1;
                }
            }
        }
	}
	return 0;
}

