CC=gcc
CFLAGS=-Wall -Wextra -O3 -funsafe-math-optimizations -fno-trapping-math -fno-signed-zeros -fno-math-errno
DEPS=
OBJ=main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

kangala: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o kangala

