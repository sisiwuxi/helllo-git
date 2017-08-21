#####################################################
# Author : sisi.wu                                  #
# Date   : 2017/7/12                                #
#####################################################

#ARCH := arm-none-linux-gnueabi-
ARCH := 
GCC := gcc
CFLAGS:=-c -Wall -O -g
STRIP := strip
SRC := yuvtool.c common.c
INC := $(shell pwd)/include
TARGET := yuvtool
VER := $(shell date)

version:
	@rm ./include/version.h
	@echo "/************************************************************/" > ./include/version.h
	@echo "/* Author : sisi.wu                                         */" >> ./include/version.h
	@echo "/* Date   : 2017/7/12                                       */" >> ./include/version.h
	@echo "/* Email   : sisiwuxi@hotmail.com                           */" >> ./include/version.h
	@echo "/************************************************************/" >> ./include/version.h
	@echo "#ifndef _VERSION_H_" >> ./include/version.h
	@echo "#define _VERSION_H_" >> ./include/version.h
	@echo char version[] = '"'$(VER)'"'';' >> ./include/version.h
	@echo "#endif" >> ./include/version.h

OBJECTS:=yuvtool.o common.o

all : $(OBJECTS)
	$(GCC) -o $(TARGET) $(OBJECTS)

$(OBJECTS):%.o:%.c
	$(GCC) $(CFLAGS) $< -o $@

.PHONY:clean
clean: 
	rm -rf $(OBJECTS) $(TARGET) 




