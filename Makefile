PORT ?= /dev/cu.usbmodem1101 #/dev/cu.HC-05  #/dev/cu.usbmodem11202 #/dev/ttyACM0
MAKEFILE_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

OS := $(shell uname)
MAC_LIBRARIES = /Users/$(USER)/Library/Arduino15/libraries,/Users/$(USER)/Documents/Arduino/libraries
JETSON_LIBRARIES = /home/$(USER)/Documents/arduino-1.8.19-linuxaarch64/arduino-1.8.19/libraries,/home/$(USER)/Arduino/libraries 

ifeq ($(OS),Darwin)
	LIBRARIES = $(MAC_LIBRARIES)
else ifeq ($(OS),Linux)
	LIBRARIES = $(JETSON_LIBRARIES)
endif

compile:
	arduino-cli compile ${MAKEFILE_DIR} --fqbn=arduino:megaavr:uno2018 --libraries=${LIBRARIES}

# Upload the compiled code to the board.
# Args:
#   port: The serial port where the board is connected (e.g., /dev/ttyACM0, /dev/cu.usbmodem1102, etc).
# Example:
#   $make upload PORT=/dev/cu.usbmodem1102
upload: compile
	arduino-cli upload ${MAKEFILE_DIR} --port=${PORT} --fqbn=arduino:megaavr:uno2018
