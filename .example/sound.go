/*
An example of edge event handling by @Drahoslav7, using the go-rpio library
Waits for button to be pressed twice before exit.
Connect a button between pin 22 and some GND pin.
*/

package main

import (
	"fmt"
	"github.com/stianeikeland/go-rpio"
	"os"
	"time"
)

var (
	pin = rpio.Pin(21)
)

func main() {
	// Open and map memory to access gpio, check for errors
	if err := rpio.Open(); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	// Unmap gpio memory when done

	defer pin.Detect(rpio.NoEdge) // disable edge event detection
	defer rpio.Close()

	pin.Input()
	pin.PullUp()
	pin.Detect(rpio.FallEdge) // enable falling edge event detection

	fmt.Println("detect sound")

	events := 0
	for {
		if pin.EdgeDetected() { // check if event occured
			fmt.Printf("sound detected %v\n", events)
			events++
		}
		time.Sleep(time.Second / 2)
	}

}
