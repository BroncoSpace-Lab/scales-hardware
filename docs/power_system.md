# SCALES Power Distribution System Development

## Requirements:
- Supply Power to Sub-components on SCALES Carrier Board
   - Components to be used are still being finalized, but they are based on the following [SCALES Component Selection List](https://livecsupomona.sharepoint.com/:x:/r/sites/broncospacelab/Shared%20Documents/SCALES%20-%20General/Documentation/Hardware/SCALES%20Hardware%20Component%20Selection.xlsx?d=w0a79ca0a7f3241a1b204a2bc7466c9c3&csf=1&web=1&e=7n6GZM)
   - Standardize Power Input:
         * 28v power supply/battery input
   - Supply power levels to the following:
         * Flight Computer: IMX8/VOXL2
            - IMX8: 3.3v / 2A (Inrush Max) - Power (Max) = 3.63W //based on datasheet
            - VOXL2: 12v / 6A (Inrush Max) - Power (Max) = 12.75W //based on Janelle's testing
         * Edge Computer: Nvidia Jetson AGR ORIN
            - End-user must have access to peak power, so they can switch between performance modes.
            - AGR Orin: 20V / 3.75A
         * FPGA System: AMD Artix 7 XC7A200T
   

## Research Notes:
(1/28/25)
- Figure out each of the maximum power requirements for each of the components
- Start looking into how power distribution systems work for different power levels
   * Resources: 
      - [TI Application Notes](https://www.ti.com/lit/an/slva887/slva887.pdf?ts=1738125836647&ref_url=https%3A%2F%2Fwww.ti.com%2Fproduct%2FTPS22993)
      - [Basics Power MUX](https://www.ti.com/lit/an/slvae51a/slvae51a.pdf?ts=1738126325034&ref_url=https%3A%2F%2Fwww.ti.com%2Fproduct%2FTPS2115A)
      - [SmallSat Power Systems - NASA](https://www.nasa.gov/smallsat-institute/sst-soa/power-subsystems/#:~:text=Power%20storage%20is%20typically%20applied,control%20to%20spacecraft%20electrical%20loads.)

(1/29/25)
- Examples of existing EPS distribution boards
   * [IBEOS 150-Watt SmallSat EPS](https://www.ibeos.com/150w-eps-datasheet)
   * [Pumpkin EPSM1](https://www.pumpkininc.com/space/datasheet/710-01952-C_DS_EPSM_1.pdf)    




