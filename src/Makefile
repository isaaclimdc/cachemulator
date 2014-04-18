# CACHEMULATOR
# Yuyang Guo (yuyangg) and Isaac Lim (idl)

OBJS = main.o CMCache.o CMSet.o CMLine.o CMAddr.o CMTest.o debug.o
CC = g++
DEBUG = -g
CFLAGS = -m64 -Wall -Werror -O3 -c $(DEBUG)
LFLAGS = -m64 -Wall -Werror -O3 $(DEBUG)
MODDIR = modules

cache: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@
	rm *.o

main.o: main.cpp CMCache.h $(MODDIR)/CMSet.h $(MODDIR)/CMAddr.h $(MODDIR)/CMLine.h $(MODDIR)/CMTest.h $(MODDIR)/debug.h
	$(CC) $(CFLAGS) main.cpp

CMCache.o: CMCache.h CMCache.cpp $(MODDIR)/CMSet.h $(MODDIR)/CMLine.h $(MODDIR)/debug.h
	$(CC) $(CFLAGS) CMCache.cpp

CMSet.o: $(MODDIR)/CMSet.h $(MODDIR)/CMSet.cpp $(MODDIR)/CMAddr.h $(MODDIR)/CMLine.h $(MODDIR)/debug.h
	$(CC) $(CFLAGS) $(MODDIR)/CMSet.cpp

CMLine.o: $(MODDIR)/CMLine.h $(MODDIR)/CMLine.cpp $(MODDIR)/CMAddr.h $(MODDIR)/debug.h
	$(CC) $(CFLAGS) $(MODDIR)/CMLine.cpp

CMAddr.o: $(MODDIR)/CMAddr.h $(MODDIR)/CMAddr.cpp $(MODDIR)/debug.h
	$(CC) $(CFLAGS) $(MODDIR)/CMAddr.cpp

CMTest.o: $(MODDIR)/CMTest.h $(MODDIR)/CMTest.cpp $(MODDIR)/CMAddr.h $(MODDIR)/debug.h
	$(CC) $(CFLAGS) $(MODDIR)/CMTest.cpp

debug.o:
	$(CC) $(CFLAGS) $(MODDIR)/debug.cpp

clean:
	rm -rf *.o *.dSYM
	rm -f cache
