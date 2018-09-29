package main

import (
	"fmt"
	"os"
)

func main() {

	influxDBConn := InfluxDBConnection{
		Host:     os.Getenv("INFLUXDB_HOST"),
		Database: "house_sensors",
		Username: os.Getenv("INFLUXDB_USER"),
		Password: os.Getenv("INFLUXDB_PASSWORD"),
		Table:    "living_room",
	}

	samplingPeriod := 3
	temperatureEvents, err := WatchTemperature(samplingPeriod)
	humidityEvents, err := WatchHumidity(samplingPeriod, 18)
	noiseEvents, err := WatchNoise(samplingPeriod, 21)
	if err != nil {
		fmt.Errorf("%v", err)
	}

	for {
		var event = SensorEvent{}
		select {
		case event = <-temperatureEvents:
			break
		case event = <-humidityEvents:
			break
		case event = <-noiseEvents:
			break
		}
		fmt.Printf("event: %v\n", event)
		err := SendEventToInfluxDB(influxDBConn, event)
		if err != nil {
			fmt.Errorf("%v", err)
		}
	}
}
