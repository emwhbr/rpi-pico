# rpi-pico

## General
Exploring the Raspberry Pi Pico W.

## Use this repository
Clone and preparations:
```
git clone https://github.com/emwhbr/rpi-pico.git
cd rpi-pico/pico-sdk
git submodule update --init
cd ..
```
Setup the environment:
```
source ./scripts/setup_environment.sh
```
Note! This script uses a custom path for the ARM GNU Toolchain.<br/>
Change this path so that it matches the settings on your system.
## Build project 'blink'
```
cd proj/blink/out
cmake ..
make
```
