# CACHEMULATOR
# Yuyang Guo (yuyangg) and Isaac Lim (idl)

CXX = g++ -m64
CXXFLAGS = -g -Wall -Werror -O3

default: cache

cache:
	$(CXX) $(CXXFLAGS) -o $@ cache.cpp
	rm -rf *.dSYM

run: default
	./cache

# Clean the src dirctory
clean:
	rm -rf *.o *.dSYM
	rm -f cache
