ROOT=..
TARGET = trackuino
SRC = $(wildcard *.c)
CXXSRC = $(wildcard *.cpp)
LIBS = ../build/libarduino.a

all: build

../build/libarduino.a:
	(cd ../lib; make)

include $(ROOT)/Makefile.inc

