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
INCLUDES        := -I jsoncpp/ -I ../open-zwave/cpp/src/command_classes/ -I ../open-zwave/cpp/src/value_classes/ -I ../open-zwave/cpp/src/ -I ../open-zwave/cpp/src/platform/
#Configurate the location of libopenzwave.so here
LIBS = ../open-zwave/libopenzwave.so

%.o : %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -o $@ $<

all: ModernOZW

ModernOZW:	Main.o jsoncpp/jsoncpp.o
	$(LD) -o $@ $(LDFLAGS) Main.o jsoncpp/jsoncpp.o $(LIBS) -ldl -lm -lzmq
	
clean:
	rm -f ModernOZW *.o
