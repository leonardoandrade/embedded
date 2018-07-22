import time
import sys
from datetime import datetime
from w1thermsensor import W1ThermSensor
temp_sensor = W1ThermSensor()

if len(sys.argv) != 3:
    print "USAGE:"
    print sys.argv[0], " <interval-in-seconds> <output-file>"
    sys.exit(1)

interval = float(sys.argv[1])
f_out = open(sys.argv[2], 'a') 

while True:
  temp_celsius = temp_sensor.get_temperature()
  time_now_iso8601 = datetime.now().replace(microsecond=0).isoformat()
  f_out.write(time_now_iso8601 + ";" + str(temp_celsius) + "\n")
  f_out.flush()
  time.sleep(interval)
    

f_out.close()
