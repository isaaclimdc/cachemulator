# CACHEMULATOR
# Yuyang Guo (yuyangg) and Isaac Lim (idl)

MD = modules
OD = objs
OBJS = $(OD)/main.o $(OD)/CMComp.o $(OD)/CMProc.o $(OD)/CMCache.o $(OD)/CMSet.o $(OD)/CMLine.o $(OD)/CMAddr.o $(OD)/CMTest.o $(OD)/CMGlobals.o $(OD)/CMConfig.o $(OD)/CMJob.o $(OD)/CMBusShout.o $(OD)/CMBusCtrlr.o $(OD)/CMMemCtrlr.o $(OD)/debug.o

CC = g++
DEBUG = -g
CFLAGS = -m64 -Wall -Werror -O3 -c $(DEBUG)
LFLAGS = -m64 -Wall -Werror -O3 $(DEBUG)

default: cache

dirs:
	mkdir -p $(OD)/

cache: dirs $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@

$(OD)/main.o: main.cpp $(MD)/CMCache.h $(MD)/CMSet.h $(MD)/CMAddr.h $(MD)/CMLine.h $(MD)/CMTest.h $(MD)/CMGlobals.h $(MD)/debug.h
	$(CC) $(CFLAGS) main.cpp -o $@

$(OD)/CMComp.o: $(MD)/CMComp.h $(MD)/CMComp.cpp $(MD)/CMProc.h $(MD)/CMGlobals.h $(MD)/debug.h $(MD)/CMBusCtrlr.h $(MD)/CMMemCtrlr.h
	$(CC) $(CFLAGS) $(MD)/CMComp.cpp -o $@

$(OD)/CMProc.o: $(MD)/CMProc.h $(MD)/CMProc.cpp $(MD)/CMCache.h $(MD)/CMGlobals.h $(MD)/CMJob.h $(MD)/debug.h $(MD)/CMBusShout.h
	$(CC) $(CFLAGS) $(MD)/CMProc.cpp -o $@

$(OD)/CMCache.o: $(MD)/CMCache.h $(MD)/CMCache.cpp $(MD)/CMSet.h $(MD)/CMLine.h $(MD)/CMGlobals.h $(MD)/debug.h
	$(CC) $(CFLAGS) $(MD)/CMCache.cpp -o $@

$(OD)/CMSet.o: $(MD)/CMSet.h $(MD)/CMSet.cpp $(MD)/CMAddr.h $(MD)/CMLine.h $(MD)/CMGlobals.h $(MD)/debug.h
	$(CC) $(CFLAGS) $(MD)/CMSet.cpp -o $@

$(OD)/CMLine.o: $(MD)/CMLine.h $(MD)/CMLine.cpp $(MD)/CMAddr.h $(MD)/CMGlobals.h $(MD)/debug.h
	$(CC) $(CFLAGS) $(MD)/CMLine.cpp -o $@

$(OD)/CMAddr.o: $(MD)/CMAddr.h $(MD)/CMAddr.cpp $(MD)/debug.h $(MD)/CMGlobals.h $(MD)/CMGlobals.h
	$(CC) $(CFLAGS) $(MD)/CMAddr.cpp -o $@

$(OD)/CMTest.o: $(MD)/CMTest.h $(MD)/CMTest.cpp $(MD)/CMAddr.h $(MD)/CMGlobals.h $(MD)/debug.h
	$(CC) $(CFLAGS) $(MD)/CMTest.cpp -o $@

$(OD)/CMGlobals.o: $(MD)/CMGlobals.h $(MD)/CMGlobals.cpp $(MD)/debug.h
	$(CC) $(CFLAGS) $(MD)/CMGlobals.cpp -o $@

$(OD)/CMConfig.o: $(MD)/CMConfig.h $(MD)/CMConfig.cpp $(MD)/debug.h
	$(CC) $(CFLAGS) $(MD)/CMConfig.cpp -o $@

$(OD)/CMJob.o: $(MD)/CMJob.h $(MD)/CMJob.cpp $(MD)/CMGlobals.h $(MD)/debug.h
	$(CC) $(CFLAGS) $(MD)/CMJob.cpp -o $@

$(OD)/CMBusShout.o: $(MD)/CMBusShout.h $(MD)/CMBusShout.cpp $(MD)/CMGlobals.h $(MD)/debug.h
	$(CC) $(CFLAGS) $(MD)/CMBusShout.cpp -o $@

$(OD)/CMBusCtrlr.o: $(MD)/CMBusCtrlr.h $(MD)/CMBusCtrlr.cpp $(MD)/CMGlobals.h $(MD)/debug.h
	$(CC) $(CFLAGS) $(MD)/CMBusCtrlr.cpp -o $@

$(OD)/debug.o: $(MD)/debug.h $(MD)/debug.cpp
	$(CC) $(CFLAGS) $(MD)/debug.cpp -o $@

$(OD)/CMMemCtrlr.o: $(MD)/CMMemCtrlr.h $(MD)/CMMemCtrlr.cpp $(MD)/CMJob.h $(MD)/debug.h $(MD)/CMGlobals.h
	$(CC) $(CFLAGS) $(MD)/CMMemCtrlr.cpp -o $@

clean:
	rm -rf $(OD) *.o *.dSYM
	rm -f cache
