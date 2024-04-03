; This code was motivated by the repositoriy of Marius Ghita
;
; https://gist.github.com/mhitza/8a4608f4dfdec20d3879#file-makefile
;
; It was adapted to implement a small hello world implementation.
;
; Copyright 2015 Marius Ghita
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;     http://www.apache.org/licenses/LICENSE-2.0
;
; We specify the Atmel microprocessor. The Uno uses ATmega328P,
; older versions might use ATmega328 
.device ATmega328P


.equ PORTB = 0x05
.equ DDRB  = 0x04
.equ PINB  = 0x03
.equ TCCR1B = 0x81
.equ TCNT1L = 0x84	; MEMORY MAPPED
.equ TCNT1H = 0x85	; MEMORY MAPPED

.org 0x0000
    jmp main

main:
    sbi DDRB, 5
    sbi PORTB, 5
    ldi r24, 5
    sts TCCR1B, r24       ; prescaler 1024 
    ldi r26, 9
    ldi r27, 61           ; compare value 15625
      
loop:
    lds  r24, TCNT1L
    lds  r25, TCNT1H
    cpi  r25, HIGH(15625)
    breq test_low_byte
    brsh toggle
    rjmp loop

test_low_byte:
    cpi  r24, LOW(15625)
    brsh toggle
    rjmp loop
    
toggle:
    ldi r24, 5
    sts PINB, r24
