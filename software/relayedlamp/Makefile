TARGET = main
BOARD = nodemcuv2
F_CPU = 80000000L
OPTIMIZATION = -Os -flto

# ESP8266 paths
ESP8266_PATH = $(HOME)/.arduino15/packages/esp8266/hardware/esp8266/3.1.2
CORE_PATH = $(ESP8266_PATH)/cores/esp8266/
VARIANT_PATH = $(ESP8266_PATH)/variants/nodemcu
TOOLS_PATH = $(ESP8266_PATH)/tools
SDK_PATH = $(ESP8266_PATH)/tools/sdk

# Toolchain paths
GCC_BIN_PATH = $(HOME)/.arduino15/packages/esp8266/tools/xtensa-lx106-elf-gcc/3.1.0-gcc10.3-e5f9fec/bin
GCC_TOOL_PREFIX = xtensa-lx106-elf-

# Library paths
USER_LIB_PATH = ../library ../library/v-usb $(HOME)/Arduino/libraries/
ARDUINO_LIBS = MessageQueue Command Square DallasTemperature OneWire usbdrv

# Tools using extracted paths
CXX = $(GCC_BIN_PATH)/$(GCC_TOOL_PREFIX)g++
CC = $(GCC_BIN_PATH)/$(GCC_TOOL_PREFIX)gcc
OBJCOPY = $(GCC_BIN_PATH)/$(GCC_TOOL_PREFIX)objcopy
AR = $(GCC_BIN_PATH)/$(GCC_TOOL_PREFIX)ar
OBJDUMP = $(GCC_BIN_PATH)/$(GCC_TOOL_PREFIX)objdump
NM = $(GCC_BIN_PATH)/$(GCC_TOOL_PREFIX)nm
CXXFILT = $(GCC_BIN_PATH)/$(GCC_TOOL_PREFIX)c++filt

# ESP8266 compiler flags from Arduino build
CXXFLAGS = -c -g -Wall -Wextra -std=gnu++17 \
           -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD \
           -DNONOSDK22x_190703=1 -DF_CPU=$(F_CPU) -DLWIP_OPEN_SRC -DTCP_MSS=536 \
           -DLWIP_FEATURES=1 -DLWIP_IPV6=0 -DARDUINO=10819 \
           -DARDUINO_ESP8266_NODEMCU_ESP12E -DARDUINO_ARCH_ESP8266 \
           -DARDUINO_BOARD="ESP8266_NODEMCU_ESP12E" -DARDUINO_BOARD_ID="$(BOARD)" \
           -DLED_BUILTIN=2 -DFLASHMODE_DIO \
           -DMMU_IRAM_SIZE=0x8000 -DMMU_ICACHE_SIZE=0x8000 \
           -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ -D_GNU_SOURCE -DESP8266 \
           -mlongcalls -mtext-section-literals -fno-rtti -falign-functions=4 \
           -Werror=return-type -free -fipa-pta $(OPTIMIZATION)

CFLAGS = -c -g -Wall -Wextra -std=gnu11 \
         -ffunction-sections -fdata-sections -MMD \
         -DNONOSDK22x_190703=1 -DF_CPU=$(F_CPU) -DLWIP_OPEN_SRC -DTCP_MSS=536 \
         -DLWIP_FEATURES=1 -DLWIP_IPV6=0 -DARDUINO=10819 \
         -DARDUINO_ESP8266_NODEMCU_ESP12E -DARDUINO_ARCH_ESP8266 \
         -DARDUINO_BOARD="ESP8266_NODEMCU_ESP12E" -DARDUINO_BOARD_ID="$(BOARD)" \
         -DLED_BUILTIN=2 -DFLASHMODE_DIO \
         -DMMU_IRAM_SIZE=0x8000 -DMMU_ICACHE_SIZE=0x8000 \
         -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ -D_GNU_SOURCE -DESP8266 \
         -mlongcalls -mtext-section-literals -falign-functions=4 \
         -Werror=return-type -free -fipa-pta $(OPTIMIZATION)

ASFLAGS = -c -g -x assembler-with-cpp -MMD \
          -mlongcalls -I$(GCC_BIN_PATH)/../include/ \
          -DNONOSDK22x_190703=1 -DF_CPU=$(F_CPU) -DLWIP_OPEN_SRC -DTCP_MSS=536 \
          -DLWIP_FEATURES=1 -DLWIP_IPV6=0 -DARDUINO=10819 \
          -DARDUINO_ESP8266_NODEMCU_ESP12E -DARDUINO_ARCH_ESP8266 \
          -DARDUINO_BOARD="ESP8266_NODEMCU_ESP12E" -DARDUINO_BOARD_ID="$(BOARD)" \
          -DLED_BUILTIN=2 -DFLASHMODE_DIO \
          -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ -D_GNU_SOURCE -DESP8266

# Include paths (CRITICAL: SDK paths must come first)
INCLUDES = -I$(SDK_PATH)/include \
           -I$(SDK_PATH)/lwip2/include \
           -I$(SDK_PATH)/libc/xtensa-lx106-elf/include \
           -I$(CORE_PATH) \
           -I$(VARIANT_PATH) \
           -I.

LIBRARY_INCLUDES = $(foreach lib,$(ARDUINO_LIBS), \
    -I$(shell find $(USER_LIB_PATH) -type d -name "$(lib)" 2>/dev/null | head -n1))

# Preprocess linker scripts (as shown in Arduino build log)
LINKER_SCRIPT_DIR = build/ld_h
PREPROCESSED_LD = build/local.eagle.flash.ld
PREPROCESSED_COMMON_LD = build/local.eagle.app.v6.common.ld

# Linker flags
LDFLAGS = -nostdlib -Wl,--no-check-sections -u app_entry -u _printf_float -u _scanf_float \
          -Wl,-static -L$(SDK_PATH)/lib -L$(SDK_PATH)/lib/NONOSDK22x_190703 \
          -L$(SDK_PATH)/libc/xtensa-lx106-elf/lib \
          -Tlocal.eagle.flash.ld \
          -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,spi_flash_read \
          -Wl,--start-group \
          -lhal -lphy -lpp -lnet80211 -llwip2-536-feat -lwpa -lcrypto -lmain \
          -lwps -lbearssl -lespnow -lsmartconfig -lairkiss -lwpa2 \
          -lstdc++ -lm -lc -lgcc \
          -Wl,--end-group \
          -Wl,-Map,$(TARGET).map \
          $(OPTIMIZATION)

.PHONY: all clean upload debug

all: build/$(TARGET).bin build/$(TARGET).lss build/$(TARGET).sym mem

# Create build directory
$(shell mkdir -p build $(LINKER_SCRIPT_DIR) 2>/dev/null)

# Preprocess linker scripts (as done in Arduino build)
# The flash linker script includes the common one, so we need both
$(PREPROCESSED_LD): $(SDK_PATH)/ld/eagle.flash.4m2m.ld
	@echo "Preprocessing linker script: $<"
	cp $< $(LINKER_SCRIPT_DIR)/local.eagle.flash.ld.h
	$(CC) -CC -E -P -DVTABLES_IN_FLASH -DMMU_IRAM_SIZE=0x8000 -DMMU_ICACHE_SIZE=0x8000 $(LINKER_SCRIPT_DIR)/local.eagle.flash.ld.h -o $@

# Preprocess common linker script with local. prefix
$(PREPROCESSED_COMMON_LD): $(SDK_PATH)/ld/eagle.app.v6.common.ld.h
	@echo "Preprocessing common linker script: $<"
	$(CC) -CC -E -P -DVTABLES_IN_FLASH -DMMU_IRAM_SIZE=0x8000 -DMMU_ICACHE_SIZE=0x8000 $< -o $@

# Main sketch
build/main.o: main.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBRARY_INCLUDES) $< -o $@

# Find all core source files - including subdirectories
CORE_SRCS := $(shell find $(CORE_PATH) -type f -name "*.c" -o -name "*.cpp" -o -name "*.S")

# Debug: Print which core files we found
CORE_CPP_SRCS := $(shell find $(CORE_PATH) -type f -name "*.cpp")
CORE_C_SRCS := $(shell find $(CORE_PATH) -type f -name "*.c")
CORE_S_SRCS := $(shell find $(CORE_PATH) -type f -name "*.S")

# Compile all core files
build/core_objects.stamp: $(CORE_SRCS)
	@echo "Found $(words $(CORE_CPP_SRCS)) .cpp files, $(words $(CORE_C_SRCS)) .c files, $(words $(CORE_S_SRCS)) .S files"
	@echo "Compiling $(words $?) core files..."
	@for src in $?; do \
		obj=build/$$(echo $$src | sed 's|$(CORE_PATH)||' | tr '/' '_' | tr '.' '_').o; \
		echo "Compiling $$src -> $$obj"; \
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
LIB_SRCS = $(foreach lib,$(ARDUINO_LIBS), $(wildcard $(shell find $(USER_LIB_PATH) -type d -name "$(lib)" 2>/dev/null | head -n1)/*.cpp))

build/library_objects.stamp: $(LIB_SRCS)
	@echo "Compiling $(words $?) library files..."
	@for src in $?; do \
		libname=$$(basename $$(dirname $$src)); \
		filename=$$(basename $$src .cpp); \
		obj=build/$${libname}_$${filename}.o; \
		echo "Compiling $$src -> $$obj"; \
		$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBRARY_INCLUDES) $$src -o $$obj; \
	done
	@touch $@

# Create core library archive
build/core.a: build/core_objects.stamp
	@echo "Creating core library archive..."
	$(AR) cru $@ build/*_*.o
	@echo "Core archive created with $$(ar t $@ | wc -l) object files"

# The elf target depends on preprocessed linker scripts
# We run the linker from the build directory so it can find the linker scripts
build/$(TARGET).elf: build/main.o build/library_objects.stamp build/core.a $(PREPROCESSED_LD) $(PREPROCESSED_COMMON_LD)
	@echo "Linking..."
	cd build && \
	$(CC) -nostdlib -Wl,--no-check-sections -u app_entry -u _printf_float -u _scanf_float \
	      -Wl,-static \
	      -Tlocal.eagle.flash.ld \
	      -Wl,-Map,main.map \
	      -o main.elf \
	      main.o *_*.o core.a \
	      -L$(SDK_PATH)/lib -L$(SDK_PATH)/lib/NONOSDK22x_190703 \
	      -L$(SDK_PATH)/libc/xtensa-lx106-elf/lib \
	      -Wl,--start-group \
	      -lhal -lphy -lpp -lnet80211 -llwip2-536-feat -lwpa -lcrypto -lmain \
	      -lwps -lbearssl -lespnow -lsmartconfig -lairkiss -lwpa2 \
	      -lstdc++ -lm -lc -lgcc \
	      -Wl,--end-group \
	      -Wl,--gc-sections \
	      -Wl,-wrap,system_restart_local -Wl,-wrap,spi_flash_read \
	      $(OPTIMIZATION)

# Create binary for flashing
build/$(TARGET).bin: build/$(TARGET).elf
	$(ESP8266_PATH)/tools/elf2bin.py --eboot $(ESP8266_PATH)/bootloaders/eboot/eboot.elf \
	                                 --app $< \
	                                 --flash_mode dio \
	                                 --flash_freq 40 \
	                                 --flash_size 4M \
	                                 --path $(GCC_BIN_PATH) \
	                                 --out $@
	@echo "Created $@"

build/$(TARGET).lss: build/$(TARGET).elf
	$(OBJDUMP) -C -h -S $< > $@

build/$(TARGET).sym: build/$(TARGET).elf
	$(NM) -n $< > $@.mangled
	$(NM) -n $< | $(CXXFILT) > $@
	@echo "Generated demangled symbols in $@ and mangled symbols in $@.mangled"

upload: build/$(TARGET).bin
	esptool.py --port /dev/ttyUSB0 --baud 115200 write_flash -fm dio 0x00000 $<

mem: build/$(TARGET).elf
	$(GCC_BIN_PATH)/$(GCC_TOOL_PREFIX)size --format=berkeley $<

clean:
	rm -rf build $(LINKER_SCRIPT_DIR)

debug:
	@echo "Core files found: $(words $(CORE_SRCS))"
	@echo "Library files found: $(words $(LIB_SRCS))"
	@echo "GCC_BIN_PATH: $(GCC_BIN_PATH)"
	@echo "GCC_TOOL_PREFIX: $(GCC_TOOL_PREFIX)"
	@echo "CXX: $(CXX)"
	@echo "CC: $(CC)"
	@echo "SDK libc path: $(SDK_PATH)/libc/xtensa-lx106-elf/lib"
	@echo "PREPROCESSED_LD: $(PREPROCESSED_LD)"
	@echo "PREPROCESSED_COMMON_LD: $(PREPROCESSED_COMMON_LD)"
	@ls -la build/*.o 2>/dev/null | wc -l | xargs echo "Object files in build:"
	@echo "First 3 core files:"
	@for f in $(wordlist 1,3,$(CORE_SRCS)); do echo "  $$f"; done
	@echo "INCLUDES: $(INCLUDES)"
	@echo "Checking linker scripts in build directory..."
	@ls -la build/*.ld 2>/dev/null || echo "No linker scripts found"
	@echo "Checking for LwipDhcpServer files..."
	@find $(CORE_PATH) -name "*DhcpServer*" -type f 2>/dev/null
	@echo "Checking if LwipDhcpServer-NonOS.cpp is in CORE_SRCS..."
	@for f in $(CORE_SRCS); do echo $$f | grep -i "dhcp" && echo "  Found: $$f"; done
