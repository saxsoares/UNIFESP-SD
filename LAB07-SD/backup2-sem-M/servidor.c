#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include "server_cent.h"

typedef struct Sitem *Titem;
struct Sitem{
    long PID;
    long tempo;
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

int insere(Tfila fila, int PID, int tempo){
  if(fila->tamanho < fila->N){
    Titem novo = malloc(sizeof(struct Sitem));
    novo->PID = PID;
    novo->tempo = tempo;
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
        printf("{%ld,%ld}->", aux->PID, aux->tempo);
        aux = aux->next;
    }
    printf("\n____________________________________________________\n");
}

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static long TEMPO = 0;
static Tfila fila;
static long TOKEN = -1;

void *consumidor(void *args) {
	Sitem dados = *(Sitem *)args;

	insere(fila, dados->PID, dados->tempo);
}

reply_out * req_1_svc(request_in *req_in, struct svc_req *rqstp){
    static reply_out rep_out;
    if(!fila)
        fila = init(10);

    printf("Request (pid_c, tempo) = (%ld, %ld)\n", req_in->pid_c, req_in->tempo_c);
    insere(fila, req_in->pid_c, req_in->tempo_c);
    while(fila->head->PID != req_in->pid_c){ //
        sleep(1);
    }
    rep_out.pid_c = req_in->pid_c;
    rep_out.recurso = TOKEN;
    rep_out.tempo_s = ++TEMPO;

    return &rep_out;
}

int * rel_1_svc(release *rel_in,struct svc_req *rqstp){
    int *result = malloc(sizeof(int));

    TOKEN = rel_in->recurso_n;
    TEMPO = TEMPO > rel_in->tempo_c ? (TEMPO + 1): (rel_in->tempo_c + 1);
    if(fila)removeItem(fila);

    printf("%ld Devolveu TOKEN\n", rel_in->pid_c);
    *result = 1;
    return result;
}
//#pragma GCC diagnostic ignored "-Wunused-variable"
