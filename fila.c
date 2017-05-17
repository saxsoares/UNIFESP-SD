/*Considera a situacao em que o primeiro da fila eh maior que o elemento inserido*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

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

int main(){
    Tfila fila;

    fila = init(10);

    fila->tamanho = 0;
    fila->head = NULL;
    struct sockaddr_in caddr;
    socklen_t addrlen = sizeof(struct sockaddr);
    int i,j=0;

    for(i=0; i<20; i+=2)
        insere(fila,++j,rand()%10,caddr, addrlen);

    imprime(fila);
    for(i=0; i<20; i+=2){
        printf("remove %d\n",removeItem(fila)->PID);
        imprime(fila);
    }
    return 0;
}
