TARGET  = main

CTRL    = atmega328p
STK     = arduino

CC      = avr-gcc
OC      = avr-objcopy
CPFLAGS = -nostartfiles
LDFLAGS = -j .text -j .data

AVRDUDE = avrdude
AVRDUDE_FLAGS = -P /dev/ttyACM1
AVRDUDE_FLAGS += -B 57600
AVRDUDE_FLAGS += -c $(STK)
AVRDUDE_FLAGS += -p m328p
AVRDUDE_FLAGS += -v -D -U flash:w:$(TARGET).hex:i

.PHONY: all program asm clean com

all: $(TARGET).hex

$(TARGET).o: $(TARGET).c
	@echo "compiling"
	$(CC) -Os -c -o $@ $(CPFLAGS) -mmcu=$(CTRL) $<

$(TARGET).elf: $(TARGET).o
	@echo "linking"
	@$(CC) -mmcu=$(CTRL) $< -o $@

$(TARGET).hex: $(TARGET).elf
	@echo "generate properly output format"
	@$(OC) $(LDFLAGS) -O ihex $< $@
	avr-size --mcu=$(CTRL) $(TARGET).elf

program: all
	@echo "Programing the device"
	@echo  $(AVRDUDE_FLAGS)
	$(AVRDUDE) $(AVRDUDE_FLAGS)

asm: $(TARGET).asm

$(TARGET).asm: $(TARGET).elf
	@echo "Generating assembler code"
	avr-objdump -d -S $< > $@

clean:
	@echo "Cleaning"
	@rm -rf $(TARGET).hex $(TARGET).o $(TARGET).elf $(TARGET).asm *~