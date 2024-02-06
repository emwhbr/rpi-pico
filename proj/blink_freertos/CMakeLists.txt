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
)

# FreeRTOS include directories
target_include_directories(${PROJECT_NAME} PRIVATE
   $ENV{REPO_DIR}/freertos/
   $ENV{FREERTOS_KERNEL_PATH}/include
   $ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/include
)

# Create bin/dis/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions).
# Add pico_stdlib library which aggregates commonly used features.
target_link_libraries(${PROJECT_NAME} 
   pico_stdlib            # For core functionality
   pico_cyw43_arch_none   # We need CYW to access the GPIO, but we don't need anything else
   FreeRTOS-Kernel        # FreeRTOS kernel
   FreeRTOS-Kernel-Heap4  # FreeRTOS heap implementation
)

# Enable/disable USB and UART output
pico_enable_stdio_usb(${PROJECT_NAME} 0)   # Disable USB
pico_enable_stdio_uart(${PROJECT_NAME} 1)  # Enable UART (default speed 115200)
