SRC:=${wildcard *.c}
OBJECTS:=${patsubst %.c, %.o, $(SRC)}
SHARED_LIBRIARIES:=libtest.so
STATIC_LIBRIARIES:=libtest.a
EXECUTE_STATIC:=test_static
EXECUTE_SHARED:=test_shared
CFLAGS_STATIC:=-Wall -g
CFLAGS_SHARED:=-fPIC -shared -o
CC:=gcc
AR:=ar

all:$(OBJECTS) $(STATIC_LIBRIARIES) $(SHARED_LIBRIARIES) $(EXECUTE_STATIC) $(EXECUTE_SHARED) 

$(OBJECTS):%.o:%.c
	$(CC) $(CFLAGS_STATIC) -c $^ -o $@
$(STATIC_LIBRIARIES):
	$(AR) -r $@ *.o
$(SHARED_LIBRIARIES):
	$(CC) *.c $(CFLAGS_SHARED) $@ 
$(EXECUTE_STATIC):
	$(CC) -o $@ test.c $(STATIC_LIBRIARIES)
$(EXECUTE_SHARED):
	$(CC) test.c -L. -ltest -o $@ 

.PHONY:clean
clean : 
	rm $(OBJECTS) $(STATIC_LIBRIARIES) $(SHARED_LIBRIARIES) $(EXECUTE)  
