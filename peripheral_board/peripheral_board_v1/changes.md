# Changes from v0 to V1

## Would like to do the following
* Changed removed RJ45 ports to be Tyco ports
    * This required to add magnetics onto the board to ensure singal integrity
    * Also connected the signals from the Wiznet to the ethernet switch to improve space.
    * Need to do impedance control on the signals
* Added a DF11 (Geko Stand in) for power and communication.
* Comb throught [hardware design check list](https://ww1.microchip.com/downloads/en/DeviceDoc/KSZ8795CLX-Hardware-Design-Checklist-00003579A.pdf) to ensure that it is properly set up
    * Need to check the magnetics
    * Check the impedence of the the traces
* Change the overall stack up of the board 
    * *Might need to be a more simple layering where they signals are next to ground*

## Changes made so far

### Wiznet
* Changed the Wiznet layout to match that of their development board
* Restructured the RJ45 connections
    * *Looked over the RJ45 still a unsure*
    * Added in 2 differnt versions of Ethernt Connections. The one that shows the magnetics might be used for the Ethernet switch.
* Removed the Single Bus Buffer gate with 3-state output to match the dev board

### Raspberry Pi
* Moved to USB type C from micro USB for debugging.
* Changed the GPIO pins to match the Wiznet dev board.
* Brought in breakout pins for the rest of the GPIO.
* Cleaned up the Schematic to make it more clean.

### Ethernet Switch
* Changed the crystal to be the same as the Wiznet and to have the correct load caps
* 