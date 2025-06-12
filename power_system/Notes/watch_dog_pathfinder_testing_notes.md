## Notes and Testing Criteria for the Watch Dog Pathfinder Testing Boards ##
Link to board Github [https://github.com/BroncoSpace-Lab/scales-hardware/pull/28]

# 4/8/25 #
    Luca and Phyllis went over the Watch Dog board and its basic concept of operations. Seperated the top watch dog circuit in two by removing R31 in WD1 subsheet and isolating the comparator segments. 

    This was done to test that the capacitors were in fact charging, however this led us to the following realization.

    What is happening here:
        - When the output transistor is off, the open-collector is well, open, which means that the pull up resistor inside is open but pulling the output voltage high. However, because this behavior characterizes the output as very high impedance, it can be used to drive some current from an actual source through and across the 4.7M resistor, therefore causing the charge of the capacitors.

        This can help us identify what the problem really is.

    Comparator Reset Pulse Threshold:
        - In order for the Watch dog board to trigger the reset, the capacitors must charge PAST the voltage at the non-inverting input of U1C, which is the reference voltage set by the resistor divider set by R32 and R34. This voltage is roughly 3.6V.

        Note that via another Resistor divider the voltage across the 4.7M resistor is around 4.6, and due to its high impedance will slowly charge the capacitor, again note that the time constant is around 96s but that is for a full charge cycle. 
    
    Caps:
        - The capacitors are in fact charging, but not according to the time constant we believe is set by the 4.7M ohm resistor and the 10uf + 10uf capacitors. According to the time constant definition RC, the expected charge time is, 
        20uF * 4.7M = 94s
        - However, what we have observed is that this charge up behavior does no occur within 94s, but rather within 3-4 hours to reach the full charge value.

        Testing Notes:
        D7 is always acting as an open, the diode is in reverse bias as the voltage across it, set by the comparator open collector is ~5v and the inverting input has whatever voltage the capacitor is currently charged to.

        Behavior of fully charging the capacitors at U1C:
        at t=0s, they are at approximately 22mv
        at t~90s, they are at approximately 100mv
        at t~10min 30s they are at approximately 536mv
        at t~20min they are at around 800mv

        so the capacitors are charging, but something is sinking current

        Voltage at U1C open-collector = 4.6V
        Voltage at the U1C inverting input slowly increasing as the capacitors charge. 

        The voltage across the diode + to - is negative, therefore it is acting as an open when the capacitors charge

        The voltage across the 100k resistors, R33 is the same voltage as that voltage across the two capacitors, therefore it is acting as an open due to the RB of the diode. This is good

        So the design itself is not incorrect, the open collector output is setting the desired voltage, the capacitors are in fact charging and the diode is not in forward bias when not expected to be.

        What could be the problem?
            -Diode has reverse leakage current that may be stealing current from the capacitor
                - Just removed D7 from the 1st WD circuit on the pathfinder board, still having the same issue, taking really really long to charge
            -
        What cant be the problem? 
            - Because the voltage at the U1C node is ~4.6v, the open-collector is not sinking current, if it was, it would read 0, as the transistor would sink the current when the comparator would trigger
            - Input bias current from LM2901B is in nA, the amount we are charging the capacitors to is in uA, they differ from 3 orders of magnitude, so this shouldnt justify such a massive slow down. 

        Most probable cause:
            - The inverting input of the LM2901B is sinking enough current to affect the charge time. The TLV1704 has an input bias current of around 10pA, where as the LM2901B has an input bias current of 25nA to 250nA

        Next Test:
        Cut pin 8 for U1C to check that it has nothing to do with the input pin of the comparator
        
        Next cut pin 14 for U1C to check that the output is not causing a charging issue

      
    