# SacdImageCreator
## Overview
 This is a program to dump a SACD. It works on specific SACD player.

## Requirement
 1. Toolchain running on ARM processors with mediatek chipset  
 2. sacd_read.ko, which is embedded to sacd_extract_160  

## How to build
 1. Run host computer  
 2. Set the toolchain  
 3. `cd SacdImageCreator`  
 4. `make`  

## How to use
 1. Copy SacdImageCreator file and sacd_read.ko file to AutoScript directory.  
 2. Copy AutoScript directory to USB memory.  
 3. Insert USB memory into the SACD player.  
 4. Run SACD player.  
 5. After the tray is opened, set the disc. The tray is closed automatically.  
 6. Dump is completed, the tray is opened automatically and closed automatically.  

## Result
 The output files are as follows  
 * .iso - 2048 bytes per sector file  
 * .raw - 2064 bytes per sector file  
 * _filesystem.txt - File with track information  
 * _header.txt - A file containing the first 12 bytes and the last 4 bytes of the 2064 bytes per sector  
 * _sector.txt - byte-formatted file of _filesystem.txt  

## Supported player
|player                     |brand           |work             |
|---------------------------|----------------|-----------------|
|BDP-S390 / BX39            |Sony            |Not yet confirmed|
|BDP-S490                   |Sony            |Not yet confirmed|
|BDP-S495                   |Sony            |Not yet confirmed|
|BDP-S590 / BX59            |Sony            |Yes              |
|BDV-E190                   |Sony            |Not yet confirmed|
|BDV-E290                   |Sony            |Not yet confirmed|
|BDP-S4100                  |Sony            |Not yet confirmed|
|BDP-S5100 / BX510          |Sony            |Yes              |
|BDP-S6200 / BX620          |Sony            |Not yet confirmed|
|BDP-S7200                  |Sony            |Not yet confirmed|
|BDP-S790                   |Sony            |Not yet confirmed|
|BDP-A6000                  |Sony            |Not yet confirmed|
|BDV-N590                   |Sony            |Not yet confirmed|
|BDV-NF620                  |Sony            |Not yet confirmed|
|BDV-NF720                  |Sony            |Not yet confirmed|
|BDV-N7200                  |Sony            |Not yet confirmed|
|BDP-S6500                  |Sony            |Not yet confirmed|
|BDP-80FD                   |Pioneer         |Not yet confirmed|
|BDP-140                    |Pioneer         |Not yet confirmed|
|BDP-440                    |Pioneer         |Not yet confirmed|
|BDP-150                    |Pioneer         |Not yet confirmed|
|BDP-450                    |Pioneer         |Not yet confirmed|
|BDP-160                    |Pioneer         |Not yet confirmed|
|BDP-170                    |Pioneer         |Not yet confirmed|
|MCS-FS232                  |Pioneer         |Not yet confirmed|
|BDP-93                     |OPPO            |Not yet confirmed|
|BDP-95                     |OPPO            |Not yet confirmed|
|BDP-103 / 103D             |OPPO            |Not yet confirmed|
|BDP-105 / 105D             |OPPO            |Not yet confirmed|
|Azur 751BD                 |Cambridge       |Not yet confirmed|
|Azur 752BD                 |Cambridge       |Not yet confirmed|
|CXU                        |Cambridge       |Not yet confirmed|
|FMJ UDP411                 |Arcam           |Not yet confirmed|
|FMJ CDS27                  |Arcam           |Not yet confirmed|
|BD32 Mk.II                 |Primare         |Not yet confirmed|
|EMP 3                      |Electrocompaniet|Not yet confirmed|
|DBT-1713                   |Denon           |Not yet confirmed|
|DBT-3313UD / 3313UDCI      |Denon           |Not yet confirmed|
|Universal Media Transport V|MSB Technology  |Not yet confirmed|
|Signature UMT V            |MSB Technology  |Not yet confirmed|
|BD-S677                    |Yamaha          |Not yet confirmed|
|UD7007                     |Marantz         |Not yet confirmed|

## License
 See LICENSE

## Disclaimer
 Use this tool at own your risk.
 Trouble in regard to the use of this tool, I can not guarantee any.
