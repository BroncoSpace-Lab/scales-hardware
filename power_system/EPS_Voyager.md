## Design Notes for EPS Voyager Board ##
* By Luca Lanzillotta
* 4/15/25
## Abstract ##
    The main purpose fo the EPS Voyager board is to provide an easily reconfigurable system reflective of current power subsystem designs that allows for testing and verification of subsystem designs.
## Requirements ##
    - The design must have the following:
        - Follow component layout requirements from component datasheets
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

        - Allow for enough spacing to replace the components that must be configured
            - Because components with excess track widths, like for example using jumper pins and breadboards will be exhaustive in terms of loop area, it would be optimal to segment out areas of the board that will isolate each configurable component so they can be manually soldered and removed when necessary.

        - Must have test points for every major component of the subsystem [DONE]
        - Must have all components available from JLC PCB
         - Use (this)[https://github.com/Bouni/kicad-jlcpcb-tools] extension to select and check which components are available for ordering from JLCPCB
## Design Notes From Reviewers ##
    -  **4/14/2025** 
        Michael:
        1. Is there a reason why you did not follow the recommended layouts prescribed in the LT and INA datasheets? Usually you should replicate them faithfully unless there are packaging concerns with your particular board
        2. The routing on the switching regulator is very suboptimal, too much loop area and not enough ground connections. The recommended layout section of the datasheet addresses this
        3. Take advantage of copper fill on your other layers. When in doubt fill with ground.
        4. How are you planning to do your interchangeable components like the shunt and the inductor? Usually these components cannot be (easily or at all) placed off board because they would probably require mezzanine boards (which are not economical to create) and going off board for passives like this causes big EMI issues.
        5. Make sure that all of the parts you have selected are actually available to order at our manufacturer, JLC. I Recommend using this plugin to assign all of the part numbers as soon as you can: 


