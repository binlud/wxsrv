GCC=g++

INCLUDEDIR= -I../lib/include64/ -I../../3rd-srv/include/

LIBDIR= -L../lib/lib64/ -l crypto -l tinyxml2 -lfcgi

CPPFLAGS = -g -W -Wall

SOURCES = $(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SOURCES) )

all: clean wx-srv install

clean:
	rm -f *.o
	rm -f ../bin/wx-srv

wx-srv : $(OBJS)
	$(GCC) -o $@ $^ $(LIBDIR) $(INCLUDEDIR)

%.o	: %.cpp
	$(GCC) $(LIBDIR) $(CPPFLAGS) -c $< -o $@ $(INCLUDEDIR) -g

install:
	mv wx-srv ../bin
	../bin/restart.sh
	../bin/watch-log.sh
