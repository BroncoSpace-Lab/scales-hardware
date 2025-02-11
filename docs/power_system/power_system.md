# SCALES Power Distribution System Development

## Quick Links:
 - Find parts from major distributors: https://octopart.com/
By Luca Lanzillotta

## Requirements:
Supply Power to sub-components on SCALES Carrier Board

## Components to be used:
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

## Block Diagrams/Basic Design Ideas:
### (1/30/25)
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

### (2/3/25)
- 1st Block Diagram:
 ![EPSBlock_V1](images\EPS_BlockV1.png)
   - Comments from Zach: 
      No need for MCU, Watchdog takes care of system stability based on timing pings
      Watchdog should be powered straight from the 28v stepped down to 5v/3v3
      Does Each voltage line needs to have voltage/current sensors? This is because it may be completely redundant, theres no point in adding a sensor that depends on the OBC to be running and even if what would it be doing?
      Stick to a simple design, we need to make sure the watchdog can operate independently from the 3 subsystems, just needs a switching regulator to function, then the pinged input from the OBC will stop the system from resetting.
      Because the watchdog will be what triggers the power cycle, we need to add some sort of system relay that triggers if a ping is not recieved.

- 2nd Block Diagram:
![EPSBlock_V2](images\EPS_BlockV2.png)
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

   - Comments from Zach: Simple, makes sense
   - Comments from Michael:
        - Watchdog system for each logic system, to trigger each individual line rather than the whole system
        - To trigger the reset, dont use relays, find some sort of solid state system 
        - Power sequencing to turn on each section individually, inrush current will be too high if we turn them on all at once
         - delay timer to sequence each section



## Research Notes:

### (1/28/25)
- Figure out each of the maximum power requirements for each of the components
- Start looking into how power distribution systems work for different power levels
   * Resources: 
      - [TI Application Notes](https://www.ti.com/lit/an/slva887/slva887.pdf?ts=1738125836647&ref_url=https%3A%2F%2Fwww.ti.com%2Fproduct%2FTPS22993)
      - [Basics Power MUX](https://www.ti.com/lit/an/slvae51a/slvae51a.pdf?ts=1738126325034&ref_url=https%3A%2F%2Fwww.ti.com%2Fproduct%2FTPS2115A)
      - [SmallSat Power Systems - NASA](https://www.nasa.gov/smallsat-institute/sst-soa/power-subsystems/#:~:text=Power%20storage%20is%20typically%20applied,control%20to%20spacecraft%20electrical%20loads.)

### (1/29/25)
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

### (2/3/25) - (2/10/25)
- EPS:
   - 28V Power Input
      - Connectors:
         - 
   - Load Switches [INFO](https://www.ti.com/lit/an/slva652a/slva652a.pdf?ts=1738793997138&ref_url=https%253A%252F%252Fwww.google.com%252F)
      - Load switches come AFTER switch regulators
         - [Load Switch](https://www.ti.com/lit/ds/symlink/lm7310.pdf?ts=1738872017422&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLM7310)
            - TILM73100 
            - Going with ths one for now because of the high current and voltage rating, has integrated current sensing, over and undervoltage protection, and an enable pin. Also has an analog current sensor.

   - Switching regulator [Vicor PI3740-00](https://www.vicorpower.com/documents/datasheets/ds_pi3740-00.pdf) (this is probably insanely fucking overkill, but better safe than sorry)
      - Using one for each subsystem, they are about 12 dollars each, and have very high efficiency ratings, they also have the majority of components already integrated.
         - Datasheet has recommended capacitors for specific voltage ranges as well as calculation sheet for inductor selection
      - 28v -> 5v (Watchdogs)
      - 28v -> 20v (Jetson)
      - 28v -> 12v (OBC)
      - 28v -> 5v (FPGA)
   - Voltage/Current monitor
      - [INA230](https://www.ti.com/lit/ds/symlink/ina230.pdf?ts=1739195723292)
         -36V, Voltage and Power sensor, I2C communication
   - Watchdog(s)
      - Need 3, one for Jetson, OBC, FPGA
         - Use Proves implementation available [here](https://github.com/BroncoSpace-Lab/scales-rad-tolerant-watchdog?tab=readme-ov-file)
   - Watchdog for each voltage level instead of the full system

### (2/11/25)
- Notes on Rev A from michael:
   - Switching Regulator: Vicor PI3740 is not in stock, find another one, stick to TI or Analog devices
   - Add a GPIO pin to toggle the enable pin on each regulator, if its needed to reset power output
- Move to Rev B:
   - Implement the following:
      - New Switching regulator
         - Options:
            - [LT8638](https://www.analog.com/media/en/technical-documentation/data-sheets/lt8638-2.pdf) (2.8V to 42V 10A/12A)
            - [LT8638SEV#PBF](https://www.mouser.com/ProductDetail/Analog-Devices/LT8638SEVPBF?qs=sGAEpiMZZMsMIqGZiACxIZbomz1DP27AbMqUs%252Bj26yi9VZ8WhNpLhw%3D%3D) (2.8V to 42V 10A/12A) Both should be the same
      - Current/Voltage Sensors
      - Standalone 5V line for peripherals
      - GPIO Enable pins for Switching regulators
      - Start calculating load switch resistors and capacitor values
#### some issues or concerns with this:
  



