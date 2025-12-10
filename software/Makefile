# Arduino Makefile for Arduino Micro
BOARD_TAG    = micro
MONITOR_PORT = /dev/ttyACM0
ARDUINO_LIBS = Wire MCP7940 OneWire DallasTemperature EEPROM
LDFLAGS = -Wl,-u,vfprintf -lprintf_flt
# Arduino directory (adjust if needed)
ARDUINO_DIR = /usr/share/arduino

# Source files
TARGET = main
SRC = src/$(TARGET).cc

all: version tests $(TARGET_EEP) $(TARGET_HEX)

run: all upload monitor


# Include Arduino.mk
include /usr/share/arduino/Arduino.mk

tests:
	make -C test

clean::
	make -C test clean ; rm -f version.h

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build the project"
	@echo "  upload  - Build and upload"
	@echo "  monitor - Open serial monitor"
	@echo "  clean   - Clean build files"

monitor:
	@echo "Starting resilient serial monitor (C-a C-x to exit permanently)..."
	@while true; do \
		if [ -c $(MONITOR_PORT) ]; then \
			echo "Device found, starting picocom..."; \
			picocom -b 9600 --echo --imap lfcrlf $(MONITOR_PORT); \
			exit_code=$$?; \
			if [ $$exit_code -eq 0 ]; then \
				echo "Picocom exited cleanly (C-a C-x). Stopping monitor."; \
				break; \
			else \
				echo "Picocom exited with error (device disconnected?). Waiting..."; \
				sleep 2; \
			fi; \
		else \
			echo "Waiting for $(MONITOR_PORT) to appear..."; \
			while [ ! -c $(MONITOR_PORT) ]; do sleep 1; done; \
			echo "Device detected!"; \
		fi; \
	done

PHONY: version
version:
	./generate_version.sh > version.h 2>/dev/null || echo "// Could not generate version from Git" > version.h
.PHONY: help
