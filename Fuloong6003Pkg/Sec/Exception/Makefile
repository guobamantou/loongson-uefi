CURRENT_DIR=$(shell pwd)
BUILD_OUT_DIR=Build
INCLUDE_DIR=$(CURRENT_DIR)/../../Include


all: prepare build

prepare:
	rm -rf $(CURRENT_DIR)/Build
	mkdir $(CURRENT_DIR)/Build
build:
	mipsel-linux-gnu-gcc -E $(CURRENT_DIR)/Asm/Exception.S -I $(INCLUDE_DIR) -I $(INCLUDE_DIR)/Loongson/ -I $(INCLUDE_DIR)/Loongson/Loongson2f/ -o $(BUILD_OUT_DIR)/Exception.s
	mipsel-linux-gnu-as $(BUILD_OUT_DIR)/Exception.s -o $(BUILD_OUT_DIR)/Exception.o
	mipsel-linux-gnu-ld -e _ModuleEntryPoint -u _ModuleEntryPoint -T $(CURRENT_DIR)/ld.script $(BUILD_OUT_DIR)/Exception.o -o $(BUILD_OUT_DIR)/Exception
	mipsel-linux-gnu-objcopy -O binary $(BUILD_OUT_DIR)/Exception $(CURRENT_DIR)/Exception.bin

clean:
	rm -rf $(CURRENT_DIR)/Build
