TARGET  = code

CTRL    = atmega328p
STK     = arduino

CC      = avr-gcc
OC      = avr-objcopy
CPFLAGS = -nostdlib               # no standard library
LDFLAGS = -j .text -j .data

AVRDUDE = avrdude
AVRDUDE_FLAGS = -P /dev/ttyACM0
AVRDUDE_FLAGS += -B 57600
AVRDUDE_FLAGS += -c $(STK)
AVRDUDE_FLAGS += -p m328p
AVRDUDE_FLAGS += -v -D -U flash:w:$(TARGET).hex:i

.PHONY: all program clean com

all: $(TARGET).hex

$(TARGET).elf: $(TARGET).S
	@echo "assembling"
	@$(CC) -mmcu=$(CTRL) $(CPFLAGS) $< -o $@

$(TARGET).hex: $(TARGET).elf
	@echo "generate properly output format"
	@$(OC) $(LDFLAGS) -O ihex $< $@
	avr-size --mcu=$(CTRL) $<

program: $(TARGET).hex
	@echo "Programing the device"
	@echo  $(AVRDUDE_FLAGS)
	$(AVRDUDE) $(AVRDUDE_FLAGS)

clean:
	@echo "Cleaning"
	@rm -rf $(TARGET).hex $(TARGET).o $(TARGET).elf *~

com:    program
	@echo "Opening Serial connection"
	gtkterm -p /dev/ttyACM0 -s 9600 -r "-"
