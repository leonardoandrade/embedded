import time
import sys
from datetime import datetime
from w1thermsensor import W1ThermSensor
import requests
import Adafruit_DHT
import RPi.GPIO as GPIO
import os

DB = 'house_sensors'
TABLE = 'living_room'

INFLUXDB_HOST = os.environ['INFLUXDB_HOST']
INFLUXDB_USER = os.environ['INFLUXDB_USER']
INFLUXDB_PASSWORD = os.environ['INFLUXDB_PASSWORD']


def send_value_to_influxdb(sensor, value):
    try:
        data = str(TABLE) + ',sensor=' + str(sensor) + ' value=' + str(value)
        res = requests.post(url='https://' + INFLUXDB_HOST + '/influxdb/write?db=' + DB,
                        data=data,
                        headers={'Content-Type': 'application/octet-stream'},
                        auth = (INFLUXDB_USER, INFLUXDB_PASSWORD))
        if res.status_code != 204:
            print "error on saving", res
        time_now_iso8601 = datetime.now().replace(microsecond=0).isoformat()
        print(time_now_iso8601 + ',' + sensor + ',' + str(value))
    except Exception:
        print "error occurred saving value"


if len(sys.argv) != 4:
    print "USAGE:"
    print sys.argv[0], " <interval-in-seconds> <DHT11 pin> <sound pin>"
    sys.exit(1)

interval = float(sys.argv[1])
dht_11_pin = int(sys.argv[2])
sound_pin = int(sys.argv[3])

#temperature 1-wire sensor
temp_sensor = W1ThermSensor()

#setup sound sensor
GPIO.setmode(GPIO.BCM)
GPIO.setup(sound_pin, GPIO.IN)

sound_events = 0

def sound_detected(_):
    global sound_events
    sound_events = sound_events + 1
    
GPIO.add_event_detect(sound_pin, GPIO.RISING, callback=sound_detected)

while True:
  temp_celsius = temp_sensor.get_temperature()
  send_value_to_influxdb('temperature', temp_celsius)

  humidity, temperature = Adafruit_DHT.read_retry(Adafruit_DHT.DHT11, dht_11_pin)
  send_value_to_influxdb('humidity', humidity)
  
  send_value_to_influxdb('sound_events', sound_events)
  sound_events = 0
  time.sleep(interval)

  

