# CACHEMULATOR
# Yuyang Guo (yuyangg) and Isaac Lim (idl)

MODDIR = modules
OBJDIR = objs
OBJS = $(OBJDIR)/main.o $(OBJDIR)/CMCache.o $(OBJDIR)/CMSet.o $(OBJDIR)/CMLine.o $(OBJDIR)/CMAddr.o $(OBJDIR)/CMTest.o $(OBJDIR)/debug.o

CC = g++
DEBUG = -g
CFLAGS = -m64 -Wall -Werror -O3 -c $(DEBUG)
LFLAGS = -m64 -Wall -Werror -O3 $(DEBUG)

default: cache

dirs:
	mkdir -p $(OBJDIR)/

cache: dirs $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@

$(OBJDIR)/main.o: main.cpp CMCache.h $(MODDIR)/CMSet.h $(MODDIR)/CMAddr.h $(MODDIR)/CMLine.h $(MODDIR)/CMTest.h $(MODDIR)/debug.h
	$(CC) $(CFLAGS) main.cpp -o $@

$(OBJDIR)/CMCache.o: CMCache.h CMCache.cpp $(MODDIR)/CMSet.h $(MODDIR)/CMLine.h $(MODDIR)/debug.h
	$(CC) $(CFLAGS) CMCache.cpp -o $@

$(OBJDIR)/CMSet.o: $(MODDIR)/CMSet.h $(MODDIR)/CMSet.cpp $(MODDIR)/CMAddr.h $(MODDIR)/CMLine.h $(MODDIR)/debug.h
	$(CC) $(CFLAGS) $(MODDIR)/CMSet.cpp -o $@

$(OBJDIR)/CMLine.o: $(MODDIR)/CMLine.h $(MODDIR)/CMLine.cpp $(MODDIR)/CMAddr.h $(MODDIR)/debug.h
	$(CC) $(CFLAGS) $(MODDIR)/CMLine.cpp -o $@

$(OBJDIR)/CMAddr.o: $(MODDIR)/CMAddr.h $(MODDIR)/CMAddr.cpp $(MODDIR)/debug.h
	$(CC) $(CFLAGS) $(MODDIR)/CMAddr.cpp -o $@

$(OBJDIR)/CMTest.o: $(MODDIR)/CMTest.h $(MODDIR)/CMTest.cpp $(MODDIR)/CMAddr.h $(MODDIR)/debug.h
	$(CC) $(CFLAGS) $(MODDIR)/CMTest.cpp -o $@

$(OBJDIR)/debug.o:
	$(CC) $(CFLAGS) $(MODDIR)/debug.cpp -o $@

clean:
	rm -rf $(OBJDIR) *.o *.dSYM
	rm -f cache
