/*********************************************************************************************//**
\page lfw NETX90 COM Loadable Firmware - LFW - LIMITED EVALUATION VERSION
===============================================================

The *.nxi files contain the netX90 limited evaluation loadable firmware,
implementing the RTE (Real-Time-Ethernet) or Fieldbus-Protocol.
This firmware is executed on the netX 90 COM (communication) side.
\note 
It must NOT BE USED for production. 

A full LFW version is a commercial product and must be licenced by Hilscher.
Please contact sales@hilscher.com for further information.

The limited evaluation LFW supports all features of the respective full LFW version,
but might stop communciation and require a reset after >30min of operation.

Please refer to the *_usedlibs.txt files for version information.

Different LFW variants X*0.nxi and X*1.nxi cover different use cases and feature sets.
Fimrware Variants: https://kb.hilscher.com/x/xC2RBg

X*0.nxi: type 1 or 2 LFW, use case A/B, external Flash and SDRAM is optional
X*1.nxi: type 3 LFW, use case C, external Flash and SDRAM is mandatory

Please assure that the matching Hardware Config (*.hwc) and Flash Device Label (*.fdl) files
are loaded together with the LFW into the flash memory of netX 90.
The use cases require different FDLs.

Please check netX 90 documentation and the FAQ section for more information

netX 90 page:         https://kb.hilscher.com/x/dYJ3B
netX 90 FAQ section:  https://kb.hilscher.com/x/KXQWBg

*************************************************************************************************/