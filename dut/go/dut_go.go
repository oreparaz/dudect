package main

// #include <stdlib.h>
// #include <stdint.h>
import "C"
import (
	"runtime"
)

// if you change these values, change them in dut_go.c aswell!
const chunksize = 16
const measurements = 1e6

// just a slice full of zeroes for now ...
var secret = make([]byte, chunksize)

//export init_dut
func init_dut() {}

//export do_one_computation
func do_one_computation(dataptr *C.uint8_t) C.uint8_t {

	data := makeslice(dataptr, chunksize)
	return (C.uint8_t)(uint8(comparebyteslices(secret, data)))

}

//export prepare_inputs
func prepare_inputs(inputptr *C.uint8_t, classesptr *C.uint8_t) {

	// create slice abstractions
	allinputs := makeslice(inputptr, chunksize*measurements)
	classes := makeslice(classesptr, measurements)
	inputs := make([][]byte, measurements)
	for i := range inputs {
		inputs[i] = allinputs[i*chunksize : (i+1)*chunksize]
	}

	prepareData(&inputs, &classes)

	// run GC now in an attempt to not have
	// it run during computations too much
	runtime.GC()

}

func main() {}
