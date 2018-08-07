package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"strings"
)

type InfluxDBConnection struct {
	Host     string
	Database string
	Username string
	Password string
	Table    string
}

func SendEventToInfluxDB(connection InfluxDBConnection, event SensorEvent) error {

	client := &http.Client{}
	data := fmt.Sprintf("%s,sensor=%s value=%f", connection.Table, event.Sensor, event.Value)
	url := fmt.Sprintf("https://%s/influxdb/write?db=%s", connection.Host, connection.Database)

	req, err := http.NewRequest("POST", url, strings.NewReader(data))
	req.SetBasicAuth(connection.Username, connection.Password)
	resp, err := client.Do(req)
	if err != nil {
		return err
	}

	bodyText, err := ioutil.ReadAll(resp.Body)
	s := string(bodyText)
	fmt.Printf("%s\n", s)
	return nil
}
