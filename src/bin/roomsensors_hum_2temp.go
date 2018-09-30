package main

import (
	//"data"
	"fmt"
	"model"
	//"os"
	"sensors"
)

func main() {

	/*influxDBConn := data.InfluxDBConnection{
		Host:     os.Getenv("INFLUXDB_HOST"),
		Database: "house_sensors",
		Username: os.Getenv("INFLUXDB_USER"),
		Password: os.Getenv("INFLUXDB_PASSWORD"),
		Table:    "living_room",
	}*/

	samplingPeriod := 3
	temperatureEvents, err := sensors.WatchTemperatureBySensorIndex(samplingPeriod, "temperature", 0)
	temperatureOutsideEvents, err := sensors.WatchTemperatureBySensorIndex(samplingPeriod, "temperature_outside", 1)
	//humidityEvents, err := sensors.WatchHumidity(samplingPeriod, "humidity", 18)

	if err != nil {
		fmt.Errorf("%v", err)
	}

	//TODO: there must be a better way to do this...
	for {
		var event = model.SensorEvent{}
		select {
		case event = <-temperatureEvents:
			break
		case event = <-temperatureOutsideEvents:
			break
			//case event = <-humidityEvents:
			//	break
		}
		fmt.Printf("event: %v\n", event)
		//err := SendEventToInfluxDB(influxDBConn, event)
		//if err != nil {
		//	fmt.Errorf("%v", err)
		//}
	}
}
