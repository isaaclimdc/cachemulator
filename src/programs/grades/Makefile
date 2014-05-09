# CACHEMULATOR
# Yuyang Guo (yuyangg) and Isaac Lim (idl)

OBJS = grades

CC = g++
DEBUG = -g
CFLAGS = -m64 -O0 -c $(DEBUG)
LFLAGS = -m64 -O0 $(DEBUG)

grades:
	$(CC) $(LFLAGS) -oterm -lpthread grades.cpp -o $@

clean:
	rm -f $(OBJS)
