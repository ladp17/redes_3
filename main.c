#include "ConexaoRawSocket.h"
#include <stdio.h>
#include <stdlib.h>

#define T 15
#define TAM T+1

int main(){
	char s[T];
	printf("cp1\n");
	int sd_socket = ConexaoRawSocket("lo"); //nome do socket (lo eh socket local).
	printf("cp2\n");
	write(sd_socket, "testetesteteste", TAM);
	printf("cp3\n");
	read(sd_socket, s, TAM);
	printf("cp4\n");
	printf("%s\n", s);
	printf("cp5\n");
	return 0;
}