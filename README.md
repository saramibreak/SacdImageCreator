# SacdImageCreator
## Overview
 This is a program to dump a SACD. It works on specific SACD player.

## Requirement
 1. Toolchain running on ARM processors with mediatek chipset  
 2. sacd_read.ko, which is embedded to sacd_extract_160, sacd_extract_6200 or sacd_extract_95  

## How to build
 1. Run host computer  
 2. Set the toolchain  
 3. `cd SacdImageCreator`  
 4. `make`  

## Preparation before use
 1. Copy SacdImageCreator file and sacd_read.ko file to AutoScript directory.  
 2. Copy AutoScript directory to USB memory.  

## How to use
Pre-load type
 1. Turn on the SACD player.  
 2. Open the tray, set the disc and close the tray.  
 3. Turn off the SACD player.  
 4. Insert USB memory into the SACD player.  
 5. Turn on the SACD player again.  
 6. After the tray is opened and the tray is closed automatically.  
 7. Dump is completed, the tray is opened automatically and closed automatically.  

Not load type
 1. Insert USB memory into the SACD player.  
 2. Turn on the SACD player.  
 3. After the tray is opened, set the disc. The tray is closed automatically.  
 4. Dump is completed, the tray is opened automatically and closed automatically.  

## Result
 The output files are as follows  
 * .iso - 2048 bytes per sector file  
 * .raw - 2064 bytes per sector file  
 * _filesystem.txt - File with track information  
 * _header.txt - A file containing the first 12 bytes and the last 4 bytes of the 2064 bytes per sector  
 * _sector.txt - byte-formatted file of _filesystem.txt  

## Supported player
|player                     |brand           |sacd_read.ko     |work             |pre-load|
|---------------------------|----------------|-----------------|-----------------|--------|
|BDP-S390 / BX39            |Sony            |sacd_extract_160 |Not yet confirmed|        |
|BDP-S490                   |Sony            |sacd_extract_160 |Not yet confirmed|        |
|BDP-S495                   |Sony            |sacd_extract_160 |Not yet confirmed|        |
|BDP-S590 / BX59            |Sony            |sacd_extract_160 |Yes              |Yes     |
|BDV-E190                   |Sony            |sacd_extract_160 |Not yet confirmed|        |
|BDV-E290                   |Sony            |sacd_extract_160 |Not yet confirmed|        |
|BDP-S4100                  |Sony            |sacd_extract_160 |Not yet confirmed|        |
|BDP-S5100 / BX510          |Sony            |sacd_extract_160 |Yes              |No      |
|BDP-S6200 / BX620          |Sony            |sacd_extract_6200|Not yet confirmed|        |
|BDP-S7200                  |Sony            |sacd_extract_6200|Not yet confirmed|        |
|BDP-S790                   |Sony            |sacd_extract_6200|Not yet confirmed|        |
|BDP-A6000                  |Sony            |sacd_extract_6200|Not yet confirmed|        |
|BDV-N590                   |Sony            |sacd_extract_6200|Not yet confirmed|        |
|BDV-NF620                  |Sony            |sacd_extract_6200|Not yet confirmed|        |
|BDV-NF720                  |Sony            |sacd_extract_6200|Not yet confirmed|        |
|BDV-N7200                  |Sony            |sacd_extract_6200|Not yet confirmed|        |
|BDP-S6500                  |Sony            |sacd_extract_6700|Not yet confirmed|        |
|BDP-80FD                   |Pioneer         |sacd_extract_160 |Not yet confirmed|        |
|BDP-140                    |Pioneer         |sacd_extract_95  |Not yet confirmed|        |
|BDP-440                    |Pioneer         |sacd_extract_95  |Not yet confirmed|        |
|BDP-150                    |Pioneer         |sacd_extract_95  |Not yet confirmed|        |
|BDP-450                    |Pioneer         |sacd_extract_95  |Not yet confirmed|        |
|BDP-160                    |Pioneer         |sacd_extract_160 |Not yet confirmed|        |
|BDP-170                    |Pioneer         |sacd_extract_160 |Not yet confirmed|        |
|MCS-FS232                  |Pioneer         |sacd_extract_6200|Not yet confirmed|        |
|BDP-93                     |OPPO            |sacd_extract_95  |Yes              |No      |
|BDP-95                     |OPPO            |sacd_extract_95  |Not yet confirmed|        |
|BDP-103 / 103D             |OPPO            |sacd_extract_160 |Not yet confirmed|        |
|BDP-105 / 105D             |OPPO            |sacd_extract_160 |Not yet confirmed|        |
|Azur 751BD                 |Cambridge       |sacd_extract_95  |Not yet confirmed|        |
|Azur 752BD                 |Cambridge       |sacd_extract_160 |Not yet confirmed|        |
|CXU                        |Cambridge       |sacd_extract_160 |Not yet confirmed|        |
|FMJ UDP411                 |Arcam           |sacd_extract_160 |Not yet confirmed|        |
|FMJ CDS27                  |Arcam           |sacd_extract_160 |Not yet confirmed|        |
|BD32 Mk.II                 |Primare         |sacd_extract_160 |Not yet confirmed|        |
|EMP 3                      |Electrocompaniet|sacd_extract_160 |Not yet confirmed|        |
|DBT-1713                   |Denon           |sacd_extract_95  |Not yet confirmed|        |
|DBT-3313UD / 3313UDCI      |Denon           |sacd_extract_160 |Not yet confirmed|        |
|Universal Media Transport V|MSB Technology  |sacd_extract_160 |Not yet confirmed|        |
|Signature UMT V            |MSB Technology  |sacd_extract_160 |Not yet confirmed|        |
|BD-S677                    |Yamaha          |sacd_extract_160 |Not yet confirmed|        |
|UD7007                     |Marantz         |sacd_extract_6200|Not yet confirmed|        |

## License
 See LICENSE

## Disclaimer
 Use this tool at own your risk.
 Trouble in regard to the use of this tool, I can not guarantee any.
