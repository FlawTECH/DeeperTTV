TARGET = deeperttv
LIBS = 
CC = gcc
CFLAGS = -g -Wall
BIN = bin

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard src/*.c))
HEADERS = $(wildcard src/lib/*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS) | $(BIN)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o bin/$@

$(BIN):
	mkdir $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)