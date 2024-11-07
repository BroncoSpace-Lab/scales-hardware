# FlatSat Development

This document is an overview of the current and future state of our FlatSat development for SCALES.

## Current Updates
### **Direct Ethernet Connection**

We are currently able to connect the boards directly in the following configuration:

![direct ethernet connection](Images/direct_ethernet.png)

From there, we have to set a temporary ip on the iMX, in this case it is set to 10.3.2.6 so we can ping the Jetson's ip.

![imx pinging jetson](Images/imx_ping_jetson.png)

We can also use the iMX to ssh into the Jetson:

![imx ssh into jetson](Images/imx_ssh_into_jetson.png)

And we can do the same process of pinging the iMX and ssh from the Jetson:

![jetson ping imx](Images/jetson_ping_imx.png)

![jetson ssh into imx](Images/jetson_ssh_into_imx.png)

We are also able to copy files from the iMX to the Jetson. In this example, we copied a file called mcp9808a from the iMX to the Jetson:

![scp from imx to jetson](Images/imx_scp_to_jetson.png)

### **SatCat**

We are currently only able to use SatCat to send/recieve messages from different serial ports within the same windows host computer. 

We followed [this example setup from SatCat's GitHub](https://github.com/the-aerospace-corporation/satcat5/tree/main/examples/arty_a7) to download the software onto the FPGA and set up the hardware in the following configuration: 

![satcat setup](Images/satcat_setup.png)

With the python chat client, we were able to get data transfer speeds on both UART and Ethernet:

![switch configuration](Images/satcat_switch_config.png)

![ethernet configuration](Images/satcat_ethernet_config.png)

![uart configuration](Images/satcat_uart_config.png)

**Next Step:** Change Verilog code to enable communication between two different systems (Jetson to the flight computer).


### **VOXL2**
The VOXL2 is currently bricked after attempting to update the software using the command "voxl-configure-mpa".

We followed the unbricking tutorial numerous times and encountered issues with VOXL's firmware files that fail to call on its own files when flashing the board. We submitted a ticket to the ModalAI forum two weeks ago and havent heard back.

The error we are receiving when attempting to flash the board is shown below:

picture of error********************


### iMX 8X

For more details, take a look at [documentation](https://scales-hardware.readthedocs.io/en/latest/imx8x_procedures/).

**Main updates:**
- successfully re-flashed the operating system to a Yocto Linux kernel
- set up the SDK on a Linux host computer to cross compile code
- got an I2C sensor up and running with a C code executable file to read sensor data on the board
- able to copy code from GitHub instead of using as USB every time
- figured out how to run python code on the board
- started trying to get F Prime up and running on board

**Development Environment**

To run python code on the iMX, any python code file can be marked as executable and run on the board. For example, the following hello world python script runs as expected on the iMX:
```
#!/usr/bin/env python

print("Hello, World!")
```
The setup and output on the board looks like this:

![hello world python output](Images/helloworldpython_output.png)

For other languages including C and C++, code must be cross-compiled in the SDK on the Linux Host machine. We have currently tested this using both C and C++ code and compiliers. After the code is cross-compiled in the SDK, it generates an executable file that can be run on the iMX. For more information on the cross-compilation process, reference the [I2C Interfacing Guide](https://scales-hardware.readthedocs.io/en/latest/imx8x_procedures/#i2c-interfacing) from our docs. In that guide, we cross-compile C code to read data from an I2C temperature sensor and output the result in terminal on the board. 

**PhyTEC Guide Experience**

The guides from PhyTEC for this board are a bit rough to work with at times. Many of the issues we ran into during development were due to a lack of description and detail in the docs and guides. We mainly found that the guides are great at getting you started, but lack the explaination on how many of these interfaces and protocols can be used outside of basic setup. Some interfacing guides are better than others, though, and others can sometimes be outdated. There are also not many tutorials on compiling code, the only examples shown compile C++ code. This is something that may become a challenge later on in development. 

However, it is worthy to note that phyTEC's support team does reply quickly and consistently, usually within 1-2 days and with helpful feedback, which makes development a little easier. 

**F Prime Development**

We are in the process of getting F Prime deployed on the iMX. We made a [discussion post](https://github.com/nasa/fprime/discussions/3002#discussioncomment-11158814) on the F Prime GitHub going over our process and the current issues we have. We have recieved some feedback that will be looked into in the next few days.

Ideally we can get F Prime up and running in the next week or so, that way we can use it for real-time data transmission between boards instead of just copying over files over ethernet. 

## Future FLATSAT Plan
After viewing the full capabilities of our chosen FPGA, we are currently comparing two different hardware architectures for our SCALES system:

### Architecture 1
Architecture 1 has sensors attached to the **flight computer**.

![image](https://github.com/user-attachments/assets/3d400f78-7bee-4894-8597-d6ad3240c092)


__Pros:__

__Cons:__


### Architecture 2
Architecure 2 has sensors attached to **SATCAT**

![image](https://github.com/user-attachments/assets/de4894f7-e44c-494b-947f-bd0005bf10aa)


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

**[Test Procedure](https://scales-hardware.readthedocs.io/en/latest/Test%20Procedure%20List/)**
