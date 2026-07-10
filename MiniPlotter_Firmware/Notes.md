# Notes

If using ICSP it will delete the bootloader, to reprogram with usb serial run:
```
pio run --target bootloader
```

Sometimes bootloader will explode if not carefull when uploading to board
Make a new temp program with this platformio.ini
```
upload_protocol = usbasp
upload_command = avrdude -p atmega328p -C ~/.platformio/packages/tool-avrdude/avrdude.conf -c usbtiny -U lfuse:w:0xFF:m -U hfuse:w:0xDA:m -U efuse:w:0x05:m -U flash:w:$SOURCE:i
```
Plug in the ICSP programmer and run
```
pio run --target bootloader
```