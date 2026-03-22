CC = gcc -Wall
TARGET = main

SRCS = main.c filename.c pictures.c pixels.c lut.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

filename.o: filename.h

lut.o: lut.h pictures.h

pixels.o: pixels.h pictures.h

main.o: pictures.h filename.h 

pictures.o: lut.h pixels.h filename.h pictures.h 

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
