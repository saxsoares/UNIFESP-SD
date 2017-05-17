#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "fila.c"

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



int main(int argc, char **argv) {
	if (argc != 2){
		printf("uso: %s <porta>\n", argv[0]);
		return 0;
	}

    int TEMPO;
    TEMPO = 0;
    Tfila fila;
    init(&fila, 10);
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
	#define MSG_LEN (64 * 1024)
	treq req;
	treq ack;
	treq rep;
	size_t reqlen = sizeof(req);
	socklen_t addrlen;
	struct sockaddr_in caddr;
	int nr, ns;
	Titem cliente;
	while (1) {
		addrlen = sizeof(caddr);
		nr = recvfrom(sock, &req, reqlen, 0, (struct sockaddr *)&caddr,	&addrlen);
		if (nr < 0) {
			perror("recvfrom()");
			return -1;
		}
		TEMPO = (TEMPO > req->tempo) ? (TEMPO+1) : (req->tempo+1);
        // REQ = 100 -> REQUEST_TOKEN
        // REQ = 200 -> RELEASE_TOKEN
        // REQ = 300 -> REPLY_ALLOW
        // REQ = 400 -> AGUARDE
        //ENVIA ACK
        ack->cod = req->cod;
        ack->PID = req->PID;
        ack->tempo = TEMPO;
		ns = sendto(sock, &ack, sizeof(ack), 0, (struct sockaddr *)&caddr, addrlen);
		if (ns < 0) {
			perror("sendto()");
			return -1;
		}
        //VERIFICA QUE TIPO DE MSG
        if(req->cod == REQUEST_TOKEN){       //      //REQUEST_TOKEN
            if(Token->flag == -1){          //Verifica TOKEN. EM USO?             //NẪO! RETORNA TOKEN
                rep->cod = REPLY_ALLOW;
                rep->PID = req->PID;
                rep->rec = Token;
                rep->tempo = TEMPO;
                ns = sendto(sock, &rep, sizeof(rep), 0, (struct sockaddr *)&caddr, addrlen);
                if (ns < 0) {
                    perror("sendto()");
                    return -1;
                }
                TEMPO++;
            }
            else{//SIM!  //Insere na fila
                insere(&fila, req->PID, req->tempo, caddr, addrlen);
            }
        }
        else if(req->cod == RELEASE_TOKEN){ //      //RELEASE TOKEN
            // ATUALIZAR TOKEN
            Token->flag = -1;
            Token->valor = req->rec->valor;

            if(fila.tamanho == 0)//      //VERIFICA FILA
                continue;
            else{
                cliente = removeItem(&fila);
                rep->cod = REPLY_ALLOW;
                rep->PID = cliente->PID;
                rep->rec = Token;
                rep->tempo = TEMPO++;
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

