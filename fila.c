/*Considera a situacao em que o primeiro da fila eh maior que o elemento inserido*/
#include <stdio.h>
#include <stdlib.h>

typedef struct Titem{
  int PID;
  int tempo;
  struct Titem *next;
}Titem;

typedef struct Tfila{
  int tamanho;
  Titem *head;
}Tfila;

int insere(int PID, int tempo, Tfila *fila) {
  Titem *novo = (Titem*)malloc(sizeof(Titem*));
  if(fila->tamanho == 0){
      novo->PID = PID;
      novo->tempo = tempo;
      novo->next = NULL;
      fila->head = novo;
      fila->tamanho++;
      return 1;
  }
  else{
    Titem *aux = fila->head;
	while(aux->next != NULL){
	      if(aux->tempo < tempo){
   	         aux = aux->next;
		     continue;
	      }
	      else{
            novo->PID = aux->PID;
	        novo->tempo = aux->tempo;
            novo->next = aux->next;
            aux->next = novo;
    		aux->PID = PID;
     		aux->tempo = tempo;
    		fila->tamanho++;
            return 1;
          }
	}
	if(aux->tempo < tempo){
	      novo->PID = PID;
	      novo->tempo = tempo;
	      novo->next = NULL;
	      aux->next = novo;
          fila->tamanho++;
	}
	else{
	      novo->PID = aux->PID;
	      novo->tempo = aux->tempo;
	      novo->next = aux->next;
              aux->next = novo;
	      aux->PID = PID;
 	      aux->tempo = tempo;
	      fila->tamanho++;
	}
  }
  return -1;
}

int removeItem(Tfila *fila){
  if(fila->head != NULL){
    Titem *aux = fila->head;
    int PID = aux->PID;
    fila->head = fila->head->next;
    free(aux);
    fila->tamanho--;
    return PID;
  }
  return -1;
}

void imprime(Tfila fila){
  Titem *aux = fila.head;
  while(aux != NULL){
    printf("{%d,%d}->", aux->PID, aux->tempo);
    aux = aux->next;
  }
  printf("\n");
}

int main(){
  Tfila fila;
  fila.tamanho = 0;
  fila.head = NULL;
  int i,j=0;
  for(i=0;i<20;i+=2)
    insere(++j,rand()%10,&fila);
  imprime(fila);
  printf("remove %d\n",removeItem(&fila));
  printf("remove %d\n",removeItem(&fila));
  imprime(fila);
  return 0;
}
