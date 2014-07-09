all : nosigtstp.so
CC=clang
%.so : %.c
	$(CC) -Wall -fPIC -ldl -shared $< -o $@

clean :
	rm -f *.o *.so a.out
