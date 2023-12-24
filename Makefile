httest: main.o hash.o
	gcc -o $@ $^

%.o: %.c
	gcc -c -o $@ $<
