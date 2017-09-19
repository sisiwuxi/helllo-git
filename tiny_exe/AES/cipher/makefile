#####################################################
# Author : sisi.wu                                  #
# Date   : 2017/9/12                             #
#####################################################
ARCH := 

GCC := $(ARCH)gcc

STRIP := $(ARCH)strip

SRC := TestCipher.c

INC := $(shell pwd)

TARGET := TestCipher

VER := $(shell date)

LIB := -lcrypto -lssl

all:
	$(GCC) -I $(INC) $(SRC) -o $(TARGET) $(LIB)
	$(STRIP) $(TARGET)
.PHONY:clean
clean: 
	rm -rf $(OBJECTS) $(TARGET) 