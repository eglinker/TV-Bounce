cls

REM Compile to run on a Atmega328p microcontroller.`
avr-gcc.exe -g -Os -mmcu=atmega328p -c amvtg.c     -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
avr-gcc.exe -g -Os -mmcu=atmega328p -c main.c      -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

REM Link and generate binary (.hex file).
avr-gcc.exe -g -mmcu=atmega328p -o demo.elf amvtg.o main.o
avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex 

REM Use this line to download to Atmega328, if is has an Arduino bootloader.
avrdude.exe -C "C:\avrdude\avrdude.conf" -p atmega328p -c arduino -P COM3 -b 115200 -U   flash:w:demo.hex

pause
