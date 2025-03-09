# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(blink_freertos C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Include common project functions
include($ENV{REPO_DIR}/proj/common_import.cmake)

# Include FreeRTOS
include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
   main.c
   FreeRTOSConfig.h
)

# FreeRTOS include directories
target_include_directories(${PROJECT_NAME} PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}
   $ENV{FREERTOS_KERNEL_PATH}/include
   $ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/include
)

# Create bin/dis/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Set the build type, with or without flash
# Type            Linker script           Comment
# no_flash        memmap_no_flash.ld      No flash, RAM only, execute in RAM
# blocked_ram     memmap_blocked_ram.ld   Flash, RAM is non-striped, execute in flash (xip)
# copy_to_ram     memmap_copy_to_ram.ld   Flash, copy to ram and execute in RAM
# default         memmap_default.ld       Flash, execute in flash (xip)
pico_set_binary_type(${PROJECT_NAME} default)

# We need the subdirectory for the project support library
add_subdirectory($ENV{REPO_DIR}/proj/support support)

# Link to pico_stdlib (gpio, time, etc. functions).
# Add pico_stdlib library which aggregates commonly used features.
target_link_libraries(${PROJECT_NAME} 
   pico_stdlib            # For core functionality
   pico_cyw43_arch_none   # We need CYW to access the GPIO, but we don't need anything else
   FreeRTOS-Kernel        # FreeRTOS kernel
   FreeRTOS-Kernel-Heap4  # FreeRTOS heap implementation
   proj-support           # Project support library
)

# Enable/disable USB and UART output
pico_enable_stdio_usb(${PROJECT_NAME} 0)   # Disable USB
pico_enable_stdio_uart(${PROJECT_NAME} 1)  # Enable UART (default speed 115200)
