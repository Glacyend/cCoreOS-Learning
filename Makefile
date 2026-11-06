MODE ?= release
BUILD_DIR := build
KERNEL_ELF := $(BUILD_DIR)/kernel.elf

QEMU := qemu-system-riscv64
QEMU_ARGS := -M virt -m 128M -nographic -bios default -kernel $(KERNEL_ELF)

CMAKE := cmake
TOOLCHAIN := $(CURDIR)/tools/riscv64.cmake

.PHONY: all build run clean

all: build

build:
	@echo ">>> Configuring kernel ($(MODE))"
	@$(CMAKE) -B $(BUILD_DIR) -S kernel -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DCMAKE_BUILD_TYPE=$(MODE)
	@echo ">>> Building kernel"
	@$(CMAKE) --build $(BUILD_DIR)

run: build
	@echo ">>> Starting QEMU"
	@$(QEMU) $(QEMU_ARGS)

clean:
	@echo ">>> Removing $(BUILD_DIR)"
	@rm -rf $(BUILD_DIR)
