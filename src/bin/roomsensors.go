package main

import (
	"data"
	"fmt"
	"model"
	"os"
	"sensors"
)

func _main() {

	influxDBConn := data.InfluxDBConnection{
		Host:     os.Getenv("INFLUXDB_HOST"),
		Database: "house_sensors",
		Username: os.Getenv("INFLUXDB_USER"),
		Password: os.Getenv("INFLUXDB_PASSWORD"),
		Table:    "living_room",
	}

	samplingPeriod := 3
	temperatureEvents, err := sensors.WatchTemperature(samplingPeriod, "temperature")
	humidityEvents, err := sensors.WatchHumidity(samplingPeriod, "humidity", 18)
	noiseEvents, err := sensors.WatchNoise(samplingPeriod, 21)
	if err != nil {
		fmt.Errorf("%v", err)
	}

	for {
		var event = model.SensorEvent{}
		select {
		case event = <-temperatureEvents:
			break
		case event = <-humidityEvents:
			break
		case event = <-noiseEvents:
			break
		}
		fmt.Printf("event: %v\n", event)
		err := data.SendEventToInfluxDB(influxDBConn, event)
		if err != nil {
			fmt.Errorf("%v", err)
		}
	}
}
