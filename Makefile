# requires libudev-dev

.SUFFIXES:      .cpp .o .a .s

CC     := $(CROSS_COMPILE)gcc
CXX    := $(CROSS_COMPILE)g++
LD     := $(CROSS_COMPILE)g++
AR     := $(CROSS_COMPILE)ar rc
RANLIB := $(CROSS_COMPILE)ranlib

DEBUG_CFLAGS    := -Wall -Wno-format -g -DDEBUG
RELEASE_CFLAGS  := -Wall -Wno-unknown-pragmas -Wno-format -O3
DEBUG_LDFLAGS   := -g

# Change for DEBUG or RELEASE
CFLAGS  := -c $(DEBUG_CFLAGS) -std=gnu++11
LDFLAGS := $(DEBUG_LDFLAGS)

#Configurate the location of the openzwave headerfiles here
#INCLUDES        := -I ../../../src -I ../../../src/command_classes/ -I ../../../src/value_classes/ \
#        -I ../../../src/platform/ -I ../../../h/platform/unix -I ../../../tinyxml/ -I ../../../hidapi/hidapi/   
INCLUDES        := -I /usr/include/openzwave/command_classes/ -I /usr/include/openzwave/value_classes/ -I /usr/include/openzwave/ -I /usr/include/openzwave/platform/
#Configurate the location of libopenzwave.so here
LIBS = $(wildcard /usr/lib/libopenzwave.so)

%.o : %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -o $@ $<

all: ZServer

ZServer:	Main.o
	$(LD) -o $@ $(LDFLAGS) Main.o $(LIBS) -ldl -lm 
	
clean:
	rm -f ZServer *.o
