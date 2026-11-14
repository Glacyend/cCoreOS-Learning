MODE ?= release
USER_BUILD := user/build
BUILD_DIR := build
KERNEL_ELF := $(BUILD_DIR)/kernel.elf

QEMU := qemu-system-riscv64
QEMU_ARGS := -M virt -m 128M -nographic -bios default -kernel $(KERNEL_ELF)

CMAKE := cmake
TOOLCHAIN := $(CURDIR)/tools/riscv64.cmake

.PHONY: all build run clean

all: build

build:
	@echo ">>> Configuring user ($(MODE))"
	@$(CMAKE) -B $(USER_BUILD) -S user -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DCMAKE_BUILD_TYPE=$(MODE)
	@echo ">>> Building user"
	@$(CMAKE) --build $(USER_BUILD)
	@echo ">>> Generating embed_apps.S"
	@python3 tools/generate_apps.py
	@echo ">>> Configuring kernel ($(MODE))"
	@$(CMAKE) -B $(BUILD_DIR) -S kernel -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DCMAKE_BUILD_TYPE=$(MODE)
	@echo ">>> Building kernel"
	@$(CMAKE) --build $(BUILD_DIR)

run: build
	@echo ">>> Checking QEMU"
	@sh tools/qemu-ver-check.sh $(QEMU)
	@echo ">>> Starting QEMU"
	@$(QEMU) $(QEMU_ARGS)

clean:
	@echo ">>> Removing $(BUILD_DIR) and $(USER_BUILD)"
	@rm -rf $(BUILD_DIR) $(USER_BUILD)
