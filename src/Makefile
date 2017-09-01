GCC=g++
CC=gcc

INCLUDEDIR= -I../lib/include64/ -I../../3rd-srv/include/

LIBDIR= -L../lib/lib64/ -lcrypto -ltinyxml2 -lfcgi -lpthread

CPPFLAGS = -g -W -Wall

SOURCES = $(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SOURCES) )

all: clean wx-srv install

clean:
	rm -f *.o
	rm -f ../bin/wx-srv

wx-srv : thpool.o $(OBJS)
	$(GCC) -o $@ $^ $(LIBDIR) $(INCLUDEDIR)

%.o : %.c
	$(CC) -lpthread $< -c -g

%.o	: %.cpp
	$(GCC) $(LIBDIR) $(CPPFLAGS) -c $< -o $@ $(INCLUDEDIR) -g

install:
	mv wx-srv ../bin
	../bin/restart.sh
	../bin/watch-log.sh
