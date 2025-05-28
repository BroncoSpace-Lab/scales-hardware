# Peripheral Board
The peripheral board is conprized for two different purposes.

1. Inter communication between the Jetson and i.MX8X.
2. Gives the end user access to ports to attach the peripherals that they want. 
    The ports given:
    * At least 1 Ethernet port
    * At least 2 SPI/UART ports

## Raspberry Pi
[JLC Link](https://jlcpcb.com/partdetail/RaspberryPi-RP2350A/C42411118) to the RP2350A.

Layout and other information from the [PROVES dev kit](https://github.com/proveskit) and the [Raspberry Pi Hardware minimum configuration](extension://efaidnbmnnnibpcajpcglclefindmkaj/https://datasheets.raspberrypi.com/rp2350/hardware-design-with-rp2350.pdf)

This can be bought as a development board as a standalone product for project use.

## Wiznet
[JLC Link](https://jlcpcb.com/partdetail/Wiznet-W5500/C32843) to the Wiznet W5500.

Layout and other informatuon from the [PROVES dev kit](https://github.com/proveskit) and [Wiznet W5500 Eval Board](https://github.com/Wiznet/W5500-EVB)

This product can be purchased bought as [WIZ850io](https://wiznet.io/products/network-modules/wiz850io) which is a compact module of just the chip and an RJ45 or as [W5500-EVB-Pico](https://wiznet.io/products/evaluation-boards/w5500-evb-pico) which is the chip with a Raspberry Pi Pico with open holes for GPIO manipulation.

## 5-port Ethernet Switch
[JLC Link](https://jlcpcb.com/partdetail/MicrochipTech-KSZ8795CLXIC/C69416) to the Microchip 5-Port Ethernet Switch.

The original eval board is no longer made but there is an update of the chip with a [newer Eval Board](https://www.microchip.com/en-us/development-tool/KSZ8795CLXD-EVAL). For this project we will be using the strap in mode of the chip to be just a 4 port etherenet switch that will not require any software or modification. The last port will be left open for possible future iterations.

## How to used the board
The RP2350 needs to be coded to ensure that the correct GPIO pins are set up in the correct signals.
    ### User Ports
    * GPIO 8 through 11 are UART labeled as UART1.
    * GPIO 12 through 15 are SPI labeled as SPI1
    
    ### Internal Connection
    * SPI0 is being held for the data transmission between the Rasberry Pi and the Wiznet.

*Taken with a grain of salt*
The Wizenet W5500 and Microchip KSZ8795CLXIC should be preconfigured to work as is. The Microchip KSZ8795CLXIC could be one reconfigured to utilize the 5th port as different data rates if they are needed.