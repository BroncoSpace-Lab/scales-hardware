# Testing Procedure Script
This document will create a mock DRM test to run on both the VOXL2 and IMX8. This document also includes individual component testing and system level testing according to the DRM.

## Mock DRM
sensors:
- SPI: HUMIDITY SENSOR
- I2C: TEMP SENSOR
- UART: IMU
- Ethernet: CAMERA
- USB: CAMERA
- CAN: PHOTORESISTIVE SENSOR
- GPIO: WATCHDOG
  
Mock Mission:
- FILL IN

FC Tasks:
- constantly monitor atmosphere using I2C, SPI, UART


## FC Individual Testing
### 1. Communication Testing
> <ins>Objective:</ins> We will be verifying each communication protocol individually by taking in sensor data and writing to memory. We will then perform the same test, but with all protocols running at the same time. We will be keeping note on runtime, bandwidth and power draw.
#
 <ins>Test Equipment:</ins>
 - Flight Computer (IMX8 dev board / VOXL2 dev board)
 - Sensors for each communication protocol (SPI, I2C, UART, Ethernet, USB, CAN, GPIO)
 - variety of jumper wires
#
<ins>FPrime Component and Commands (need to be developed):</ins>
- Component: REALTIME_DATA_PROCESSING
   - Description: This component will have commands that take data from sensors and write it to the flight computer's memory for 30 seconds
    - Commands: (individual commands to start each protocol for 30 sec, and one multi-communication command for 30 sec)
      - SPI_START
      - I2C_START
      - UART_START
      - ETHERNET_START
      - USB_START
      - CAN_START
      - GPIO_START
      - ALL_START
    - Telemetry Channels:
        - sensorState
        - sensorValue
        - runtime
        - bandwidth
        - powerDraw
#
<ins>Pass/Fail Criteria:</ins>

| Test        | FAIL        | PASS        | yes/no?     |
| ----------- | ----------- | ----------- | ----------- |
| Data transfer| doesnt write to memory| writes to memeory| ? |
| individual functionality|not every protocol works|every protocol works|?|
|multi-sensor functionality| no/partial functionality | full functionality |?|
|runtime| | |?|
|bandwidth| | |?|
|power draw| | |?|

#
> <ins>Procedure:</ins>
> - [ ] wire all sensors to the filght computer
> - [ ] run REALTIME_DATA_PROCESSING FPrime deployment and start GDS
> - [ ] run SPI_START command
> - [ ] Measure runtime, bandwidth, powerdraw, etc.
> - [ ] verify data was written into memory
> - [ ]  Repeat this process to each command listed below:
>   - [ ] I2C_START: TEMP SENSOR
>   - [ ] UART_START: IMU
>   - [ ] Ethernet_START: CAMERA
>   - [ ] USB_START: CAMERA
>   - [ ] CAN_START: PHOTORESISTIVE SENSOR
>   - [ ] GPIO_START: WATCHDOG
>   - [ ] ALL_START: starts all sensor data collection
#
### 2. Power Testing
> <ins>Objective:</ins> We will be observing different power states of both flight computers (high, middle, low, overclocked, underclocked, idle) to obtain a better understanding of how much power each flight computer will generally use. To do this, we will be using cpuburn to activate different percentages(100%, 50%, and 10%) of the cpu's cores and the power consumption at each percentage. For over and under clocking, we will manually set the clockspeed of the cpu to (clockspeed-0.4GHz) for underclocking and (clockspeed+0.2GHz) for overclocking. For the idle state, we will turn flight computer on with no tasks running and observe power consumption.
#
 <ins>Test Equipment:</ins>
 - Flight Computer (IMX8 dev board / VOXL2 dev board)
#
<ins>Pass/Fail Criteria:</ins>

| Test        | FAIL        | PASS        | yes/no?     |
| ----------- | ----------- | ----------- | ----------- |
| high power state | over # | under # |?|
| low power state | over # | under # |?|
| underclocking | unresponsive system | responsive system |?|
| overclocking | unresponsive system | responsive system |?|
| idle | over # | under # |?|

#
> <ins>Procedure:</ins>
> - [ ] download cpuburn on the flight computer
> - [ ] Test high power draw by running ./cpuburn and record power draw
> - [ ] Test medium power draw by running ./cpuburn -n 2 and record power draw
> - [ ] Test low power draw by running ./cpuburn -n 1 and record power draw
> - [ ] Test overlocking by setting clock speed to (clockspeed+0.2GHz) and record power draw
> - [ ] Test underclocking by setting clock speed to (clockspeed-0.4GHz) and record power draw
> - [ ] Test Idle by turning flight computer on with no tasks running and record power draw
#
### 3. Watchdog Testing
> <ins>Objective:</ins> We will be verifying the functionality of an external watchdog timer circuit with our flight computers. We will observe what the WDT does at 5 seconds, 5+0.2 seconds, and 5-0.2 seconds to ensure that the WDT is resetting at 5sec properly.
#
 <ins>Test Equipment:</ins>
 - Flight Computer (IMX8 dev board / VOXL2 dev board)
 - external watchdog timer circuit
 - jumper wires
#
<ins>FPrime Component and Commands (need to be developed):</ins>
- Component: WATCHDOG
   - Description: This component will serve as the trigger for an external watchdog circuit.
    - Commands: (GPIO on/off timers)
      - GPIO_ON (turns gpio pin on)
      - GPIO_OFF (turns gpio pin off)
      - GPIO_ON_5SEC (toggles gpio pin ever 5 seconds)
      - GPIO_ON_4.8SEC (toggles gpio pin ever 4.8 seconds)
      -GPIO_ON_5.2SEC (toggles gpio pin ever 5.2 seconds)
    - Telemetry Channels:
        - gpioState
#
<ins>Pass/Fail Criteria:</ins>

| Test        | FAIL        | PASS        | yes/no?     |
| ----------- | ----------- | ----------- | ----------- |
| 5sec cycle|WDT does reset FC|WDT does NOT reset FC|?|
|4.8sec cycle|WDT does reset FC|WDT does NOT reset FC|?|
|5.2sec cyle|WDT does NOT reset FC|WDT does reset FC|?|

#
> <ins>Procedure:</ins>
> - [ ] Attach watchdog circuit to FC
> - [ ] run WATCHDOG FPrime deployment and start GDS
> - [ ] run the command GPIO_ON 
> - [ ] Wait 15 seconds and verify FC stays ON
> - [ ] run the command GPIO_OFF
> - [ ] wait and verify FC RESETS (this should be about 5 seconds)
> - [ ] run the command GPIO_ON_5SEC
> - [ ] wait 15 seconds and verify FC stays ON
> - [ ] run the command GPIO_ON_4.8SEC
> - [ ] wait 15 seconds and verify FC stays ON
> - [ ] run the command GPIO_ON_5.2SEC
> - [ ] wait 10 seconds and verify FC RESETS
#

### Jetson
### 1. Power Testing
> <ins>Objective:</ins> We will be observing different power states of the Jetson AGX Orin (high, middle, low, overclocked, underclocked, idle) to obtain a better understanding of how much power it will generally use. To do this, we will use different percentages of gpu burn to test high, middle, and low power states. For over and under clocking, we will manually set the clockspeed of the cpu to (clockspeed-0.4GHz) for underclocking and (clockspeed+0.2GHz) for overclocking. For the idle state, we will turn the Jetson on with no tasks running and observe power consumption.
#
 <ins>Test Equipment:</ins>
 - Jetson AGX Orin
#
<ins>Pass/Fail Criteria:</ins>

| Test        | FAIL        | PASS        | yes/no?     |
| ----------- | ----------- | ----------- | ----------- |
| high power state | over # | under # |?|
| low power state | over # | under # |?|
| underclocking | unresponsive system | responsive system |?|
| overclocking | unresponsive system | responsive system |?|
| idle | over # | under # |?|

#
> <ins>Procedure:</ins>
> - [ ] download gpu burn on the Jetson (https://github.com/wilicc/gpu-burn)
> - [ ] open Jetson Power GUI to obtain power draw in the duration of this test
> - [ ] Test high power draw by running 'gpu_burn -m 100' and record power draw
> - [ ] Test medium power draw by running 'gpu_burn -m 50' and record power draw
> - [ ] Test low power draw by running 'gpu_burn -m 10' and record power draw
> - [ ] Test overlocking by setting clock speed to (clockspeed+0.2GHz) and record power draw
> - [ ] Test underclocking by setting clock speed to (clockspeed-0.4GHz) and record power draw
> - [ ] Test Idle by turning Jetson on with no tasks running and record power draw
#

## First System Level Test (FC and Jetson Only)
### 1. FC to Jetson Watchdog Timer Test
> <ins>Objective:</ins> We will be verifying that the FC is able to act as the Jetson's watchdog timer. This will be done by connecting a GPIO pin from the FC to the Jetson, which will have the Jetson send a pulse every 5 seconds to the FC to reset a digital counter on the FC. The Jetson will also be connected to the FC through its reset pin, which allows the FC to reset the Jetson if the Jetson stops sending a pulse through the GPIO pin. This test will test the case when the Jetson stops sending a pulse, which will force the FC to reset the Jetson.
#
 <ins>Test Equipment:</ins>
 - Flight computer (IMX8 dev board / VOXL2 dev board)
 - Jetson
 - variety of jumper wires
#
 <ins>FC and Jetson Scripts: (need to be developed)</ins>
 - The FC will have a script that will have a counter. One the counter reaches 5 seconds, it will pull the reset button on the Jetson high to reset the Jetson. This counter will reset to zero when the FC recieves a pulse from the GPIO pin from the Jetson.
 - The Jetson will have a script that will send a pulse through the GPIO pin every 4 seconds.
#


<ins>Pass/Fail Criteria:</ins>

| Test        | FAIL        | PASS        | yes/no?     |
| ----------- | ----------- | ----------- | ----------- |
| 5sec cycle|FC does reset Jetson|FC does NOT reset Jetson|?|
|4.8sec cycle|FC does reset Jetson|FC does NOT reset Jetson|?|
|5.2sec cyle|FC does NOT reset Jetson|FC does reset Jetson|?|

#
> <ins>Procedure:</ins>
> - [ ] Connect the FC and Jetson. This includes the GPIO pin and the RST pin
> - [ ] Wait 15 seconds and verify Jetson stays ON
> - [ ] Change the Jetson script to send a pulse every 4.8 seconds
> - [ ] wait 15 seconds and verify Jetson stays ON
> - [ ] Change the Jetson script to send a pulse every 5.2 seconds
> - [ ] wait 10 seconds and verify Jetson RESETS
#
### 2. FC to Jetson Reflashing Test
> <ins>Objective:</ins> We will be verifying that the FC is capabable of reflashing the Jetson. We will do this by connecting the FC to the Jetson through USB, and running a script that will reflash the Jetson.
#
 <ins>Test Equipment:</ins>
 - Flight computer (IMX8 dev board / VOXL2 dev board)
 - Jetson
 - USB to USB-C Cable
#
<ins>Reflash Script: (needs to be developed)</ins>
 - The reflash script will run commands that will reflash the Jetson.
#
<ins>Pass/Fail Criteria:</ins>

| Test        | FAIL        | PASS        | yes/no?     |
| ----------- | ----------- | ----------- | ----------- |
| Reflash capability | unable to relfash | successfully reflash |?|

#
> <ins>Procedure:</ins>
> - [ ] Connect the FC to the Jetson using the USB cable
> - [ ] Run the reflash script on the FC
> - [ ] Verify that the Jetson Reflashes
#

## Final System Level Test (FC, SatCat5, and Jetson)
### 1. Data Transfer Test
> <ins>Objective:</ins> We will be verifying ethernet switching capability on the Arty A7 FPGA dev board between a FC and Jetson. To do this, we will have data collected from a sensor connected to a FC. That FC will be connected through ethernet to SatCat5, where it will relay the data to the Jetson through ethernet as well. Then, the data recieved on the Jetson will be echoed back to the FC and save it to the FC's memory.
#
 <ins>Test Equipment:</ins>
 - SatCat5 FPGA dev board
 - Jetson
 - flight computer (IMX8 dev board / VOXL2 dev board)
 - 2x ethernet cables
 - 1x Ethernet PMod connector
 - sensor (for data)
#
<ins>FPGA Script (need to be developed):</ins>
- Description: This script needs to define 2 ethernet gates where the laptop and FC will be plugged into. The code will create a pathway for the two to talk to each other and pass over data in realtime.

<ins>Flight Computer and Jetson Script (need to be developed):</ins>
- The flight computer that will start the sensor data and send it through its ethernet port, and also to recieve data and write it down in its memory. 
- The Jetson script will recieve data from its ethernet port, save it, and echo it back to the flight computer.
#
<ins>Pass/Fail Criteria:</ins>

| Test        | FAIL        | PASS        | yes/no?     |
| ----------- | ----------- | ----------- | ----------- |
| full system functionality | system does not transfer data from FC to Jetson and vice versa | system is able to transfer data between FC and Jetson and vice versa |?|
| Jetson Writing | Jetson is unable to write data received from FC | Jetson successfully writes data received from FC |?|
| transfer speed | noticable lag | no noticeable lag |?|
| SatCat5 functionality | SatCat5 stops working during data transfer | SatCat5 runs with no issues during data transfer |?|

#
> <ins>Procedure:</ins>
> - [ ] connect sensor to the flight computer
> - [ ] connect the Jetson and FC to the FPGA using ethernet cables
> - [ ] program the FPGA 
> - [ ] start data transfer scripts on both the flight computer and Jetson
> - [ ] wait 30 seconds and verify the data saved on the Jetson and flight computer
#
### 2. Total Power Test
> <ins>Objective:</ins> We will be oberving the overall power draw of the entire system including the FC, SatCat5, and Jetson. We will do this by running the data transfer test (the test right above this test) and monitor the power draw levels on the FC, SatCat5, and Jetson. We will add up the power draw from each of these components to obtain the overall power draw of the sytem.
#
 <ins>Test Equipment:</ins>
 - SatCat5 FPGA dev board
 - Jetson
 - flight computer (IMX8 dev board / VOXL2 dev board)
 - 2x ethernet cables
 - 1x Ethernet PMod connector
 - sensor (for data)
#
<ins>Pass/Fail Criteria:</ins>

| Test        | FAIL        | PASS        | yes/no?     |
| ----------- | ----------- | ----------- | ----------- |
| Overall Power Draw | over # | under # |?|


#
> <ins>Procedure:</ins>
> - [ ] Connect the FC, SatCat5, and Jetson together
> - [ ] Run the same procedure listed under the Data Transfer Test above
> - [ ] Monitor and record power draw from the FC, SatCat5, and Jetson
> - [ ] Sum the power draws together to obtain the overall power draw
#

### 3. Full FPrime-to-FPrime Communication Test
> <ins>Objective:</ins> We will verify the communication between the FC and the Jetson with FPrime running on both of the systems. We will do this by having the FC FPrime deployment pull telemetry channel data through the Jetson's FPrime Deployment and vice versa.

#
 <ins>Test Equipment:</ins>
 - SatCat5 FPGA dev board
 - Jetson
 - flight computer (IMX8 dev board / VOXL2 dev board)
 - 2x ethernet cables
 - 1x Ethernet PMod connector
 - sensor (for data)
 #
<ins>FPrime Component and Commands (need to be developed):</ins>
- Component: ?
   - Description: ?
    - Commands:
      - ?
      
    - Telemetry Channels:
        - ?
- Component: ?
   - Description: ?
    - Commands:
      - ?
      
    - Telemetry Channels:
        - ?
#
<ins>Pass/Fail Criteria:</ins>

| Test        | FAIL        | PASS        | yes/no?     |
| ----------- | ----------- | ----------- | ----------- |
| FC to Jetson Communication | FC unable to retreive data | FC able to retreive data |?|
| Jetson to FC Communication | Jetson unable to retreive data | Jetson able to retreive data |?|

#
> <ins>Procedure:</ins>
> - [ ] Connect the FC, SatCat5, and Jetson together
> - [ ] Run the respective FPrime deployments on the FC and Jetson
> - [ ] Run the GDS's for the FC deployment and the Jetson deployment
> - [ ] (**need help on Alex's side to finish the rest of the procedure and making the right components on FPrime**)