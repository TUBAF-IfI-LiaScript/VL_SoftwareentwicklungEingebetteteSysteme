import pysimavr.connect
from pysimavr.connect import connect_pins_by_rule
from pyavrutils import AvrGcc
from pysimavr.avr import Avr
from pysimavr.firmware import Firmware
from pysimavr.button import Button
from pysimavr.swig.simavr import avr_gdb_init, cpu_Done, cpu_Crashed, cpu_Stopped
from pysimavr.vcdfile import VcdFile
from pyDigitalWaveTools.vcd.parser import VcdParser
import pandas as pd
import re

def prepareHeaderFiles (names, folder):
  # sources may be defined by code OR file names
  # header files have to be passed by source code (no idea why)
  headers = {}
  for name in names:
    with open(folder+name) as f:
      content = f.readlines()
    headers[name]=''.join(content)
  if headers == {}:
    headers = None
  return headers

def evaluateVCDfile(file_name, channels):
    # Extract start and end time from vcd file and assign information to
    # channel names 
    vcd = open(vcd_file)
    timeseries_started= False
    signals=[]
    for line in vcd:
        line = line.rstrip()
        if "$var" in line:
            for channel in channels.keys():
                if channel in line:
                    channels[channel] = line.split(' ')[3]
        else:
            if re.findall('^#', line):
                current_time = int(''.join(line[1:]))
                timeseries_started = True
            elif timeseries_started == True:
                for channel, variable in channels.items():
                    if variable in line:
                        sigVar = line.split(' ')
                        data_entry={}
                        data_entry['channel'] = channel
                        data_entry['state'] = int( sigVar[0][0])
                        data_entry['time_us'] = float(current_time)/(10.**3)
                        signals.append(data_entry)

    pd_signals = pd.DataFrame(signals)
    return pd_signals

mcu = 'atmega2560'
f_cpu = 16000000
vcdfile = 'output'

sources = ['./avrCode/main.c', './avrCode/AVR_writeSerial.c']
headerfiles = ['AVR_writeSerial.h']
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

build_successful = False
try:
    cc.build(sources = sources,
             headers = prepareHeaderFiles (headerfiles, folder=folder))
    print("Command list")
    print(cc.command_list(sources = sources))
    size = cc.size()
    print('-------------------------------------------------------------------')
    print('Program size \n  program =' , str(size.program_bytes).rjust(8) ,
          '\n  data    =', str(size.data_bytes).rjust(8))
    print('-------------------------------------------------------------------')
    build_successful = True

except AvrGccCompileError as e:
    print(cc.error_text)
    print( 'compile error')

if build_successful:
    firmware = Firmware(cc.output)
    firmware.mcu = mcu
    firmware.f_cpu = f_cpu
    avr=Avr(mcu=mcu,f_cpu=f_cpu)
    avr.load_firmware(firmware)
    vcd_file = "%s.vcd"%(vcdfile)
    simvcd = VcdFile(avr, period=1000, filename=vcd_file)
    connect_pins_by_rule('''avr.B0 ==> vcd
                            avr.B1 ==> vcd
                            avr.B2 ==> vcd
                            avr.B3 ==> vcd''',
                     dict(avr=avr,),
                     vcd=simvcd,)
    simvcd.start()
    avr.step(f_cpu)
    simvcd.terminate()
    avr.terminate()

    channels = {'avr.B0':'', 'avr.B1':'', 'avr.B2':'', 'avr.B3':''}
    pd_signals = evaluateVCDfile(vcd_file, channels)

    print("Duration of active pins")
    for channel in channels:
        duration = pd_signals[pd_signals.channel == channel]['time_us'].diff().values[-1]
        print("{0} - {1:12.5f} us".format(channel, duration))
