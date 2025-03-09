#!/bin/sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
export REPO_DIR=${SCRIPT_DIR}/..

# ARM GNU Toolchain
ARM=/opt2/arm/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi

# OpenOCD
OPENOCD=/opt2/openocd

# Raspberry Pi PicoTool
PICOTOOL=/opt2/picotool

# Raspberry Pi Pico SDK
export PICO_SDK_PATH=${REPO_DIR}/pico-sdk

# Raspberry Pi Pico platform
export PICO_PLATFORM=rp2040

# Raspberry Pi Pico board type/variant
export PICO_BOARD=pico_w

# FreeRTOS
export FREERTOS_KERNEL_PATH=${REPO_DIR}/freertos/FreeRTOS-Kernel

# Update PATH
export PATH=${PICOTOOL}/bin:${ARM}/bin:${OPENOCD}/bin:${PATH}
