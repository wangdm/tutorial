

all: server client
	
	
server: server.c
	gcc server.c -o server -lpthread
	
client: client.c
	gcc -g client.c -o client
	
clean:
	rm client
	rm server