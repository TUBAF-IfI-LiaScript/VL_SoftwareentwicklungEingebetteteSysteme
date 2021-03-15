TARGET     = main

# CTRL ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
CTRL     = atmega2560
CLOCK      = 8000000

CC      = avr-gcc
OC      = avr-objcopy
LDFLAGS = -j .text -j .data
CCFLAGS = -g -Wall -Os -Wunused-variable

######################################################################
######################################################################

all:
	@printf "Examplary make file for avr compilation\n"
	@printf "======================================================\n"

	@printf "1. compiling \n"
	$(CC) -c $(CCFLAGS) -mmcu=$(CTRL) $(TARGET).c -o $(TARGET).o

	@printf "\n2. linking \n"
	$(CC) -g -mmcu=$(CTRL) $(TARGET).o -o $(TARGET).elf

	@printf "\n3. generate properly output format\n"
	$(OC) $(LDFLAGS) -O ihex $(TARGET).elf $(TARGET).hex

	@printf "\n4. generate properly output format\n"
	avr-objdump -S -D $(TARGET).elf >  $(TARGET).asm

	@printf "\n5. analyse code size \n"
	avr-size --mcu=$(CTRL) -C $(TARGET).elf

clean:
	rm -f $(TARGET).elf $(TARGET).o $(TARGET).hex $(TARGET).asm
