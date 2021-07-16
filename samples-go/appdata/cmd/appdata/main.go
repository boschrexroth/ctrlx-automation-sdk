package main

import "fmt"

func main() {

	err := RestApiStart()
	if err != nil {
		fmt.Printf("An error has occurred: %s", err.Error())
	}

}
