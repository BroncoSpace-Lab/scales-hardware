# i.MX 8X Custom Carrier Board

Notes:

- ENET0 is translated by the SOM by default
- ENET1 is not translated by the SOM, so we need to integrate out own chip for it into our design.
the iMX is not rated for RGMII operation at 3.3V, so we must configure it for 1.8V operation