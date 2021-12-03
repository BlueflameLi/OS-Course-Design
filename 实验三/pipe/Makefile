all: pipe read write
pipe :
	gcc pipe.c -o pipe -pthread
read :
	gcc read.c -o read -pthread
newfifo :
	gcc write.c -o write -pthread
clean :
	rm pipe read write
