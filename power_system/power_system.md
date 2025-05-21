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
 ![EPSBlock_V1](images/EPS_BlockV1.png)
   - Comments from Zach: 
      No need for MCU, Watchdog takes care of system stability based on timing pings
      Watchdog should be powered straight from the 28v stepped down to 5v/3v3
      Does Each voltage line needs to have voltage/current sensors? This is because it may be completely redundant, theres no point in adding a sensor that depends on the OBC to be running and even if what would it be doing?
      Stick to a simple design, we need to make sure the watchdog can operate independently from the 3 subsystems, just needs a switching regulator to function, then the pinged input from the OBC will stop the system from resetting.
      Because the watchdog will be what triggers the power cycle, we need to add some sort of system relay that triggers if a ping is not recieved.

- 2nd Block Diagram:
![EPSBlock_V2](images/EPS_BlockV2.png)
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
   - Make sure all capacitors are ceramic, electrolytic caps can explode in space
- Move to Rev B:
   - Implement the following:
       ## Switching regulators ##
            - [LT8638SEV] (https://www.mouser.com/ProductDetail/Analog-Devices/LT8638SEVPBF?qs=sGAEpiMZZMsMIqGZiACxIZbomz1DP27AbMqUs%252Bj26yi9VZ8WhNpLhw%3D%3D)
            (2.8V to 42V 10A/12A) Both should be the same
            - Left off on implementing footprint and symbol will implement design wednesday
            - Daisy Chain CLK implementation:
               The goal with this is to put each regulator (4) slightly out of phase with the others but all at the same switching frequency, this is to reduce input current ripple on each regulator
               PHMODE sets the phase angle output for the clock cycle, so the idea would be to set the first one to a PHMODE of 0, the next for a PHMODE of 90deg, then the next 180, and then 270 so well have 4 out of phase.
               - Pins:
                   - PHMODE: Tie to ground for single phase operation
                   - BIAS: Because output voltage is from 3.3v to 25v, datasheet says to tie it to Vout
                   - INTVcc: Internal 3.4V regulator bypass pin. Use a 1uf ESR capacitor from here to ground CLOSE to the ICEPS2
                     - 1uF capacitor
                   - BST: Used to provide a drive voltage, higher than the input voltage, supplies current to the topside power switch, us a 0.1uF boost capacitor
                     - 0.1uF capacitor
                   - SW: Outputs of the internal power switches. Tie them together and connect them to the inductor. For the PCB keep the nodes small and close together.
                     - Confirm inductor value
                   - GND: Place the gnd terminal of the input capacitor as close to the GND pins as possible. Try to use pins 29 and 32 for best thermal performance although they may be left disconnected.
                   - Vin: VIN pin supply current to the LT8638S internal circuitry and to the topside switch. To be tied together and be locally bypassed by a 4.7uF capacitor.
                     - 4.7uf capacitor
                     - Place the capacitor + side as close to the VIN pins, and the - side as close as possible to the GND pins
                  - EN/UV: Enable pin for the IC, you can use a voltage divider configuration from Vin to set a threshold voltage to program the IC to turn off if the voltage drops below this level. I will not be using this.
                  - RT: A resistor is tied between RT and ground to set the switching frequency
                     - Confirm switching frequency for output voltage and current
                  - CLKOUT: Output clock signal for PolyPhase Operation, in forced continuous, spread spectrum, and synchronization mode it outputs a 50% duty cycle square wave of the switching frequency. If in burst mode operation, the CLKOUT pin. If not use, let it float. I  wont be using this pin. NC
                  - Sync/Mode: Has four operating modes:
                     - Burst Mode: Tie to ground for this mode.
                     - Forced Continuous Mode, fast transient response and full frequency load over a wide load range, Float pin for this mode.
                     - Forced Continuous Mode with Spread Spectrum, allows for FCM but with a small modulation of the switching frequency to reduce EMI, if desired tie to INTVcc
                     - Synchronization Mode, drive it with a clock source to an external frequency, during the sync it will behave in forced continuous mode
                  - PG: Power good pin, remains low until the FB pin is within +-7.75% of the final regulation voltage. PG is valid when Vin is above 2.8V
                  - SS: Output tracking and a soft start pin, allows for regulation of the output voltage ramp rate. Documentation has a function that relates the input voltage to the pin to the rate at which the output voltage can increase by. I will not be using this as a load switch will take care of this. In order to disable this, set SS to higher than 1v, or may be left floating.
                  - FB: Feedback pin that sets the output voltage according to a provided formula in the data sheet
                  - Vc: Internal error amplifier, used to help stability in the output voltage, recommended by data sheet is a 10kohm resistor with a 1nF cap to ground.               
               - Jetson Config:
                  - Enable FCM:
                     - Leave Sync/Mode pin floating
                  - FB Resistor Network:
                     - Program the output voltage with a resistor divider, Jetson requires 20V.
                     - Formula: R1 = R2((Vout/.6v)-1)
                     - Calculated Values: R1 = 100kOhm, R2 = 3kOhm, Check calculations sheet to check.
                  - RT Resistor Selection:
                     - Resistor value that sets the switching frequency based on the following formula
                     - RT = (44.8/fsw) -5.9, RT is in Kohms
                     - Settled on 400Khz so RT about 105KOhms
                     - Calculations done on sheet on my ipad, will link here later
                  - Inductor Calculation and Selection:
                     - Requires about 4 different calculations including the frequency selection.
                     - Concluded with L >= 6uH with Irms >= 15A for safe operation
                  - Sync Pin has to be tied to external clock with phase shift (4 total so 0, 90, 180, 270)
               - OBC Config:
                  - Enable FCM:
                     - Leave Sync/Mode pin floating
                  - FB Resistor Network:
                     - Program the output voltage with a resistor divider, OBC requires 12V.
                     - Formula: R1 = R2((Vout/.6v)-1)
                     - Calculated Values: R1 = 95kOhm, R2 = 5kOhm, Check calculations sheet to check.
                  - RT Resistor Selection:
                     - Resistor value that sets the switching frequency based on the following formula
                     - RT = (44.8/fsw) -5.9, RT is in Kohms
                     - Settled on 400Khz so RT about 105KOhms
                     - Calculations done on sheet on my ipad, will link here later
                  - Inductor Calculation and Selection:
                     - Requires about 4 different calculations including the frequency selection.
                     - Concluded with L >= 6uH with Irms >= 13A for safe operation, with L calculation settled on 6.4uH
                  - Sync Pin has to be tied to external clock with phase shift (4 total so 0, 90, 180, 270)
               - FPGA Config:
                  - Enable FCM:
                     - Leave Sync/Mode pin floating
                  - FB Resistor Network:
                     - Program the output voltage with a resistor divider, FPGA requires 5V.
                     - Formula: R1 = R2((Vout/.6v)-1)
                     - Calculated Values: R1 =22kOhm, R2 = 3kOhm, Check calculations sheet to check.
                  - RT Resistor Selection:
                     - Resistor value that sets the switching frequency based on the following formula
                     - RT = (44.8/fsw) -5.9, RT is in Kohms
                     - Settled on 400Khz so RT about 105KOhms
                     - Calculations done on sheet on my ipad, will link here later
                  - Inductor Calculation and Selection:
                     - Requires about 4 different calculations including the frequency selection.
                     - Concluded L calculation settled on 2.9uH so 3uH should work fine
                  - Sync Pin has to be tied to external clock with phase shift (4 total so 0, 90, 180, 270)
               - +5v Peripheral Config:
                  - Enable FCM:
                     - Leave Sync/Mode pin floating
                  - FB Resistor Network:
                     - Program the output voltage with a resistor divider, FPGA requires 5V.
                     - Formula: R1 = R2((Vout/.6v)-1)
                     - Calculated Values: R1 =22kOhm, R2 = 3kOhm, Check calculations sheet to check.
                  - RT Resistor Selection:
                     - Resistor value that sets the switching frequency based on the following formula
                     - RT = (44.8/fsw) -5.9, RT is in Kohms
                     - Settled on 400Khz so RT about 105KOhms
                     - Calculations done on sheet on my ipad, will link here later
                  - Inductor Calculation and Selection:
                     - Requires about 4 different calculations including the frequency selection.
                     - Concluded L calculation settled on 2.9uH so 3uH should work fine
                  - Sync Pin has to be tied to external clock with phase shift (4 total so 0, 90, 180, 270)
      ## Load Switch ##
         - [LTC4375] (https://www.analog.com/media/en/technical-documentation/data-sheets/LTC4365.pdf)
            - Requires: [SISB46DN-T1-GE3] (https://www.vishay.com/docs/76655/sisb46dn.pdf)
            - ^ Dual channel 40v MOSFET
            - Has an active high pin that when triggered low sets the current output to be very low, labeled SHDN pin
            - Has overvoltage and undervoltage pins that set safety levels for OV and UV which can be configured by voltage dividers. Will configure +-2V for each component. More on this below
            - SHDN Config -> Tied to Watchdog output, triggered high when everything is working correctly and when set low the load switch disables the load
            - No utilizing fault output

       ## Clock ##
         - [LTC6902] (https://www.analog.com/en/products/ltc6902.html) 
            - Allows for setting phase outputs as well as resistor programmable frequency timings
               - Set Phase Mode: 4 Phase connect PH to V+, set N to floating which is 10 due to frequency range of 5Khz <= 400Khz <= 500Khz
               - Since SSFM (Spread Spectrum Frequency Modulation)
               - Review calculations page in EPS-> Calculations folder to verify with datasheet instructions.
               - Phases set:
                  - 5v Perif -> 0deg
                  - FGPA -> 90 deg
                  - OBC -> 180 deg
                  - Jetson -> 270 deg
      ## Current/Voltage Sensors ##
         - [INA230](https://www.ti.com/lit/ds/symlink/ina230.pdf?ts=1739195723292)
         - Shunt resistors have really low resistance to allow for high currents on each line, Vbus is also used to measure voltage. Bear in mind each shunt resistor should have a   relatively high power dissipation rating to prevent it from burning up.
   - Notes for RevB:
      - Add decoupling capacitors to input voltage lines
  To Do (2/19/25):
   - Add soft start capacitors to every switch regulator so that they delay long enough for the clock to be powered first
      - Each is given a 25nF Soft Start capacitor to take 10ms to fully turn on the regulator, this allows the clock to be powered first by setting a shorter slew rate time therefore setting them all out of phase before they fully turn on. [DONE]
   - Start calculating load switch resistors and capacitor values [DONE]
   - Pick and integrate 28v to 5v buck converter for clock [DONE]
   - Add decoupling capacitors to input voltage lines [DONE]

   - Notes (2/19/25) 
      - Filter caps added at power input
      - Notes are written for everything, calculations are in the EPC calculations folder. Rev B schematic DONE. 
      - Next phase is review with Michael and then ERC cleaning. 

      =======
   ## EPS REV.B Review Notes ##
    - https://scales-hardware.readthedocs.io/en/latest/meeting_notes/feb_21_25/
    - REV C -> Satcat dropped moving to new FPGA board
    - Make an updated Block diagram to reflect what the schematic is without getting too complicated

   ### To do 2/25/25
   - Rev C: (Make new block diagram)
      - Power Input Spec:
         - Define power input requirement
         - How low is undervoltage?
            - Jetson:
               - Soft start capability?
               - Idle current
               - Max operational average current
               - Max transient current
      - Load Switches: (Need a new selection)
         - Go BEFORE switching regulators
         - Need current sensing capability
         - Add slew rate caps for turn on time
      - Switching Regulators:
         - Verify Rt sets clock when power drops
         - Power on first, then worry about syncing
      - Clock: (Need a new selection)
         - Switch to a crytal oscillating clock instead of what is current chosen

   ## Week of 3/3 ## 
      - Goals:
         - Complete Block Diagram of Rev C [DONE]
         (LINK HERE)[https://drive.google.com/file/d/1UQ95z8zSc4xQTobpHLgdMU17UKOtdfAZ/view?usp=sharing]
         - Finalize Rev C Requirements:
            - Interface requirements for OBC/Jetson/Peripheral Board
               - Gecko Connectors?
            - Power: The EPS system will supply 4 switching regulators with adecuate voltage and current:
            - Load Switch Configuration: 
               - Each subsystem has a load switch with a rst toggle, all are handled by the OBC, as well as the watch dog timer. Meaning both can be pulled down depending on the state.
               - Watchdog system has a load switch which in the case of a watch dog failure, indicated by the I/V sensor can reset the timing for ensured redundancy
            - Switching Regulator Configuration:
               - Jetson: 28v to 20v @ 6A MAX
                  - Start in sync with OBC
               - OBC: 28v to 5v @ 3A MAX
                  - Start in sync with Jetson
               - Perif: 28v to 5v @ 2A Max
                  - Start Early to boot up smaller components
               - Watchdogs and Clock: Powered by OBC, boots first
               Startup sequence:
               OBS powers on first due to shortest SS. Clock turns on, sets frequency and phase, then it pulls the Enable pins on the load switches high and then the Jetson and Perif Load switch turn on. Then I2c Sensors are powered on for each and data is starting to be read for each subsystem.
               
         ## Modifications for this to happen ## 
            - Set SS capacitor for OBC/Jetson/Perif Load Switch to cause slightly longer turn on time compared to Clock and WDs
            - WD intial boot capacitor must be set for longer time to allow for proper system boot sequence
            - Software on OBC should allow for when the WDs are to be reset, to pull its own RST pin for the load switch high, so on reset the watch dogs do not false trigger the reset on the OBC load switch
            - Need to implement I2C Program to monitor incoming I/V sensor signals for each subsystem

         - Finalize Rev C Component Selection:
            - Edge Computer: Jetson Orin AGX (20V/6A Out)
            - Flight Computer: IMX8 (Have a 5v/3A Out)
            - Peripheral Board: N/A (Have a 5v/2A Out)

            - Capacitors: SMD Ceramics
            - Resistors: SMD Wire Wounds
            - Inductors: SMD No-Air Ceramic
         
            - Load Switche(s): [LTC4365](https://www.analog.com/media/en/technical-documentation/data-sheets/LTC4365.pdf)
               - Require: [SISB46DN-T1-GE3](https://www.vishay.com/docs/76655/sisb46dn.pdf)
            - Switching Regulator(s): [LT8638SEV#PBF](https://www.mouser.com/ProductDetail/Analog-Devices/LT8638SEVPBF?qs=sGAEpiMZZMsMIqGZiACxIZbomz1DP27AbMqUs%252Bj26yi9VZ8WhNpLhw%3D%3D)
            - Clock: [LTC 6902](https://www.analog.com/media/en/technical-documentation/data-sheets/6902f.pdf) (Use SSFM)
            - I/V Sensor(s): [INA230](https://www.ti.com/lit/ds/symlink/ina230.pdf?ts=1739195723292)

            
         - Complete Rev C Schematic:
         
         - Begin EPS Board Layout:
            PCB Notes:
               -JLC PCB has specific requirements for manufacturing be sure to check everything needed to make sure it is correct.
               Check Here[https://jlcpcb.com/capabilities/pcb-capabilities]

               -Each component has specific design constraints so make sure that the component is following its pcb layout design spec

               -How should I go about routing?
                  - So far ive just been connecting nets together and when I cant make a connection from the top plane im just using vias to connect things together through the planes. Not entirely sure if this is how it should be done but it seems to be working so far.
                  - Latest problem is the SIS mosfet which the pads wont let me do connections to easily

            - Rev C Dev Board Notes 3/26/25
               - Ran into some bullshit github issues to be honest I have no fucking clue what the problem is but thankfully I was able to remove the problem by nuking my branch, and pulling from main again and then merging it to main. 
               - Redid layout for all the components, im happy with what i have right now. Going to go over this and start routing from the beginning. What I will try is doing the jetson subsystem and then trying to copy the same tracks and use multichannel
               - I also removed the XNOR configuration because it doesnt make sense to add it. Sure it can be useful but on a boot sequence its useless because it can throw me into a infinite boot loop essentially a race condition because its trying to turn on faster than it is triggered off by the first system to power on

         ## Week of 3/31/25 ##
            -REV C Dev Board To Do's 3/31/25
               - Gecko Connectors swap to DF11-16DP-2DSA(08) Connector [DONE]
                  - Make sure the footprint has the through holes
               - Add meme to the front of the board
               - Add schematic layout to the back of the board for dev purposes
               - Test WD kit Michael made on the scope, test with different voltage ranges

            -REV C Dev Board Revision Suggestions from Andrew Greenberg (4/1/25)
               Main Concerns:
               - Stop working on layout so you dont waste your time [DONE]
               - Rename +28V as Vbatt and display voltage ranges (Vbatt = [24.0, 28.8, 33.6]) [DONE]
                  -Reconfigure the UV and OV numbers to allow for this voltage range [DONE]
                     - Modify in the calculations page (be sure to note the numbers) 
               - 100nF caps on +28V inputs can be removed due to 10uF [DONE]
               - Ensure the capacitors are +75v rated so they dont explode at the input side [DONE]
                  -Must be ceramic [DONE]
                  -Must be around double of what the peak input voltage is for safe margins [DONE]

               SISB46DN: [DONE]
                  -Replace symbol with 2 MOSFETS so that the end user can understand what the schematic actually represents
               INA230:
                  -Calculate proper resistor values for the INA230 voltage and current detection [DONE] (In rev c calcs last page)
                  -Remove 4.7k resistors on the INA230 side, you want to include them on the host side, be sure to add these notes in the schematic [DONE]
         
               -Physical Design Revision Notes:
                  -For each component go to its manufacturing page and make note of all the component types that are used for the individual designs, this means that the devices that are used for each are recommended by the manufacturer in the evaluation board.

                  -Replace the footprints for all the inductors to ensure they have the proper current ratings and also have proper spacing for additions to it on the board.

                  Manufacturer recommended components around subsystem comcponents: [DONE]
                     LTC4365:
                           -No hard restrictions on component selection for those that will go around it
                           -Worth adding test points to the UV/OV side to see the values it should trip at
                     LS8638SEV: 
                           -Use +50v rated ceramic caps, 3225 metric packages
                           -Use this kind of inductor: https://www.coilcraft.com/en-us/products/power/shielded-inductors/molded-inductor/xel/xel6030/?srsltid=AfmBOoqxe6hS-AVIr3woqGFRZjt3QaXyQi86MwGE0zvqKz2AENsjz4RA
                           this is the one referenced in the evaluation board. Having trouble finding the footprint for the right sizing
                     INA230: No specifics on what caps or resistors to use, go with ceramic caps and carbon/ceramic resistors as usual
                     LTC6902:
                           -No strict requirements on what type of components are used with it

                Watchdogs:
                  -Probe C19 and C20 as well as the 100k resistor.
                  -Worth doing circuit analysis on to make out mathematically what is going on in the circuit
               
               Extra Stuff:
                  -For the OBC SDA/SCL, we need to include an i2c buffer to ensure proper signaling, for this we can use the    LTC4300A


# SCALES EPS REV C Design Development #
* By Luca Lanzillotta
* 4/15/2025
* 4/16/2025

## Abstract ## 
   - The SCALES EPS Board is a power distribution board designed with the intention of powering a flight computer, a machine learning edge computer, and a peripheral system for onboard diagnostics and user defined sensors to interface with both the flight computer and the edge computer. Each subsystem is equipped with an error mitigation feature known as a watchdog, which is pet by each corresponding subsystem and serves as a reset point for each subsystem. The flight computer has access to power diagnostics for each subsystem and can be is software programmable of its direct control of each subsystems operation mode.

   ### Component Selection ###
   **Primary Components:** 

   - IV Sensor: 
   [INA230](https://www.ti.com/lit/ds/symlink/ina230.pdf?ts=1744680590205&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FINA230)

   - Switching Regulator: 
   [LT8638SEV](https://www.mouser.com/ProductDetail/Analog-Devices/LT8638SEVPBF?qs=sGAEpiMZZMsMIqGZiACxIZbomz1DP27AbMqUs%252Bj26yi9VZ8WhNpLhw%3D%3D)

   - Load Switch:
   [LTC4365](https://www.analog.com/media/en/technical-documentation/data-sheets/LTC4365.pdf)

   - Dual-Gate Mosfet: 
   [SISB46DN-T1-GE3](https://www.vishay.com/docs/76655/sisb46dn.pdf)

   - Clock: 
   [LTC 6902](https://www.analog.com/media/en/technical-documentation/data-sheets/6902f.pdf)

   - Power Connector: 
   [XT-60](https://outofdarts.com/products/xt-60-connector-90-degree-connector?srsltid=AfmBOoqKylA6kCYljerReYkHtwbVrW2sa4yj6tmw-vBZRMtz7rP4LZ-8)

   - Watchdog Comparator: 
   [TLV-1704SEV](https://www.ti.com/lit/ds/symlink/tlv1704-sep.pdf?ts=1744728353106&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTLV1704-SEP%252Fpart-details%252FTLV1704AMPWPSEP)

   **Secondary Components:**

   **Resistors:**
   - (0603):
      - Load Switch (UV/OV, SHDN Pull Down)
      - Switching Regulator (Vc, Rt, Bias, Fb)
      - Clock (Mode set, Rt set)
      - Watchdog (WDT-Vin, every block within the system)
   - (2512):
      - IV Sensor Shunt (IN+ to IN-)

   **Capacitors:**
   
   - (1210) +75V:
      - Input Power Connector (Filter caps)
      - Load Switch Output (Filter caps)
      - Switching regulator (BST to SW, BIAS to FB, SW to INA IN+)

   **Inductors:**

   - Per recommendation of the LT8638SEV evaluation board design the recommended inductors are the following
      - [XEL6030](https://www.coilcraft.com/getmedia/ed8d0314-5cbb-4be3-85da-32235c3701c8/xel6030.pdf)
      - Switching regulator (BST to SW)
      - Rated from 0v to 80v
      - AEC-200 rated (automotive standard)
      - IRMS ratings are more than reasonable based on REV C calculations requirements
      - Refer to layout requirements in the datasheet


   ### Requirements ###

   * Power: (XT-60 Connector)
      - 28V/12A Max Input
      - 3 Subsystems, OBC, Edge Computer, Peripheral
      - Existing calculations for inductors and input power are overspecked

   * Load Switches: (LTC4365)
      - Dual Gate Mosfet (SISB46DN-T1-GE3) is utilized to pass the output based on the inputs of the load switch IC
      - SHDN feature is held by the WD and/or the flight computer depending on the subsystem
      - UV/OV thresholds are adapted to the input ratings set by LiPO operating ranges, each subsystem has a calculation in the REV C calculations in this subfolder.
      - Soft Start capacitors for each subsystem mitigate current spikes at the input side when initializing, they also serve as start up delays

   * Clock: (LTC6902)
      - Set to 400khz based on average performance metrics for the switching regulator(s)
      - Using SSFM to reduce system wide noise of a specific signal
      - 3 Phases used to seperate switching frequency timing across each regulator

   * Switching Regulators: (LT8638SEV)
      - Switching frequency set by clock, phase set by clock
      - In the case of clock failure, a frequency setting resistor on the switching regulator will maintain a constant switching frequencyIn the case of clock failure, a frequency setting resistor on the switching regulator will maintain a constant switching frequency

   * Watchdog(s): (TLV1704SEV)
      - Watchdogs for each subsystem
      - OBC controls which subsystem is on, OBC is always on
      - Watchdogs are PET every ~30s by their respective subsystem
      - Utilizing an adapted version of the OreSAT design provided by Andrew Greenberg [Design HERE](https://github.com/BroncoSpace-Lab/scales-hardware/pull/280)
      - NOTE: The current watchdog implementation is currently not operating as expected, refer to testing notes.md in this folder for more information on progress    

### Rev. C Block Diagram ###

![REVC](../docs/Images/EPS_REVC_BlockDiagram.png)

### Design Revision Notes ###
- Use the JLCPCB Component stock checker plugin 

- Fix Dual Gate Mosfet input and output pins Vin and Vout are references for the Load Switch [FIXED]

- Add a resistor to the WD_Toggle so when the WD pulls low it doesnt short the GPIO pin on the subsystem GPIO [FIXED]

- Set all caps to be +75v rated [FIXED]

- Set the shunt resistor to 10mohms [FIXED]
