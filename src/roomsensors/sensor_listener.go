package main

import (
	"fmt"
	"github.com/d2r2/go-dht"
	"github.com/stianeikeland/go-rpio"
	"github.com/yryz/ds18b20"
	"time"
)

type SensorEvent struct {
	Sensor string
	Value  float64
}

func WatchTemperature(periodSeconds int) (<-chan SensorEvent, error) {
	events := make(chan (SensorEvent))
	sensors, err := ds18b20.Sensors()
	if err != nil {
		return nil, err
	}

	go func() {
		for {
			t, err := ds18b20.Temperature(sensors[0])
			if err != nil {
				fmt.Errorf("error when reading temperature:%v", err)
			} else {
				events <- SensorEvent{"temperature", t}
			}
			time.Sleep(time.Duration(periodSeconds) * time.Second)
		}
	}()

	return events, nil
}

func WatchHumidity(periodSeconds int, pin int) (<-chan SensorEvent, error) {
	events := make(chan (SensorEvent))

	go func() {
		for {
			_, humidity, err := dht.ReadDHTxx(dht.DHT11, pin, false)
			if err != nil {
				fmt.Errorf("error when reading humidity :%v", err)
			} else {
				events <- SensorEvent{"humidity", float64(humidity)}
			}
			time.Sleep(time.Duration(periodSeconds) * time.Second)
		}
	}()

	return events, nil
}

func WatchNoise(periodSeconds int, pinNumber int) (<-chan SensorEvent, error) {
	events := make(chan (SensorEvent))

	if err := rpio.Open(); err != nil {
		fmt.Errorf("error opening rpio :%v", err)
		return nil, err
	}

	pin := rpio.Pin(pinNumber)

	go func() {
		defer pin.Detect(rpio.NoEdge)
		defer rpio.Close()

		for {

			pin.Input()
			pin.PullUp()
			pin.Detect(rpio.FallEdge)

			soundEvents := 0
			for i := 0; i < 10; i++ {
				if pin.EdgeDetected() {
					fmt.Printf("sound detected %v\n", events)
					soundEvents++
				}
				time.Sleep((time.Duration(periodSeconds) * time.Second) / 10)
			}
			events <- SensorEvent{"sound_events", float64(soundEvents)}
		}
	}()

	return events, nil
}
