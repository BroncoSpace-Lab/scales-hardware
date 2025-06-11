# Peripheral Board
The peripheral board is conprized for two different purposes.

1. Inter communication between the Jetson and i.MX8X.
2. Gives the end user access to ports to attach the peripherals that they want. 
    The ports given:
    * At least 1 Ethernet port
    * At least 2 SPI/UART ports
## General Information of each major block of the board

![Peripheral Board General Info & Block Diagram](docs\Images\peripheral_board_info_diagram.png)

The above image is of the root page of the schematic with basic infomration and a block diagram of the board and its use.

This is the front of the board in KiCad's 3D view.
![Peripheral Board Front](docs\Images\peripheral_board_front.PNG)

Some of the models are not in the correct placement beacuse their 3D model was incorrectly created. They are placed in the correctly on the board.

This is the back of the board in KiCad's 3D view.
![Peripheral Board Back](docs\Images\peripheral_board_back.PNG)

## Break down of the three core parts
Each section is about the individual chips and it has basic information on the layout.
 
### Raspberry Pi
[JLC Link](https://jlcpcb.com/partdetail/RaspberryPi-RP2350A/C42411118) to the RP2350A.

Layout and other information from the [PROVES dev kit](https://github.com/proveskit) and the [Raspberry Pi Hardware minimum configuration](extension://efaidnbmnnnibpcajpcglclefindmkaj/https://datasheets.raspberrypi.com/rp2350/hardware-design-with-rp2350.pdf)

This can be bought as a development board as a standalone product for project use.

### Wiznet
[JLC Link](https://jlcpcb.com/partdetail/Wiznet-W5500/C32843) to the Wiznet W5500.

Layout and other informatuon from the [PROVES dev kit](https://github.com/proveskit) and [Wiznet W5500 Eval Board](https://github.com/Wiznet/W5500-EVB)

This product can be purchased bought as [WIZ850io](https://wiznet.io/products/network-modules/wiz850io) which is a compact module of just the chip and an RJ45 or as [W5500-EVB-Pico](https://wiznet.io/products/evaluation-boards/w5500-evb-pico) which is the chip with a Raspberry Pi Pico with open holes for GPIO manipulation.

There is [Coding with CircuitPython on W5100S-EVB-Pico2](https://maker.wiznet.io/viktor/projects/coding-with-circuitpython-on-w5100s-evb-pico2/) that is on the Wiznet Makers website that should be easy to follow for setting up the Wiznet RP combination.

### 5-port Ethernet Switch
[JLC Link](https://jlcpcb.com/partdetail/MicrochipTech-KSZ8795CLXIC/C69416) to the Microchip 5-Port Ethernet Switch.

The original eval board is no longer made but there is an update of the chip with a [newer Eval Board](https://www.microchip.com/en-us/development-tool/KSZ8795CLXD-EVAL). For this project we will be using the strap in mode of the chip to be just a 4 port etherenet switch that will not require any software or modification. The last port will be left open for possible future iterations.

## How to used the board
*Still in Development*
The RP2350 needs to be coded to ensure that the correct GPIO pins are set up in the correct signals.
    ### User Ports
    * GPIO 8 through 11 are UART labeled as UART1.
    * GPIO 12 through 15 are SPI labeled as SPI1
    
    ### Internal Connection
    * SPI0 is being held for the data transmission between the Rasberry Pi and the Wiznet.

*Taken with a grain of salt*
The Wizenet W5500 and Microchip KSZ8795CLXIC should be preconfigured to work as is. The Microchip KSZ8795CLXIC could be one reconfigured to utilize the 5th port as different data rates if they are needed.