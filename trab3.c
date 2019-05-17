#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "ConexaoRawSocket.h"

typedef struct envelope {
    uint64_t inicio;
    uint8_t destino[6];
    uint8_t origem[6];
    uint16_t tam;
    uint8_t data[]; // o crc vai aqui no final
    // unsigned int crc;
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
	getchar(); // pula \n
}
void le_entrada(char* msg){
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
            printf ("mensagem muito curta, tamanho: %lu\n", strlen(msg));
            aux1=1;
        }
        if (strlen(msg)>1500 && aux1==0){
            printf ("mensagem muito longa\n");
            aux1=1;
        }
    }
}

uint32_t calcula_crc(envelope* e){
    const char *buf = (const char*) e + (8);
    size_t len = e->tam + (2) +(6) + (6); // quantidade de bytes
    uint32_t crc = 0; // precisa para função
    
	static uint32_t table[256];
	static int have_table = 0;
	uint32_t rem;
	uint8_t octet;
	int i, j;
	const char *p, *q;
 
	/* This check is not thread safe; there is no mutex. */
	if (have_table == 0) {
		/* Calculate CRC table. */
		for (i = 0; i < 256; i++) {
			rem = i;  /* remainder from polynomial division */
			for (j = 0; j < 8; j++) {
				if (rem & 1) {
					rem >>= 1;
					rem ^= 0xedb88320;
				} else
					rem >>= 1;
			}
			table[i] = rem;
		}
		have_table = 1;
	}
 
	crc = ~crc;
	q = buf + len;
	for (p = buf; p < q; p++) {
		octet = *p;  /* Cast to unsigned octet. */
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
	}
	return ~crc;
}

void envelopa(char* msg, envelope *e){
    uint32_t crc;
    e->inicio = 0x5555555555555488;
    uint8_t destino[6] =  { 0x6b };
    memcpy(e->destino, destino, 6);
    uint8_t origem[6] = { 0x00 };
    memcpy(e->origem, origem, 6);
    
    e->tam = (strlen(msg)+1)*(sizeof(uint8_t));
    strcpy((char*) e->data, msg);
    crc = calcula_crc(e);
    memcpy(&e->data[strlen(msg)], &crc, 4);
}

void envia(int sd_socket, envelope* e){
    printf("67 data: %s, tam: %d\n", e->data, e->tam);
    printf("68 abrindo socket\n");
    printf("70 enviando\n");
    write(sd_socket, e, e->tam + sizeof(envelope));
    printf("72 enviado\n");
}

void mestre(int rec_socket){
	envelope* e = (envelope*) malloc(sizeof(envelope) + 1500* sizeof(uint8_t) + 4);
	char* msg = malloc(sizeof(char) * 1500);
	le_entrada(msg);
	printf("84 lido\n");
	envelopa(msg, e);
	printf("86 envelopado\n");
	envia(rec_socket, e);
}

void escravo(int rec_socket){
	uint8_t *buffer = (uint8_t *) malloc(40960*sizeof(uint8_t));

	printf("94 esperando...\n");

	while(1){
		int bytes = read(rec_socket, buffer, 40960);
	    printf("98 recebido %d bytes\n", bytes);
        printf("%s\n", buffer);
        
        printf("%s\n", buffer+sizeof(envelope)); 
        // idealmente é só isso:
        // printf("%s\n", buffer); 
	    printf("100 lido\n");
	}
}

int main(){
    int sd_socket = ConexaoRawSocket("lo"); //nome do socket (lo eh socket local).
    if (inicia()){
	    mestre(sd_socket);
	    printf("108 escravo\n");
	    escravo(sd_socket);
	    printf("111 concluido\n");
	    return 0;
	}
	escravo(sd_socket);
	mestre(sd_socket);
	printf("concluido\n");
	return 0;
}