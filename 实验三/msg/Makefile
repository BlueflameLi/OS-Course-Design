all: sender1 sender2 receive
sender1: 
	gcc sender1.c -pthread -o sender1 
sender2: 
	gcc sender2.c -pthread -o sender2
receive:
	gcc receive.c -pthread -o receive
clean:
	rm sender1 sender2 receive