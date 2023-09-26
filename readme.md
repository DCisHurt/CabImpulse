
# Cab Impulse

[![GitHub Release](https://img.shields.io/badge/https%3A%2F%2Fgithub.com%2FDCisHurt%2FCabImpulse%2Freleases%2Ftag%2Fv1.0.0)]()

Cab Impulse is an impulse response generator for guitar cabinets based on real studio recordings. Currently Supports VST3 and Windows Standalone application.

## Download

Download the latest release [here](https://github.com/DCisHurt/CabImpulse/releases/tag/v1.0.0).

## Features

This plugin is designed to be used with a guitar cab simulator. It can be used to simulate the sound of the cabinet and the microphone, including :

- 17 guitar cabinets and 4 bass cabinets.
- Adjustable microphone type and position.
- Built-in high-cut and low-cut filters.

## Support cabinet list

- Marshall 1936 2X12
- Marshall 1960 4X12
- Marshall 1960AHW 4X12
- Marshall 1970 4X12
- Line 6 Vetta 4X12
- ENGL Pro 4X12
- Randall RS412XLT100 4X12
- Krank Krankenstein 4X12
- Bogner Uberkab 4X12
- Mesa Standard 4X12
- Orange 4X12
- Genz Benz G-Flex ported 2X12
- Fender 1965 Super Reverb 4X10
- Roland JC120 2X12
- Supro Thunderbolt 1X15
- Vox AC30 2X12
- Fender Deluxe 1X12
- Ampeg SVT 8X10
- Ampeg Portaflex 1X15
- Aguilar DB 4X12
- Gallien-Krueger Neo 4X10

## Support Microphone list

- R121
- MD409
- MD421
- S545SD
- SM57
- M30
- U87
- ADi5

- Support MIDI Polyphonic Expression ([MPE](https://www.midi.org/midi-articles/midi-polyphonic-expression-mpe)) standard.

## Clone and build

This repository is written in C++ and uses the [JUCE](https://juce.com/) framework. To build it, you need to have [CMake](https://cmake.org/) installed.

``` shell
git clone https://github.com/DCisHurt/CabImpulse.git
cd CabImpulse/
git submodule update --init
cmake --build build --target ALL_BUILD
```
