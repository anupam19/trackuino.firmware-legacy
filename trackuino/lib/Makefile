ROOT=..
TARGET = libarduino
SRC    = $(wildcard Core/*.c) \
				 $(wildcard Ethernet/utility/*.c) \
				 $(wildcard Wire/utility/*.c)
CXXSRC = $(wildcard Core/*.cpp) \
				 $(wildcard EEPROM/*.cpp) \
				 $(wildcard Ethernet/*.cpp) \
				 $(wildcard Firmata/*.cpp) \
				 $(wildcard LiquidCrystal/*.cpp) \
				 $(wildcard Matrix/*.cpp) \
				 $(wildcard Servo/*.cpp) \
				 $(wildcard SoftwareSerial/*.cpp) \
				 $(wildcard Sprite/*.cpp) \
				 $(wildcard Stepper/*.cpp) \
				 $(wildcard Wire/*.cpp)

all: a

include $(ROOT)/Makefile.inc

