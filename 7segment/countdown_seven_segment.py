import RPi.GPIO as GPIO
import time
 
# map of pins
pin_map = [16, #0
           12, #1
           19, #2
           13, #3
           6,  #4
           20, #5
           21] #6


segment_to_pin_map = [
    [GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.LOW],    #0
    [GPIO.LOW,  GPIO.HIGH, GPIO.HIGH, GPIO.LOW,  GPIO.LOW,  GPIO.LOW,  GPIO.LOW],    #1
    [GPIO.HIGH, GPIO.HIGH, GPIO.LOW,  GPIO.HIGH, GPIO.HIGH, GPIO.LOW,  GPIO.HIGH],   #2
    [GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.LOW,  GPIO.LOW,  GPIO.HIGH],   #3
    [GPIO.LOW,  GPIO.HIGH, GPIO.HIGH, GPIO.LOW,  GPIO.LOW,  GPIO.HIGH, GPIO.HIGH],   #4
    [GPIO.HIGH, GPIO.LOW,  GPIO.HIGH, GPIO.HIGH, GPIO.LOW,  GPIO.HIGH, GPIO.HIGH],   #5
    [GPIO.HIGH, GPIO.LOW,  GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.HIGH],   #6
    [GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.LOW,  GPIO.LOW,  GPIO.LOW,  GPIO.LOW],   #7
    [GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.HIGH],   #8
    [GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.HIGH, GPIO.LOW,  GPIO.HIGH, GPIO.HIGH]    #9
]


GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

def countdown(interval):
  for p in pin_map:
    GPIO.setup(p, GPIO.OUT)
   
  for x in reversed(range(9+1)):
    print "displaying:", x
    for i, s in enumerate(segment_to_pin_map[x]):
      GPIO.output(pin_map[i], s)

    time.sleep(interval)
    for pin in pin_map:
      GPIO.output(pin, GPIO.LOW)

countdown(0.5)
