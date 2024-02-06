#!/bin/sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
export REPO_DIR=${SCRIPT_DIR}/..

# ARM GNU Toolchain
ARM=/opt2/arm/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi

# OpenOCD
OPENOCD=/opt2/openocd/

# Raspberry Pi Pico SDK
export PICO_SDK_PATH=${REPO_DIR}/pico-sdk

# Raspberry Pi Pico platform
export PICO_PLATFORM=rp2040

# Raspberry Pi Pico board type/variant
export PICO_BOARD=pico_w

# FreeRTOS
export FREERTOS_KERNEL_PATH=${REPO_DIR}/freertos/FreeRTOS-Kernel

# Update PATH
export PATH=${ARM}/bin:${OPENOCD}/bin:${PATH}
