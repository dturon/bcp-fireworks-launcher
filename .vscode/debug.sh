#!/bin/bash

TMP_FLASH_JLINK=$(mktemp)

echo -e "si 1\nspeed 4000\nr\nloadbin $1,0x08000000\nexit" > "$TMP_FLASH_JLINK"

JLinkExe -device stm32l083cz -CommanderScript "$TMP_FLASH_JLINK"

rm "$TMP_FLASH_JLINK"

JLinkGDBServer -singlerun -device stm32l083cz -if swd -speed 4000 -localhostonly -reset
