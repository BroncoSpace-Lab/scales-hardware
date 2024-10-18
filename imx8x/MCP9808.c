// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MCP9808
// This code is designed to work with the MCP9808_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Temperature?sku=MCP9808_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-16";
	if ((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, MCP9808 I2C address is 0x18(24)
	ioctl(file, I2C_SLAVE, 0x18);

	// Select configuration register(0x01)
	// Continuous conversion mode, Power-up default(0x00, 0x00)
	char config[3] = {0};
	config[0] = 0x01;
	config[1] = 0x00;
	config[2] = 0x00;
	write(file, config, 3);
	// Select resolution rgister(0x08)
	// Resolution = +0.0625 / C(0x03)
	config[0] = 0x08;
	config[1] = 0x03;
	write(file, config, 2);
	sleep(1);

	// Read 2 bytes of data from register(0x05)
	// temp msb, temp lsb
	char reg[1] = {0x05};
	write(file, reg, 1);
	char data[2] = {0};
	if(read(file, data, 2) != 2)
	{
		printf("Error : Input/Output error \n");
	}
	else
	{
		// Convert the data to 13-bits
		int temp = ((data[0] & 0x1F) * 256 + data[1]);
		if(temp > 4095)
		{
			temp -= 8192;
		}
		float cTemp = temp * 0.0625;
		float fTemp = cTemp * 1.8 + 32;

		// Output data to screen
		printf("Temperature in Celsius is : %.2f C \n", cTemp);
		printf("Temperature in Fahrenheit is : %.2f F \n", fTemp);
	}
}
