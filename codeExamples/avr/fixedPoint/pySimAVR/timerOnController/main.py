from entrypoint2 import entrypoint
from pyavrutils.avrgcc import AvrGcc, AvrGccCompileError
from pysimavr.sim import ArduinoSim
from pysimavr.firmware import Firmware
from pysimavr.avr import Avr
import pandas as pd
import time

def prepareHeaderFiles (names, folder):
  # sources may be defined by code OR file names
  # header files have to be passed by source code (no idea why)
  # lets extract the source code from header files
  headers = {}
  for name in names:
    with open(folder+name) as f:
      content = f.readlines()
    headers[name]=''.join(content)
  if headers == {}:
    headers = None
  return headers

mcu = 'atmega2560'
f_cpu = 16000000

sources = ['./avrCode/main.c', './avrCode/AVR_writeSerial.c', './avrCode/AVR_timer.c']
headerfiles = ['AVR_writeSerial.h', 'AVR_timer.h']
folder = "./avrCode/"

cc = AvrGcc(mcu = mcu)
cc.f_cpu = f_cpu
# the additional libraries are necessary for a complete printf support
# take a view on http://www.nongnu.org/avr-libc/user-manual/
cc.options_extra = ['-uvfprintf', '-lprintf_flt', '-lm']
cc.optimize_for_size()

print(cc.options_generated())

print('-------------------------------------------------------------------')
print( 'compiler version:', cc.version())
print('-------------------------------------------------------------------')
print('Project targets')
for source in sources:
  print('  '+source)
for headerfile in headerfiles:
  print('  '+headerfile)
print('-------------------------------------------------------------------')

try:
    cc.build(sources = sources,
             headers = prepareHeaderFiles (headerfiles, folder=folder))

    print("Command list")
    print(cc.command_list(sources = sources))

    cc.build(sources = sources)
    print('Temporary output file \n' + '   ' + cc.output)
    size = cc.size()
    print('-------------------------------------------------------------------')
    print('Program size \n  program =' , str(size.program_bytes).rjust(8) ,
          '\n  data    =', str(size.data_bytes).rjust(8))
    print('-------------------------------------------------------------------')

    avr=Avr(mcu=mcu,f_cpu=f_cpu)
    firmware = Firmware(cc.output)
    avr.load_firmware(firmware)

    fps=20
    speed=1
    timespan=5

    dt_real = 1. / fps
    dt_mcu = dt_real * speed
    count = int(timespan * fps / speed)
    for _ in range(count):
      time.sleep(dt_real)
      avr.move_time_marker(dt_mcu)

    avr.goto_time(timespan)

    while avr.time_passed() < timespan * 0.99:
       time.sleep(0.05)

    print('Outputs' + ''.join(avr.uart.buffer))
    print('-------------------------------------------------------------------')
    avr.terminate()

except AvrGccCompileError as e:
    print(cc.error_text)
    print( 'compile error')
