# TTT Firmware
## Requirements
### Hardware
- 2x SSD1306 OLED I2C 0.96" White Display
- 2x Arduino Nano
- 2x USB A to B micro
- 4x Push buttons
- 2x HC-05 Bluetooth module
- A bunch of wires

### Software
#### Optional
 - [Fritzing](http://fritzing.org/download/) for schematics
 
## Setup
### Board Configuration

I've had more luck with using **old bootloader** for some reason.

| Setting    | Value                              |
| ---------- | ---------------------------------- |
| Board      | Arduino Nano (Arduino AVR Boards)  |
| Processor  | ATmega328P (Old Bootloader)        |
| Programmer | AVRISP mkII                        | 

### Circuit
![Breadboard schematic](https://github.com/anthonyec/ttt_firmware/blob/master/docs/ttt_proto_schematic_2.png)
