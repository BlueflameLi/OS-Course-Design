all: sender receive
sender: sender.c
	gcc sender.c -o sender -pthread
receive: receive.c
	gcc receive.c -o receive -pthread
clean:
	rm sender receive
