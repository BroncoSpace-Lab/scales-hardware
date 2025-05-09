# Design Notes

This design for the i.MX 8X SCALES Custom Devlopment Board is mostly based on PhyTec's Development Kit design. For clarity, this version of the SCALES board will be called "SCALES board" and the PhyTec Devlopment Kit board will be called "dev kit."

Version 1 of the SCALES board is for development only. Future versions of the board as we develop for flight will include the power and watchdog system on the same board as the flight computer. The development boards for each of these subsystems will be isolated for testing before combining them into one board for flight.

Most of this design is based on the dev kit schematics and Hardware Manual, available from PhyTec upon request. This link to the [dev kit](https://www.phytec.com/product/phycore-i-mx-8-development-kit/) should supply the necessary supporting documentation used for this design.

## Elements of the SCALES Board

The main elements of the dev kit that are used in the SCALES board are:

- Micro USB UART Debug port
- 2 Ethernet ports
- Micro SD Card Reader

Elements of the dev kit that will be altered significantly for the SCALES board:

- Power input and distribution
- SPI
- UART
- I2C

## Schematic Design

The schematic is organized into heirarchical pages to make the page cleaner and more concise.

The root of the schematic includes the board to board (B2B) connectors that interface the SOM to the SCALES board. The same connectors are used in the dev kit.

This page will be organized to show essential connections between blocks as the schematic progresses.

This version of the SCALES board for development will use DF11 connectors for power input, SPI, UART, and I2C. Later versions of this board for flight will use Gecko connectors. The DF11 is used to reduce lead times for the initial versions of the board.

Important note: all signals from the B2B connectors on the SCALES board begin with "X_". The same naming convention is used in the dev kit.

### Micro USB UART Debug

Replicated as much of the dev kit design as possible. Main changes are:

- MBR0520L is no longer manufactured. using MBR0520LT
- There is a dip switch and a multiplexor that allow for FTDI to be enabled. The dip switch and multiplexor were removed. (they might be brought back later as we continue to develop UART)

### Ethernet

The i.MX 8X SOM has an onboard ethernet transciever for ETH0, but not ETH1. Any carrier board connected to the SOM is required to implement a transciever for ETH1. The design for this transciever and the supporting ethernet female connector plugs were directly pulled from the dev kit schematic, with the only changes being specific part numbers for components and connectors. The functionality is the same.

### Micro SD Card Reader

Everything in the SCALES board is the same as the dev kit except:

- No part # for dip switch selected yet
- Different SD card reader part # used. The one use in the dev kit was not available from JLC PCB.
- Removed Enable_WIFI switch. This signal is pulled low elsewhere for the multiplexor to still work and we do not need WiFi on the SCALES board, so the functionality was removed.

The multiplexor and switches allow you to switch between NAND flash/boot and SD flash/boot. You cannot use SD at the same time as WiFi. Since we will not be using WiFi, this will not be an issue, but the Enable_WIFI signal still needs to be pulled low for the multiplexor to work for switching between SD and NAND.

### Power Input and Distribution

The dev kit used a 12V input. The SOM only requires a 3.3V input at 2A, so we will be using a 3.3V input for the SCALES board.

The dev kit also gets input power from a barrel jack wall plug. The SCALES board (version 1 development) will be getting input from a DF11 2x8 pin connector through a tabletop power supply.

The DF11 power connector used in the SCALES board will also be able to get input power from the SCALES Power Distribution board. Pinouts are still being discussed across boards to maintain consistency.

The power input and distribution system of the imx SCALES board is still in development. Trying to figure out how the voltage rails will translate from the dev kit without the 12V input.

The i.MX 8X SOM generates 3.3V and 1.8V voltage rails, but the dev kit does not use them. Not sure if SCALES should just use the rails or generate new rails.

The dev kit also has a 5V rail, but all the components that use the 5V rail will not be used in SCALES, so it will most likely be omitted. 

### SPI

There are expansion header pins on the last page of the dev kit schematic that allow connection to the SPI pins not in use elsewhere on the board. These signals will be connected to the DF11 for use on the SCALES board.

### UART

The UART on the i.MX 8X dev kit is accessible through header pins, RS232, and the Debug USB. We already covered the Debug USB, so figuring out how to use the signals from the header pins and the RS232 for the DF11 is the next step for the SCALES board. Still in progress.

### I2C

I2C on the dev kit seems pretty straightforward, all that needs to be done is confirm the pinouts for the DF11 with the peripheral board to make sure that everything lines up correctly across both boards for continuity in the design of the system.