#include <stdio.h>
#include <rpc/rpc.h>
#include "tempo.h"
int main(int argc, char **argv){
    CLIENT *c;
    char *servidor;
    if (argc != 2){
        printf ("uso: %s <ip_servidor>", argv[0]);
        return 0;
    }
    servidor = argv[1];
    c = clnt_create(servidor, TEMPOPROG, TEMPOVERS, "tcp");
    if (!c){
        clnt_perror(c, servidor);
        return 0;
    }
    timeval_ *result = malloc(sizeof(timeval_));
    tempo_1(NULL, result, c);
    if (!result){
        clnt_perror(c, servidor);
        return 0;
    }
    printf("tempo segundos: %ld, microssegundos: %ld\n", result->tv_sec, result->tv_usec);
    return 0;
}
