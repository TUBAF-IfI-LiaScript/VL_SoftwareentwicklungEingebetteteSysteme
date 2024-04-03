TARGET  = main

CTRL    = atmega328p
STK     = arduino

CC      = avr-gcc
OC      = avr-objcopy
CPFLAGS = -nostartfiles
LDFLAGS = -j .text -j .data

AVRDUDE = avrdude
AVRDUDE_FLAGS = -P /dev/ttyUSB0
AVRDUDE_FLAGS += -B 57600
AVRDUDE_FLAGS += -c $(STK)
AVRDUDE_FLAGS += -p m328p
AVRDUDE_FLAGS += -v -D -U flash:w:$(TARGET).hex:i

.PHONY: all

%.hex: %.asm
	@echo "Assambling asm code"
	avra -fI $<
	rm *.eep.hex *.obj *.cof

all: $(patsubst %.asm,%.hex,$(wildcard *.asm))

program: ${program}.hex
	@echo "Programing the device"
	@echo  $(AVRDUDE_FLAGS)
	$(AVRDUDE) $(AVRDUDE_FLAGS)

clean:
	@echo "Cleaning"
	@rm -rf $(TARGET).hex $(TARGET).o $(TARGET).elf *~

com:    program
	@echo "Opening Serial connection"
	gtkterm -p /dev/ttyACM0 -s 9600 -r "-"
