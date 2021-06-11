kcoloring: kcoloring.c
	gcc -Wall -g -o kcoloring kcoloring.c

clean:
	rm kcoloring

run:
	./kcoloring
