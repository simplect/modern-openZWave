# Modified to work with ModernOZW

# SOFTWARE NOTICE AND LICENSE

#This file is part of OpenZWave.

#OpenZWave is free software: you can redistribute it and/or modify
#it under the terms of the GNU Lesser General Public License as published
#by the Free Software Foundation, either version 3 of the License,
#or (at your option) any later version.

#OpenZWave is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU Lesser General Public License for more details.

#You should have received a copy of the GNU Lesser General Public License
#along with OpenZWave.  If not, see <http://www.gnu.org/licenses/>.


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

all: ModernOZW

ModernOZW:	Main.o
	$(LD) -o $@ $(LDFLAGS) Main.o $(LIBS) -ldl -lm 
	
clean:
	rm -f ModernOZW *.o
