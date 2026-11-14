#!/bin/sh

# Argument1: The filename of qemu executable, e.g. qemu-system-riscv64
QEMU_PATH=$(which "$1")
RET=$?
MINIMUM_MAJOR_VERSION=7
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
if [ "$RET" -ne 0 ]; then
    echo "$1 not found"
    exit 1
else
    QEMU_VERSION=$("$1" --version | head -n 1 | sed -n 's/.*version \([0-9][0-9.]*\).*/\1/p')
    MAJOR_VERSION=$(echo "$QEMU_VERSION" | awk -F '.' '{print $1}')
    if [ -z "$QEMU_VERSION" ] || [ "$MAJOR_VERSION" -lt "$MINIMUM_MAJOR_VERSION" ]; then
        echo "${RED}Error: Required major version of QEMU is ${MINIMUM_MAJOR_VERSION}, but current is ${QEMU_VERSION}.${NC}"
        exit 1
    else
        echo "${GREEN}QEMU version is ${QEMU_VERSION} (>= ${MINIMUM_MAJOR_VERSION}), OK!${NC}"
        exit 0
    fi
fi
