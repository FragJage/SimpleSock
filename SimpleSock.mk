#
#	Makefile de SimpleSock
#

# CFLAGS_RELEASE := -O2 -s -std=c++11
# CFLAGS_RELEASE GCC < 4.7 := -O2 -s -std=c++0x
# CFLAGS_DEBUG := -O0 -std=c++11 -g -ggdb3 -fsanitize=address -Wall -Wextra -Wzero-as-null-pointer-constant -pedantic -pedantic-errors -Wmain -Weffc++ -Wswitch-default -Wswitch-enum -Wmissing-declarations -Wunreachable-code -Wfloat-equal -Wundef -Wcast-align -Wredundant-decls -Winit-self -Wshadow -Wnon-virtual-dtor 
# CFLAGS_DEBUG GCC < 4.7 := -O0 -std=c++11 -g -ggdb3 -Wall -Wextra -pedantic -pedantic-errors -Wmain -Weffc++ -Wswitch-default -Wswitch-enum -Wmissing-declarations -Wunreachable-code -Wfloat-equal -Wundef -Wcast-align -Wredundant-decls -Winit-self -Wshadow -Wnon-virtual-dtor
CFLAGS= -DLINUX -Isrc -O0 -std=c++11 -g -ggdb3 -fsanitize=address -Wall -Wextra -Wzero-as-null-pointer-constant -pedantic -pedantic-errors -Wmain -Weffc++ -Wswitch-default -Wswitch-enum -Wmissing-declarations -Wunreachable-code -Wfloat-equal -Wundef -Wcast-align -Wredundant-decls -Winit-self -Wshadow -Wnon-virtual-dtor  
LDFLAGS=-lm

VPATH = src:test:examples
OBJS_test = SimpleSock.o SafeFunctions.o main.o
OBJS_example1  = SimpleSock.o SafeFunctions.o example1.o
OBJS_example2  = SimpleSock.o SafeFunctions.o example2.o

all:	    unittest example1 example2

unittest: $(OBJS_test)
	        g++ -o $@ $(OBJS_test) $(CFLAGS) $(LDFLAGS)
example1: $(OBJS_example1)
	        g++ -o $@ $(OBJS_example1) $(CFLAGS) $(LDFLAGS)
example2: $(OBJS_example2)
	        g++ -o $@ $(OBJS_example2) $(CFLAGS) $(LDFLAGS)

clean:
	    rm -f unittest example1 example2 $(OBJS_test) $(OBJS_example1) $(OBJS_example2)

.cpp.o:
	    g++ $(CFLAGS) -c $<