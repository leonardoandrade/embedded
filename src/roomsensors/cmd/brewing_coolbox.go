package main

import (
	"fmt"
	"os"
)

func saveEvent(event SensorEvent, conn InfluxDBConnection) {
	err := SendEventToInfluxDB(conn, event)
	if err != nil {
		fmt.Println("ERROR: %v", err)
	}
}

func main() {

	influxDBConn := InfluxDBConnection{
		Host:     os.Getenv("INFLUXDB_HOST"),
		Database: "brewing",
		Username: os.Getenv("INFLUXDB_USER"),
		Password: os.Getenv("INFLUXDB_PASSWORD"),
		Table:    "brewing",
	}

	samplingPeriod := 3
	temperatureInsideEvents, err := WatchTemperature(samplingPeriod, "temperature_inside")
	temperatureOutsideEvents, err := WatchTemperatureDHT11(samplingPeriod, "temperature_outside", 18)
	//humidityEvents, err := WatchHumidity(samplingPeriod, "humidity", 18)
	if err != nil {
		fmt.Errorf("%v", err)
	}

	relayChan, _ := RegulateTemperature(19, 22, 2.0)

	for {
		select {
		case event := <-temperatureInsideEvents:
			saveEvent(event, influxDBConn)
			relayChan <- event.Value
			break
		case event := <-temperatureOutsideEvents:
			saveEvent(event, influxDBConn)
			break
			//case event := <-humidityEvents:
			//	saveEvent(event, influxDBConn)
			//		break
		}
	}
}
