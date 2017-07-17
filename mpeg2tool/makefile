#####################################################
# Author : sisi.wu                                  #
# Date   : 2017/7/12                                #
# Email  : sisi.wu@mstarsemi.com                    #
#####################################################

#ARCH := arm-none-linux-gnueabi-
ARCH := 
GCC := gcc
CFLAGS:=-c -Wall -O -g
STRIP := strip
SRC := mpeg2tool.c common.c abend.c parseTS.c
INC := $(shell pwd)/include
TARGET := mpeg2tool
VER := $(shell date)

version:
	@rm ./include/version.h
	@echo "/************************************************************/" > version.h
	@echo "/* Author : sisi.wu                                         */" >> version.h
	@echo "/* Date   : 2017/7/12                                       */" >> version.h
	@echo "/* Email  : sisi.wu@mstarsemi.com                           */" >> version.h
	@echo "/************************************************************/" >> version.h
	@echo "#ifndef _VERSION_H_" >> version.h
	@echo "#define _VERSION_H_" >> version.h
	@echo char version[] = '"'$(VER)'"'';' >> version.h
	@echo "#endif" >> version.h

OBJECTS:=mpeg2tool.o common.o abend.o parseTS.o

all : $(OBJECTS)
	$(GCC) -o $(TARGET) $(OBJECTS)

$(OBJECTS):%.o:%.c
	$(GCC) $(CFLAGS) $< -o $@

.PHONY:clean
clean: 
	rm -rf $(OBJECTS) $(TARGET) 




