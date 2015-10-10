#
#	Makefile de SimpleSock
#

# CFLAGS_RELEASE := -O2
# CFLAGS_DEBUG := -ggdb3 -O0 -g -Wall -Wextra -std=c++11 -fsanitize=address
CFLAGS=  -O2 -DLINUX -Isrc
LDFLAGS=-lm

VPATH = src:test:examples
OBJS_test = SimpleSock.o main.o
OBJS_example1  = SimpleSock.o example1.o
OBJS_example2  = SimpleSock.o example2.o

all:	    unittest example1 example2

unittest: $(OBJS_test)
	        g++ -o $@ $(OBJS_test) $(CFLAGS) $(LDFLAGS)
example1: $(OBJS_example1)
	        g++ -o $@ $(OBJS_example1) $(CFLAGS) $(LDFLAGS)
example2: $(OBJS_example2)
	        g++ -o $@ $(OBJS_example2) $(CFLAGS) $(LDFLAGS)

clean:
	    rm -f test example1 example2 $(OBJS_test)

.cpp.o:
	    g++ $(CFLAGS) -c $<