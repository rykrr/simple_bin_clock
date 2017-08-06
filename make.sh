avr-gcc -mmcu=atmega328 -o ledbin simple_led_clock.c
avr-objcopy -j .text -j .data -O ihex ledbin ledhex
sudo avrdude -c usbasp -p atmega328 -U flash:w:ledhex 
