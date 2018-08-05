import time
import sys
from datetime import datetime
from w1thermsensor import W1ThermSensor
import requests
import Adafruit_DHT
import RPi.GPIO as GPIO
import os

DB = 'brewing'
TABLE = 'brewing'

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


if len(sys.argv) != 3:
    print "USAGE:"
    print sys.argv[0], " <interval-in-seconds> <DHT11 pin> "
    sys.exit(1)

interval = float(sys.argv[1])
dht_11_pin = int(sys.argv[2])

#temperature 1-wire sensor
temp_sensor = W1ThermSensor()


while True:
  temperature_inside = temp_sensor.get_temperature()
  send_value_to_influxdb('temperature_inside', temperature_inside)

  humidity, temperature_outside = Adafruit_DHT.read_retry(Adafruit_DHT.DHT11, dht_11_pin)
  send_value_to_influxdb('humidity', humidity)
  send_value_to_influxdb('temperature_outside', temperature_outside)

time.sleep(interval)

  

