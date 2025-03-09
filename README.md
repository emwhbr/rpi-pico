# rpi-pico

## General
Exploring the Raspberry Pi Pico W.

## Dependencies
All projects have been built and tested with:

* Linux Mint 21 Vanessa (x86_64)
* Arm GNU Toolchain, 14.2.rel1-x86_64-arm-none-eabi
* OpenOCD, built from sources (see notes below)
* SEGGER J-Link debug probe PRO, 6.0
* SEGGER J-Link software, V8.16
* SEGGER Ozone, V3.38d

## Notes regarding OpenOCD
OpenOCD was built from sources:
* repo: https://github.com/openocd-org/openocd.git
* commit: 4593c75f0b45ebb1bf10350c26c0163d0676f81a

Configure and build:
```
cd openocd
./bootstrap
./configure --prefix=/opt2/openocd --enable-jlink
make
sudo make install
```

## Use this repository
Clone and preparations:
```
git clone https://github.com/emwhbr/rpi-pico.git
cd rpi-pico
git submodule update --init
cd pico-sdk
git submodule update --init
cd ..
cd freertos/FreeRTOS-Kernel
git submodule update --init
cd ..
```
Setup the environment:
```
source ./scripts/setup_environment.sh
```
Note! This script uses custom paths for the ARM GNU Toolchain and OpenOCD.<br/>
Change the paths to match the settings on your system.

## Build a project, example for 'blink'
```
cd proj/blink/out
cmake ..
make
make program_flash
make reset
```
## Build a project for debug, example for 'blink_freertos'
```
cd proj/blink_freertos/out
cmake -DCMAKE_BUILD_TYPE=Debug
make VERBOSE=1
make program_flash
make reset
```
Start the SEGGER Ozone debugger.<br/>
Debugger project file: proj/blink_freertos/jlink/blink_freertos.jdebug<br/>
<br/>
Attach & Halt Program
