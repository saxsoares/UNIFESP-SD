#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MSG_LEN (64 * 1024)

typedef struct tfila *fila;
struct tfila{
    int PID;
    int tempo;
    fila next;
};

fila novo_no(int PID, int tempo){
    fila novo = malloc(sizeof(*novo));
    novo->PID = PID;
    novo->tempo = tempo;
    novo->next = NULL;
    return novo;
}

fila insere(int PID, int tempo, fila head){
    fila novo = novo_no(PID, tempo);
    fila aux;
    if(head == NULL){
        head = novo;
    }
    else{
        for(aux = head; aux->next != NULL && aux->next->tempo < novo->tempo  ; aux = aux->next){

        }
        novo->next = aux->next;
        aux->next = novo;
    }
    return head;
}

void imprime_fila(fila head){
    fila aux;
    for(aux = head; aux != NULL ; aux = aux->next){
        printf("{%d, %d}->", aux->PID, aux->tempo);
    }
    printf("\n");
}

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

	char req[MSG_LEN];
	socklen_t addrlen;
	struct sockaddr_in caddr;
	int nr, ns;
	while (1) {
		addrlen = sizeof(caddr);
		nr = recvfrom(sock, req, MSG_LEN, 0, (struct sockaddr *)&caddr,
			&addrlen);
		if (nr < 0) {
			perror("recvfrom()");
			return -1;
		}

		ns = sendto(sock, req, nr, 0, (struct sockaddr *)&caddr,
				addrlen);
		if (ns < 0) {
			perror("sendto()");
			return -1;
		}
	}
	return 0;
}

