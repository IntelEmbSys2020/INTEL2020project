# worker: myThread.cpp
# 	g++ Demo/samples/xtts_offline_sample/xtts_offline_sample.c  /
# myThread.cpp cameraGetImage/cameraGetImageSource.cpp / 
#  main.cpp /
#  ../P2P/p2p.cpp  -o mainpro -I ./include -I ./Demo/include / 
# -lMvCameraControl -L ./64 -lpthread -std=c++11 -L /usr/lib/x64  /
# -g -Wall -I ./Demo/include

#common makefile header

DIR_INC = ./Demo/include
DIR_BIN = ./Demo/bin
DIR_LIB = ./Demo/libs
# DIR_LIB = /usr/lib 

TARGET	= xtts_offline_sample
BIN_TARGET = $(DIR_BIN)/$(TARGET)  

CROSS_COMPILE = 
CFLAGS = -g -Wall -I$(DIR_INC)

LDFLAGS := -L$(DIR_LIB)/x64
LDFLAGS += -L./64
LDFLAGS += -lmsc -lrt -ldl -lpthread -lstdc++ -lMvCameraControl

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))
OBJECTS += myThread.cpp ./Demo/samples/xtts_offline_sample/xtts_offline_sample.c cameraGetImage/cameraGetImageSource.cpp ../P2P/p2p.cpp main.cpp 

$(BIN_TARGET) : $(OBJECTS)
	$(CROSS_COMPILE)g++ $(CFLAGS) $^ -o $@ $(LDFLAGS) -std=c++11 -I ./include

%.o : %.c
	$(CROSS_COMPILE)g++ -c $(CFLAGS) $< -o $@
clean:
	@rm -f *.o $(BIN_TARGET)

.PHONY:clean

#common makefile foot
