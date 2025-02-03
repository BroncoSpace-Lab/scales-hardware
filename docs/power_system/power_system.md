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
   * [Skylabs NanoEPS 158W](https://www.nasa.gov/smallsat-institute/sst-soa/power-subsystems/#:~:text=Power%20storage%20is%20typically%20applied,control%20to%20spacecraft%20electrical%20loads.)

      - **Common Components in Power Boards**
      This list of components for the power board is based on information gathered from the various power systems above
         - *MCU*:
            - Power regulation monitoring
               - voltage and current measuring
            - Fault detection and protection
               - protects against shorts, overcurrent, brownouts and undervolting situations
               - can send system reset commands
            - OBC communication for power telemetry
               - integrate with IMX8/VOXL2
         - *Temperature sensors*:
            - Used to monitor connector/converter temps in case of high current cases
            - Some smallsat EPS use thermistors as a way to easily measure temperature, may be innacurate, or simple enough
         - *Watchdog*:
            - sends regular intervals to main components to check that they are operating correctly, IIRC only the jetson will have this
         - *Switching Regulator/(DC/DC) Buck Converter*:
            -Steps down voltage with minimal loss to respective subsystem (Uses capacitors, inductors and transistors to step down voltages using switching frequencies)
               - 28V -> 3v3/5v/12v @high current input/output
         - *RBF Pin/Kill Switch/Reset Switch*:
            - Some
         - *Fuse*: (probably not the best to use because if it blows, the whole system is inoperable until it comes back down)
            - Last bastion of defense in case of a current surge, protecting all components from frying (not likely to be used)

(1/30/25)
- [Basic Block Diagram of EPS board for SCALES Components](https://drive.google.com/file/d/1f2GgWEEMVI20wVgbpNZj8v8eEYCyvHen/view?usp=sharing)
   - Requirements: (Using Janelles Power Board Block Diagram for reference)
      - Microcontroller (Ex: STM32F091)
         - Checks Watchdog Timer
         - Measures current and voltage outputs
         - Utilizes CAN/I2C to relay current/voltage info to OBC
      - External Voltage in connector
         - start thinking about types of connectors that can handle high current input @ 24/28V inputs that are standard in aerospace/cubesat applications
      - Analog to Digital converters 
         - Used to measure voltage/current values and translate them to GPIO inputs for the MCU
      - Voltage Channel Buck Converters/Switching regulators
      - RBF System
         - Use relay to bypass high current/ MOSFET/FET?
      - Temperature Sensors for Buck/Switching Regulators
(2/3/25)
- First review on block diagram: [EPSBlock_V1](docs\Images\EPS_BlockV1.png)
   - Comments from Zach: 
      No need for MCU, Watchdog takes care of system stability based on timing pings
      Watchdog should be powered straight from the 28v stepped down to 5v/3v3
      Does Each voltage line needs to have voltage/current sensors? This is because it may be completely redundant, theres no point in adding a sensor that depends on the OBC to be running and even if what would it be doing?
      Stick to a simple design, we need to make sure the watchdog can operate independently from the 3 subsystems, just needs a switching regulator to function, then the pinged input from the OBC will stop the system from resetting.
      Because the watchdog will be what triggers the power cycle, we need to add some sort of system relay that triggers if a ping is not recieved.
   - Requirements: (More simple design, almost purely analog)
      - 28V V+/GND Connector
      - Relay/Switch system to be triggered by Watchdog (Normally Closed, Watchdog RST command triggers Open Switch)
      - Switching Regulators
         - 28v -> 20v (up to 8A) (Jetson)
         - 28v -> 24v/22v (Watchdog)
         - 28V -> 12v (OBC VOXL2/IMX8)
         - 28v -> 5v (FPGA Artix 7)
      - [Watchdog System](https://private-user-images.githubusercontent.com/61564344/374731107-10ac20a2-ea8e-4982-9281-9fe7444e7854.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3Mzg2MTExOTksIm5iZiI6MTczODYxMDg5OSwicGF0aCI6Ii82MTU2NDM0NC8zNzQ3MzExMDctMTBhYzIwYTItZWE4ZS00OTgyLTkyODEtOWZlNzQ0NGU3ODU0LnBuZz9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNTAyMDMlMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjUwMjAzVDE5MjgxOVomWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPTg3ZjEyZDBmOTkwYWZhOTM1N2NhZjU4NmU1ZmFkOTNkODBkMDVmYjcwODc0MjhlMjZlYTAxMWM5NjRiMzk3M2MmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0In0.Eo-wQk_yyZsGSnr17a3k5jSkEDWWzKaskMTcdA0J3aw)
         - Based on the TLV1704-SEP (Voltage input rated up to 24v)
         - No idea how it works but its what we will be using, schematic is complicated, linked above
- 2nd Block Diagram based on input above [EPSBlock_V2](docs\Images\EPS_BlockV2.png)
   - Comments from Zach: Simple, makes sense
   - Comments from Michael:
      
   

## To do ##
   (Week of 2/3/25)
   - First block diagram has been designed, refine design selection
   - Run through best design options for block diagram, how many current/voltage sensors should be used, what do other engineers think about how I intend to step down the input voltages, etc.
   - Start trade matrixes after the design for the block system has been made, from there the trade matrixes can start leading to sub-component selection and then initial schematic designs.
   - Consider INA219 Chip to measure both voltage and current draw




