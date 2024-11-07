# FlatSat Development

This document is an overview of the current state of our FlatSat development for SCALES.

## Direct Ethernet Connection

We are currently able to connect the boards directly in the following configuration:

picture from draw.io

From there, we have to set a temporary ip on the iMX, in this case it is set to 10.3.2.6 so we can ping the Jetson's ip.

![imx pinging jetson](Images/imx_ping_jetson.png)

We can also use the iMX to ssh into the Jetson:

![imx ssh into jetson](Images/imx_ssh_into_jetson.png)

And we can do the same process of pinging the iMX and ssh from the Jetson:

![jetson ping imx](Images/jetson_ping_imx.png)

![jetson ssh into imx](Images/jetson_ssh_into_imx.png)

## SatCat

We are currently only able to use SatCat to send/recieve messages from different serial ports within the same windows host computer. We followed [this example setup from SatCat's GitHub](https://github.com/the-aerospace-corporation/satcat5/tree/main/examples/arty_a7) to download the software onto the FPGA and set up the hardware in the following configuration: 

picture from drawio
