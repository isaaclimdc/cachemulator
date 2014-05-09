# CACHEMULATOR
# Yuyang Guo (yuyangg) and Isaac Lim (idl)

OBJS = pthreads

CC = g++
DEBUG = -g
CFLAGS = -m64 -O0 -c $(DEBUG)
LFLAGS = -m64 -O0 $(DEBUG)

pthreads:
	$(CC) $(LFLAGS) -oterm -lpthread pthreads.cpp -o $@

clean:
	rm -f $(OBJS)
