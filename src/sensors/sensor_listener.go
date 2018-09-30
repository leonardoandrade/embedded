package sensors

import (
	"fmt"
	"github.com/d2r2/go-dht"
	"github.com/stianeikeland/go-rpio"
	"github.com/yryz/ds18b20"
	"model"
	"time"
)

func WatchTemperature(periodSeconds int, sensorName string) (<-chan model.SensorEvent, error) {
	return WatchTemperatureBySensorIndex(periodSeconds, sensorName, 0)
}

func WatchTemperatureBySensorIndex(periodSeconds int, sensorName string, sensorIndex int) (<-chan model.SensorEvent, error) {
	events := make(chan (model.SensorEvent))
	sensors, err := ds18b20.Sensors()
	if err != nil {
		return nil, err
	}

	go func() {
		for {
			t, err := ds18b20.Temperature(sensors[sensorIndex])
			if err != nil {
				fmt.Errorf("error when reading temperature:%v", err)
			} else {
				events <- model.SensorEvent{sensorName, t}
			}
			time.Sleep(time.Duration(periodSeconds) * time.Second)
		}
	}()

	return events, nil
}

func WatchTemperatureDHT11(periodSeconds int, sensorName string, pin int) (<-chan model.SensorEvent, error) {
	events := make(chan (model.SensorEvent))

	go func() {
		for {
			temperature, _, err := dht.ReadDHTxx(dht.DHT11, pin, false)
			if err != nil {
				fmt.Printf("error when reading temperature :%v\n", err)
			} else {
				events <- model.SensorEvent{sensorName, float64(temperature)}
			}
			time.Sleep(time.Duration(periodSeconds) * time.Second)
		}
	}()

	return events, nil
}

func WatchHumidity(periodSeconds int, sensorName string, pin int) (<-chan model.SensorEvent, error) {
	events := make(chan (model.SensorEvent))

	go func() {
		for {
			_, humidity, err := dht.ReadDHTxx(dht.DHT11, pin, false)
			if err != nil {
				fmt.Printf("error when reading humidity :%v\n", err)
			} else {
				events <- model.SensorEvent{sensorName, float64(humidity)}
			}
			time.Sleep(time.Duration(periodSeconds) * time.Second)
		}
	}()

	return events, nil
}

func WatchNoise(periodSeconds int, pinNumber int) (<-chan model.SensorEvent, error) {
	events := make(chan (model.SensorEvent))

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
			events <- model.SensorEvent{"sound_events", float64(soundEvents)}
		}
	}()

	return events, nil
}
