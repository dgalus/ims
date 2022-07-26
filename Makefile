CC = gcc

all:
	rm -rf bin
	mkdir bin
	${CC} src/*.c -o bin/ims -lcurl -lconfig

clean:
	rm -rf bin