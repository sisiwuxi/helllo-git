#####################################################
# Author : sisi.wu                                  #
# Date   : 2015/5/10                                #
# Email  : sisi.wu@mstarsemi.com                    #
#####################################################

#ARCH := arm-none-linux-gnueabi-
ARCH := 

GCC := $(ARCH)gcc

STRIP := $(ARCH)strip

SRC := rsa1024.c

INC := $(shell pwd)

TARGET := rsa1024

VER := $(shell date)

LIB := -lcrypto -lssl

all:
	$(GCC) -I $(INC) $(SRC) -o $(TARGET) $(LIB)
	$(STRIP) $(TARGET)
