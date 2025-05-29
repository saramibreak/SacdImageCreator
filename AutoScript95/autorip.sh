#! /bin/sh
sleep 10
cd /mnt/sda1/AutoScript
./SacdImageCreator /dev/sr0 128 > SacdImageCreator.txt 2>&1
