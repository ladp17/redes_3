#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ConexaoRawSocket.h"

typedef struct envelope{
    long unsigned int inicio;
//    destino;
//	  origem;
    unsigned short tam;
    char *data;
//    crc;
} envelope;

int inicia(){
	char i;
	printf("digite \"m\" para mestre ou \"e\" para escravo\n");
	while(1){
		i = getchar();
		printf("i: %c\n",i);
		if (i == 'm'){
			return 1;
		}
		if (i == 'e'){
			return 0;
		}
		printf("input invalido\n");
		printf("digite \"m\" para mestre ou \"e\" para escravo\n");
	}
}
void le_entrada(char msg[1501]){
    int aux, aux1;
    aux = 1;
    while(aux){
        printf("Digite uma mensagem de 46 ate 1500 caracteres\n");
        fgets(msg, 1501, stdin);
        aux1=0;

        if (45<strlen(msg) && strlen(msg)<1501){
            aux = 0;
        }
        if (strlen(msg)<46 && aux1==0){
            printf ("mensagem muito curta, tamanho: %d\n", strlen(msg));
            aux1=1;
        }
        if (strlen(msg)>1500 && aux1==0){
            printf ("mensagem muito longa\n");
            aux1=1;
        }
    }
}

void envelopa(char msg[1501], envelope *e){
    e->inicio = 6148914691236517000;
    e->tam = (strlen(msg)+1)*(sizeof(char));
    e->data = (char*) malloc(strlen(msg)* sizeof(char));
    strcpy(e->data, msg);
}

void envia(envelope e){
    envelope s;
    s.data = (char*) malloc(1500* sizeof(char));
    printf("data: %s, tam: %d\n", e.data, e.tam);
    printf("abrindo socket\n");
    int sd_socket = ConexaoRawSocket("lo"); //nome do socket (lo eh socket local).
    printf("enviando\n");
    write(sd_socket, &e, sizeof(e));
    printf("enviado\n");
    e.tam = 1;
/*    read(sd_socket, &s, sizeof(e));
    printf("lendo\n");
    printf("tams: %d, %d\n", sizeof(s), s.tam);
    printf("%s\n", s.data);
    printf("lido\n");*/
}

void mestre(){
	envelope e;
	char msg[1500];
	le_entrada(msg);
	printf("lido\n");
	envelopa(msg, &e);
	printf("envelopado\n");
	envia(e);
}

void escravo(){
	char *buffer = (char *) malloc(40960*sizeof(char));
	int rec_socket = ConexaoRawSocket("lo");

	printf("esperando...\n");

	while(1){
		read(rec_socket, buffer, 40960);
	
	}
	printf("recebido\n");
    printf("%s\n", buffer);
	printf("lido\n");
}

int main(){
    if (inicia()){
	    mestre();
	}
	escravo();
	printf("concluido\n");
}