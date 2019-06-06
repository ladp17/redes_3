CFLAGS =-Wall \
		-Wextra \
		-g
		
all: conexaorawsocket

conexaorawsocket: ConexaoRawSocket.c trab3.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean:
	rm -rf conexaorawsocket