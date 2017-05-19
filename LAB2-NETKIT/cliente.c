#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

#define REQUEST_TOKEN 10
#define RELEASE_TOKEN 20
#define REPLY_ALLOW 30
#define ACK 40

typedef struct request *treq;
struct request{
    int rec;
    char cod;
    int tempo;
    int PID;
};

int main(int argc, char **argv){
    srand(time(NULL));
	if (argc != 4) {
		printf("uso: %s <ip_srv> <porta_srv> <PID>\n", argv[0]);
		return 0;
	}
	printf("\n\n");
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		perror("socket()");
		return -1;
	}
    int TEMPO, PID;
    PID = atoi(argv[3]);
    TEMPO = 0;
	int ns, nr;
	struct sockaddr_in saddr, addr;
	socklen_t addr_len;
	saddr.sin_port = htons(atoi(argv[2]));
	saddr.sin_family = AF_INET;
	inet_aton(argv[1], &saddr.sin_addr);

    treq req = malloc(sizeof(struct request));
    treq resp = malloc(sizeof(struct request));
    addr_len = sizeof(addr);
    int len=20;
    char buffer[len];
    char aguardar = 0;
    while(1){
        bzero(buffer, len);

        req->cod = REQUEST_TOKEN;
        req->PID = PID;
        req->tempo = (++TEMPO)%INT_MAX;
        req->rec = -1;

        ns = sendto(sock, req, sizeof(struct request), 0,	(struct sockaddr *)&saddr,	sizeof(saddr));
        if (ns < 0) {
            perror("sendto()");
            return -1;
        }
        printf("Enviando requisição:\n");
        printf("req->cod = %d\nreq->PID = %d\nreq->tempo = %d\n", req->cod, req->PID, req->tempo);

        while(1){
            printf("\n__________________________________________________\nAguardando resposta\n");
            nr = recvfrom(sock, resp, sizeof(struct request), 0, (struct sockaddr *)&addr, &addr_len);
            if (nr < 0) {
                perror("recvfrom()");
                return -1;
            }
            if(resp->cod == ACK){   //RECEBENDO ACK
                inet_ntop(AF_INET, &(addr.sin_addr), buffer, len);
                printf("\nRecebeu ACK do servidor(%s)\n",buffer);
                printf("resp->cod: %d\n", resp->cod);
                printf("resp->PID: %d\n", resp->PID);
                printf("resp->tempo: %d\n", resp->tempo);
                printf("resp->rec: %d\n", resp->rec);
                if(resp->PID != PID){
                    aguardar++;
                    if(aguardar > 3)
                        break;
                    printf("Servidor respondendo, mas ACK não é o meu.\n");
//                    sleep(1);
                    continue;
                }
                else{
                    printf("Servidor respondendo. Aguardando minha vez...\n");
                    continue;
                }
            }
            else if(resp->cod == REPLY_ALLOW){  //RECEBENDO TOKEN
                ++TEMPO;
                TEMPO = TEMPO%INT_MAX;
                inet_ntop(AF_INET, &(addr.sin_addr), buffer, len);
                printf("\nServidor(%s) concedeu token. Processando...\n",buffer);
                sleep(rand()%10);
                printf("resp->cod: %d\n", resp->cod);
                printf("resp->PID: %d\n", resp->PID);
                printf("resp->tempo: %d\n", resp->tempo);
                printf("resp->rec: Valor antigo = %d\t", resp->rec);
                resp->rec = resp->rec + 1;
                printf("Novo valor = %d\n", resp->rec);
                //Devolver TOKEN
                req->cod = RELEASE_TOKEN;
                req->PID = PID;
                req->tempo = (++TEMPO)%INT_MAX;
                req->rec = resp->rec;

                ns = sendto(sock, req, sizeof(struct request), 0,	(struct sockaddr *)&saddr,	sizeof(saddr));
                if (ns < 0) {
                    perror("sendto()");
                    return -1;
                }
                printf("Enviando release:\n");
                printf("req->cod = %d\nreq->PID = %d\nreq->tempo = %d\nTOKEN = %d\n", req->cod, req->PID, req->tempo, req->rec);
                aguardar = 0;
                sleep(rand()%PID);
                break;
            }
        }
    }
	return 0;
}
