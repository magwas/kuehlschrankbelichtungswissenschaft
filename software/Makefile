# Arduino Makefile for Arduino Micro
BOARD_TAG    = micro
MONITOR_PORT = /dev/ttyACM0
ARDUINO_LIBS = Wire MCP7940 OneWire DallasTemperature

# Arduino directory (adjust if needed)
ARDUINO_DIR = /usr/share/arduino

# Source files
TARGET = main
SRC = src/$(TARGET).cc



# Include Arduino.mk
include /usr/share/arduino/Arduino.mk
all: tests $(TARGET_EEP) $(TARGET_HEX)

tests:
	make -C test

clean::
	make -C test clean

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build the project"
	@echo "  upload  - Build and upload"
	@echo "  monitor - Open serial monitor"
	@echo "  clean   - Clean build files"

monitor:
	picocom -b 9600 --echo --imap lfcrlf --omap crcrlf $(MONITOR_PORT)


.PHONY: help
