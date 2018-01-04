CROSS=
#CROSS=/gcc-4.2/arm-linux-  	
CC := $(CROSS)gcc
LDFLAGS = -L. -Wl,-rpath,./
LIBS = -ltest -ladd -ldl

OBJS   = main.o
DYNLIB = libadd.so libtest.so
TARGET = test

all : $(DYNLIB) $(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

$(OBJS):main.c
	$(CC) -c $^ $<

$(DYNLIB):
	$(CC) -fPIC -shared add.c -o libadd.so
	$(CC) -fPIC -shared test.c -o libtest.so 

clean:
	rm -f *.o
	rm -f *.so
	rm -f $(TARGET)
