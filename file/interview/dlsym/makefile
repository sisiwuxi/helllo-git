#####################################################
# Author : sisi.wu                                  #
# Date   : 2017/8/30                                #
#####################################################
GCC := gcc
OBJECTS := libcaculate.so
TARGET := main
all : 
	$(GCC) -fPIC -shared caculate.c -o $(OBJECTS)
	$(GCC) -rdynamic -o $(TARGET) main.c -ldl
.PHONY:clean
clean: 
	rm -rf $(OBJECTS) $(TARGET) 
 
