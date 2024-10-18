#! /bin/bash
echo host > /sys/kernel/debug/ci_hdrc.0/role
cd /sys/class/gpio/
echo 30 > export
echo out > gpio30/direction
echo 0 > gpio30/value
cd /run/media/sda1
./mcp9808a
