# This is the second revision of the peripheral board.
    This board went from having a raspberry pi pico2 to having just an Etherent switch. The reason being is that this addition of low level sensors can be used on the flight control computer. This eliminates another system archetecture and give the end user the ability to create their own custom peripheral baord that can be added to a local area network.

    This makes this board just an Ethernet switch daughter board that is full optional.

## Changes from v0 to V1
    Moved away from the microcontroller to just being the multi-port ethernet switch.