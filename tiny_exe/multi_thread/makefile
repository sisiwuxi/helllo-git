OBJECTS:=multiple_thread.o
CFLAGS:=-c -Wall -O -g
CC:=gcc
all:$(OBJECTS)
	$(CC) $(OBJECTS) -lpthread

$(OBJECTS):%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	rm -rf $(OBJECTS)

