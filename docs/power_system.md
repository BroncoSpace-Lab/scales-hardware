# SCALES Power Distribution System Development

## Requirements:
Supply Power to Sub-components on SCALES Carrier Board

## Components to be used
Components to be used are still being finalized, but they are based on the following [SCALES Component Selection List](https://livecsupomona.sharepoint.com/:x:/r/sites/broncospacelab/Shared%20Documents/SCALES%20-%20General/Documentation/Hardware/SCALES%20Hardware%20Component%20Selection.xlsx?d=w0a79ca0a7f3241a1b204a2bc7466c9c3&csf=1&web=1&e=7n6GZM).
- **Flight Computer: IMX8/VOXL2**
  - **IMX8:** 
      - 3.3V / 2A (Inrush Max) - Power (Max) = 3.63W *(based on datasheet)*
  - **VOXL2:** 
      - 12V / 6A (Inrush Max) - Power (Max) = 7W *(based on Janelle's testing; check datasheet for idle values)*

- **Edge Computer: Nvidia Jetson AGX Orin**
  - End-user must have access to peak power, so they can switch between performance modes.
  - **AGR Orin:** 
      - 20V / 3.75A (MAX) - Power (Max) = 75W *(based on max current/voltage ratings)*

- **FPGA System:**
    **AMD Artix 7 XC7A200T SOM**
      - 5V / 2A (MAX) - Power (Max) = 10W *(based on datasheet and max utilization data)*

## Standardize Power Input
- **28V power supply/battery input**
  - Based on information compiled above *(Does this include system peripherals?)*:
    - **OBC:** 3.3V @ 2A Max (IMX8) / 12V @ 6A Max (VOXL2)
    - **EC:** 20V @ 3.75A Max (Jetson AGX)
    - **FPGA:** 5V @ 2A Max (Artix 7 SOM)
   

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
   * [GOMSPACE NanoPower P31u](https://gomspace.com/UserFiles/Subsystems/datasheet/gs-ds-nanopower-p31u-32_(1).pdf)
   * [ISISPACE ICEPS2](https://www.isispace.nl/wp-content/uploads/2019/04/ISIS-ICEPS2-DSH-0001-ICEPS2_Datasheet-01_02.pdf)

      - **Common Components in Power Boards**
         - *MCU*:
            - Power regulation monitoring
               - voltage and current measuring
            - Fault detection and protection
               - protects against shorts, overcurrent, brownouts and undervolting situations
               - can send system reset commands
            - OBC communication for power telemetry
               - integrate with IMX8/VOXL2
         -*Temperature sensors*
            -Used to monitor connector/converter temps in case of high current cases
         - *Watchdog*
            - sends regular intervals to main components to check that they are operating correctly, IIRC only the jetson will have this
         - *Switching Regulator/(DC/DC) Buck Converter*
            -Steps down voltage with minimal loss to respective subsystem (Uses capacitors, inductors and transistors to step down voltages using switching frequencies)
               - 28V -> 3v3/5v/12v @high current input/output
         -*Fuse* (probably not the best to use because if it blows, the whole system is inoperable until it comes back down)
            -Last bastion of defense in case of a current surge, protecting all components from frying (not likely to be used)





