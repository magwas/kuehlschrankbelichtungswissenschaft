
TARGET = main
MCU = atmega32
F_CPU = 8000000L
OPTIMIZATION = -Os #-flto

# Core paths
CORE_PATH = $(HOME)/.arduino15/packages/MightyCore/hardware/avr/3.0.3/cores/MCUdude_corefiles
VARIANT_PATH = $(HOME)/.arduino15/packages/MightyCore/hardware/avr/3.0.3/variants/standard

# Library paths
USER_LIB_PATH = ../library $(HOME)/Arduino/libraries/
ARDUINO_LIBS = MessageQueue Command Square DallasTemperature OneWire

# Tools
CXX = /usr/bin/avr-g++
CC = /usr/bin/avr-gcc
OBJCOPY = /usr/bin/avr-objcopy
AVRDUDE = /usr/bin/avrdude
OBJDUMP = /usr/bin/avr-objdump
NM = /usr/bin/avr-nm
CXXFILT = /usr/bin/avr-c++filt

# Compiler flags
CXXFLAGS = -c -g -Wall -Wextra -std=gnu++17 -fpermissive -fno-exceptions \
           -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD \
           -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DARDUINO=10819 \
           -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR $(OPTIMIZATION) -g

CFLAGS = -c -g -Wall -Wextra -std=gnu11 \
         -ffunction-sections -fdata-sections -MMD \
         -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DARDUINO=10819 \
         -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR $(OPTIMIZATION) -g

ASFLAGS = -c -g -Wall -Wextra -std=gnu11 \
          -ffunction-sections -fdata-sections -MMD \
          -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DARDUINO=10819 \
          -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR $(LOPTIMIZATIONTO) -g \
          -x assembler-with-cpp

INCLUDES = -I$(CORE_PATH) -I$(VARIANT_PATH) -I.
LIBRARY_INCLUDES = $(foreach lib,$(ARDUINO_LIBS), \
    -I$(shell find $(USER_LIB_PATH) -type d -name "$(lib)" 2>/dev/null | head -n1))

LDFLAGS = -mmcu=$(MCU) -Wl,--gc-sections,--section-start=.FAR_MEM1=0x10000 \
          -Os -flto -w -Wl,--gc-sections -Wl,-u,vfprintf -lprintf_flt -lm -Wl,--undefined-version
#-nostartfiles

.PHONY: all clean upload debug

all: build/$(TARGET).hex build/$(TARGET).lss build/$(TARGET).sym mem

# Create build directory
$(shell mkdir -p build 2>/dev/null)

# Main sketch
build/main.o: main.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBRARY_INCLUDES) $< -o $@

# Find all core source files
CORE_SRCS := $(shell find $(CORE_PATH) -name "*.c" -o -name "*.cpp" -o -name "*.S")

# Compile all core files - FIXED: Use case instead of [[ for /bin/sh compatibility
build/core_objects.stamp: $(CORE_SRCS)
	@echo "Compiling $(words $?) core files..."
	@for src in $?; do \
		obj=build/$$(basename $$src).o; \
		case "$$src" in \
			*.c) \
				$(CC) $(CFLAGS) $(INCLUDES) $$src -o $$obj; \
				;; \
			*.cpp) \
				$(CXX) $(CXXFLAGS) $(INCLUDES) $$src -o $$obj; \
				;; \
			*.S) \
				$(CC) $(ASFLAGS) $(INCLUDES) $$src -o $$obj; \
				;; \
		esac; \
	done
	@touch $@

# Compile libraries
LIB_SRCS = $(foreach lib,$(ARDUINO_LIBS), $(wildcard $(USER_LIB_PATH)/$(lib)/*.cpp))

build/library_objects.stamp: $(LIB_SRCS)
	@echo "Compiling $(words $?) library files..."
	@for src in $?; do \
		libname=$$(basename $$(dirname $$src)); \
		filename=$$(basename $$src .cpp); \
		obj=build/$${libname}_$${filename}.o; \
		$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBRARY_INCLUDES) $$src -o $$obj; \
	done
	@touch $@

build/$(TARGET).elf: build/main.o build/core_objects.stamp build/library_objects.stamp
	@echo "Linking..."
	$(CC) $(LDFLAGS) -o $@ build/*.o -lm

build/$(TARGET).hex: build/$(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo "Created $@"

build/$(TARGET).lss: build/$(TARGET).elf
	$(OBJDUMP) -C -h -S $< > $@

build/$(TARGET).sym: build/$(TARGET).elf
	$(NM) -n $< > $@.mangled
	$(NM) -n $< | $(CXXFILT) > $@
	@echo "Generated demangled symbols in $@ and mangled symbols in $@.mangled"

upload: build/$(TARGET).hex
	$(AVRDUDE) -v -p$(MCU) -cstk500v1 -P/dev/ttyACM0 -b19200 -Uflash:w:$<:i

mem: build/$(TARGET).elf
	avr-size --format=avr --mcu=$(MCU) build/$(TARGET).elf
clean:
	rm -rf build

debug:
	@echo "Core files found: $(words $(CORE_SRCS))"
	@echo "Library files found: $(words $(LIB_SRCS))"
	@ls -la build/*.o 2>/dev/null | wc -l | xargs echo "Object files in build:"
	@echo "First 3 core files:"
	@for f in $(wordlist 1,3,$(CORE_SRCS)); do echo "  $$f"; done
