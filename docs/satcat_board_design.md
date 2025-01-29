# SatCat Custom Carrier Board Design

This document shows the progress of development of the carrier board for SatCat5.
The base of the design is based on Aerospace Corp's SatCat5 open source ethernet switcher. Their GitHub includes a prototype design featuring the AC701 Evaluation Board for the Artix 7 FPGA chip. For the purposes of SCALES, an evaluation board like that is not something we would want in our design, so we are using an SOM of the Artix 7 instead.

The idea is to take the important signals from SatCat's prototype using the AC701 and modify it to work with the SOM we would use in SCALES.
This presents a problem for us, since the AC701 uses a different version of the Artix 7 than the SOM, so the signals don't match up to the same pins on the Artix 7 chip.

## Mapping the Signals

The AC701 Evaulation Board used in the SatCat prototype uses the xc7a200tfbg676 version of the Artix 7 200t FPGA. The SOM that will be used in SCALES uses the xc7a200tfbg484 version of the Artix 7 200t FPGA.

Resources:

- [SatCat prototype PCB design](https://github.com/the-aerospace-corporation/satcat5/tree/main/examples/ac701_proto_v1/proto_pcb)
- [AMD Artix 7 FPGA AC701 Evaluation Kit used on SatCat](https://www.xilinx.com/products/boards-and-kits/ek-a7-ac701-g.html#resources)
- [AC7200 FPGA SOM used on SCALES](https://www.en.alinx.com/Product/FPGA-System-on-Modules/Artix-7/AC7200.html)
- [Pin definitions for the FGB676 on the AC701](https://www.xilinx.com/content/dam/xilinx/support/packagefiles/a7packages/xc7a200tfbg676pkg.txt)
- [Pin definitions for the FBG484 on the SOM](https://www.xilinx.com/content/dam/xilinx/support/packagefiles/a7packages/xc7a200tfbg484pkg.txt)
- [AMD 7 Series FPGA Packaging and Pinout](https://docs.amd.com/v/u/en-US/ug475_7Series_Pkg_Pinout)

Steps: 

1. Look at the [SatCat proto PCB schematic](https://github.com/the-aerospace-corporation/satcat5/blob/main/examples/ac701_proto_v1/proto_pcb/Prototype%20schematic.pdf). The FPC connector on page 1 is what connects their proto PCB to the AC701 evaluation board. In a spreadsheet, copy the names of each signal bank letter and pin number used in the FMC connector for SatCat.
2. Find the [datasheet/user's guide](https://docs.amd.com/v/u/en-US/ug952-ac701-a7-eval-bd) for the AC701 evaluation board. Navigate to the "FPGA Mezzanine Card Interface" on page 58. There is a table in this section listing the schematic net names for the FMC connector. The first column is the bank letter and pin number (ex. C10) of the FPC connector and the last column is the corresponding pin on the FPGA chip (ex. G19) of the AC701 eval board. Add these to the sheet for each SatCat signal.

    ![FMC pin definitions](Images/FMC%20pin%20definitions.png)
    
    FMC pin definitions

    ![SatCat FMC pin signals](Images/satcat%20FMC%20pin%20signals.png)

    SatCat FMC pin signals

3. Take the [Pin definitions for the FGB676 on the AC701](https://www.xilinx.com/content/dam/xilinx/support/packagefiles/a7packages/xc7a200tfbg676pkg.txt) and add them to the spreadsheet in a different tab. Do the same for the [Pin definitions for the FBG484 on the SOM](https://www.xilinx.com/content/dam/xilinx/support/packagefiles/a7packages/xc7a200tfbg484pkg.txt). It may also help to combine these lists in their own tab to make cross-referencing easier. 
The spreadsheet used in SCALES is organized as follows:
- signals - main sheet to show and compare the signals between the AC701 eval board and the SCALES SOM
- chips - shows the pin definitions for each chip variation to cross-reference in the signals sheet.
- xc7a200tfbg484pkg - pin definitions of the SOM chip pulled from the txt file. for reference only.
- xc7a200tfbg676pkg - pin definition of the eval board chip pulled from the txt file. for reference only.

4. In the chips tab of the sheet, ctrl+f for the FMC pin name (ex. G19) and find the corresponding signal name for the FBG676 chip used on SatCat. Make sure to select “Match entire cell contents” and that you are searching the Value, not the Formulas in the Find and Replace options. Copy the signal name to the main sheet and repeat for all signals on the FMC.

    ![Find and Replace example](Images/find%20and%20replace.png)

    Find and Replace example

    ![Signal Matching example](Images/signal%20matching.png)

    Signal Matching example

5. Now, the same signal on the other chip will not match up to the same pin, so we have to ctrl+f for the signal name of each FMC/SatCat signal in the chips tab of the sheet in the SOM FBG484 column, then copy the pin and signal name name over to the signals sheet.
    - Disclaimer: the 484 chip does not have a bank 12, but the 676 does. For our purposes, I am replacing the signals used on bank 12 of the 676 FMC chip with the equivalent signals on bank 13 of the 484 SOM chip. I do not currently see an issue with this, but it is something to keep in mind if issues arise later.

    ![Find and Replace for SOM](Images/find%20and%20replace%20part%202.png)

    Find and replace for SOM

6. Once that is done, open the [SOM schematic](https://www.en.alinx.com/Product/FPGA-System-on-Modules/Artix-7/AC7200.html) (found in the "Documentations" section). Here you will see each bank of the SOM with the FPGA signal name, FPGA pin name, and the SOM signal name listed for each output. Ctrl+F for each FPGA pin on the SOM and copy down the matching SOM output signal name into the spreadsheet. These SOM output signals will be what the board-to-board (B2B) connector uses to communicate with the SCALES custom carrier board.

    ![Resulting spreadsheet](Images/final%20spreadsheet%20satcat.png)

    Resulting spreadsheet with the SOM signals on the right

Still working on what comes next. To be continued...