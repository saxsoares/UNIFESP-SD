#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "fila.c"

typedef struct TRecurso *Recurso;
struct TRecurso{
    int valor;
};

typedef struct requisicao *treq;
struct requisicao{
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
	size_t reqlen = sizeof(req);
	socklen_t addrlen;
	struct sockaddr_in caddr;
	int nr, ns;
	while (1) {
		addrlen = sizeof(caddr);
		nr = recvfrom(sock, req, reqlen, 0, (struct sockaddr *)&caddr,	&addrlen);
		if (nr < 0) {
			perror("recvfrom()");
			return -1;
		}
        // REQ = 100 -> REQUEST_TOKEN
        // REQ = 200 -> RELEASE_TOKEN
        // REQ = 300 -> REPLY_ALLOW
		ns = sendto(sock, req, nr, 0, (struct sockaddr *)&caddr, addrlen);
		if (ns < 0) {
			perror("sendto()");
			return -1;
		}

		//ENVIA ACK
        //VERIFICA QUE TIPO DE MSG
        //      //REQUEST TOKEN
                    //Verifica TOKEN. EM USO?
                        //NẪO! RETORNA TOKEN
                        //SIM!
                            //Insere na fila
        //      //RELEASE TOKEN
        //      //VERIFICA FILA
        //      //VAZIA?
                    //SIM -> NADA
                    //NÃO -> TOKEN TO REMOVE(FILA)

	}
	return 0;
}

