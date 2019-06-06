CFLAGS =-Wall \
		-Wextra \
		-g
		

all: conexaorawsocket

run: conexaorawsocket
	./conexaorawsocket

conexaorawsocket: ConexaoRawSocket.c trab3.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean:
	rm -rf conexaorawsocket
