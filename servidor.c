#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define REQUEST_TOKEN 100
#define RELEASE_TOKEN 200
#define REPLY_ALLOW 300
#define AGUARDANDO 400

typedef struct TRecurso *Recurso;
struct TRecurso{
    char flag;
    int valor;
};

typedef struct request *treq;
struct request{
    int cod;
    int PID;
    int tempo;
    Recurso rec;
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
  return 0;
}

Titem removeItem(Tfila fila){
    Titem item = malloc(sizeof(item));
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
    return NULL;
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

    int TEMPO;
    TEMPO = 0;
    Tfila fila;
    fila = init(10);
    Recurso Token = malloc(sizeof(Recurso));

    Token->valor = 0;
    Token->flag = -1;

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

	treq req;
	treq ack;
	treq rep;
	size_t reqlen = sizeof(struct TRecurso);
	socklen_t addrlen;
	struct sockaddr_in caddr;
	int nr, ns;
	Titem cliente;

	while (1) {
        addrlen = sizeof(caddr);
		// PRINTS

        imprime(fila);
        printf("\nAguardando mensagem:\n");
		nr = recvfrom(sock, &req, reqlen, 0, (struct sockaddr *)&caddr,	&addrlen);
		if (nr < 0) {
			perror("recvfrom()");
			return -1;
		}
		printf("Recebido MSG: req->cod = %d from %d\n", req->cod, req->PID);
		TEMPO = (TEMPO > req->tempo) ? (TEMPO+1) : (req->tempo+1);
        // REQ = 100 -> REQUEST_TOKEN
        // REQ = 200 -> RELEASE_TOKEN
        // REQ = 300 -> REPLY_ALLOW
        // REQ = 400 -> ACK
        //ENVIA ACK
        ack->cod = req->cod;
        ack->PID = req->PID;
        ack->tempo = TEMPO++;
        printf("Enviando ACK (ack->cod = %d, ack->PID = %d, ack->tempo=%d)\n", ack->cod, ack->PID, ack->tempo);
		ns = sendto(sock, &ack, sizeof(ack), 0, (struct sockaddr *)&caddr, addrlen);
		if (ns < 0) {
			perror("sendto()");
			return -1;
		}
        //VERIFICA QUE TIPO DE MSG
        if(req->cod == REQUEST_TOKEN){       //      //REQUEST_TOKEN
            if(Token->flag == -1){          //Verifica TOKEN. EM USO?             //NẪO! RETORNA TOKEN
                printf("Recebido REQUEST_TOKEN & Fila Vazia. (%d, %d)\n", req->PID, req->tempo);
                Token->flag = req->PID;
                rep->cod = REPLY_ALLOW;
                rep->PID = req->PID;
                rep->rec = Token;
                rep->tempo = TEMPO++;
                printf("Enviando REPLY_ALLOW to %d (Token, tempo)=(%d, %d)\n", rep->PID, Token->valor, rep->tempo);
                ns = sendto(sock, &rep, sizeof(rep), 0, (struct sockaddr *)&caddr, addrlen);
                if (ns < 0) {
                    perror("sendto()");
                    return -1;
                }
            }
            else{//SIM!  //Insere na fila
                printf("Enfileirando: Cliente (%d, %d)\n", req->PID, req->tempo);
                insere(fila, req->PID, req->tempo, caddr, addrlen);
            }
        }
        else if(req->cod == RELEASE_TOKEN){ //      //RELEASE TOKEN
            // ATUALIZAR TOKEN
            Token->valor = req->rec->valor;
            Token->flag = -1;
            printf("Recebido RELEASE_TOKEN. novo Valor: (%d)\n", Token->valor );
            if(fila->tamanho == 0)//      //VERIFICA FILA
                continue;
            else{
                cliente = removeItem(fila);
                rep->cod = REPLY_ALLOW;
                rep->PID = cliente->PID;
                rep->rec = Token;
                rep->tempo = TEMPO++;
                printf("Enviando para prox da fila: (PID, Tempo, Token) = (%d, %d, %d)\n", rep->PID, rep->tempo, rep->rec->valor);
                ns = sendto(sock, &rep, sizeof(rep), 0, (struct sockaddr *)&cliente->caddr, cliente->addrlen);
                if (ns < 0) {
                    perror("sendto()");
                    return -1;
                }
                Token->flag = cliente->PID;
            }
        }
	}
	return 0;
}

