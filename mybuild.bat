cls

c:\WinAVR-20100110\bin\avr-gcc.exe -g -Os -mmcu=atmega328p -c amvtg.c     -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

c:\WinAVR-20100110\bin\avr-gcc.exe -g -Os -mmcu=atmega328p -c main.c      -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums


rem c:\WinAVR-20100110\bin\avr-gcc.exe -g -Os -mmcu=atmega328p -c main.c 
c:\WinAVR-20100110\bin\avr-gcc.exe -g -mmcu=atmega328p -o demo.elf amvtg.o main.o
c:\WinAVR-20100110\bin\avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex 
C:\avrdude\avrdude.exe -C "C:\avrdude\avrdude.conf" -p atmega328p -c arduino -P COM3 -b 115200 -U   flash:w:demo.hex

pause
