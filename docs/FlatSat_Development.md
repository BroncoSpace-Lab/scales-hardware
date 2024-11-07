# FlatSat Development

This document is an overview of the current and future state of our FlatSat development for SCALES.

# Current Updates
### **Direct Ethernet Connection**

We are currently able to connect the boards directly in the following configuration:

picture from draw.io***************

From there, we have to set a temporary ip on the iMX, in this case it is set to 10.3.2.6 so we can ping the Jetson's ip.

![imx pinging jetson](Images/imx_ping_jetson.png)

We can also use the iMX to ssh into the Jetson:

![imx ssh into jetson](Images/imx_ssh_into_jetson.png)

And we can do the same process of pinging the iMX and ssh from the Jetson:

![jetson ping imx](Images/jetson_ping_imx.png)

![jetson ssh into imx](Images/jetson_ssh_into_imx.png)
#
### **SatCat**

We are currently only able to use SatCat to send/recieve messages from different serial ports within the same windows host computer. 

We followed [this example setup from SatCat's GitHub](https://github.com/the-aerospace-corporation/satcat5/tree/main/examples/arty_a7) to download the software onto the FPGA and set up the hardware in the following configuration: 

picture from drawio*******

With the python chat client, we were able to get data transfer speeds on both UART and Ethernet:

pictures of chat client**************

**Next Step:** Change Verilog code to enable communication between two different systems (Jetson to the flight computer).

#
### **VOXL2**
The VOXL2 is currently bricked after attempting to update the software using the command "voxl-configure-mpa".

We followed the unbricking tutorial numerous times and encountered issues with VOXL's firmware files that fail to call on its own files when flashing the board. We submitted a ticket to the ModalAI forum two weeks ago and havent heard back.

The error we are receiving when attempting to flash the board is shown below:

picture of error********************

#
### **IMX8**
kelly's corner




# Future FLATSAT Plan
After viewing the full capabilities of our chosen FPGA, we are currently comparing two different hardware architectures for our SCALES system:

## Architecture 1
Architecture 1 has sensors attached to the **flight computer**.

Picture of architecture 1 diagram*************

__Pros:__

__Cons:__


## Architecture 2
Architecure 2 has sensors attached to **SATCAT**

Picture of architecture 2 diagram***************

__Pros:__

__Cons:__

## Testing Metrics
These are the test metrics that will help make hardware design decisions:

- 
- 
- 
- 
- 
- 

## Test Procedure
(link to Test procedure tab)
