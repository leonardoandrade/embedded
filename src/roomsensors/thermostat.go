package main

import (
	"fmt"
	"github.com/stianeikeland/go-rpio"
)

func RegulateTemperature(pin int, targetTemperature float64, maxDeviation float64) (chan float64, error) {
	pinRelay := rpio.Pin(pin)
	rpio.Open()
	temperature := make(chan (float64))

	go func() {
		for value := range temperature {
			fmt.Printf("[Thermostat] got temperature: %f\n", value)
			if value > targetTemperature+maxDeviation {
				pinRelay.High()
			}
			if value < targetTemperature-maxDeviation {
				pinRelay.Low()
			}
		}
	}()

	return temperature, nil

}
