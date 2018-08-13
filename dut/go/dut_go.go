package main

/*
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

*/
import "C"

import (
	"crypto/rand"
	"crypto/subtle"
	"reflect"
	"unsafe"
)

func randombit() uint8 {
	b := make([]byte, 1)
	_, err := rand.Read(b)
	if err != nil {
		panic(err)
	}
	return uint8(b[0] & 0x01)
}

func randombytes(b *[]byte) {
	_, err := rand.Read(*b)
	if err != nil {
		panic(err)
	}
}

const chunk_size C.size_t = 16
const number_measurements C.size_t = 1e6

var secret = make([]byte, chunk_size)
var indata = make([][]byte, number_measurements)
var idx = 0

//export init_dut
func init_dut() {}

//export do_one_computation
func do_one_computation(data *C.uint8_t) C.uint8_t {

	//bytes.Compare(secret, indata[idx])
	comp := subtle.ConstantTimeCompare(secret, indata[idx])
	idx++
	return (C.uint8_t)(uint8(comp))

}

//export prepare_inputs
func prepare_inputs(input *C.uint8_t, classesptr *C.uint8_t) {

	idx = 0

	var classes []uint8
	sliceHeader := (*reflect.SliceHeader)((unsafe.Pointer(&classes)))
	sliceHeader.Cap = int(number_measurements)
	sliceHeader.Len = int(number_measurements)
	sliceHeader.Data = uintptr(unsafe.Pointer(classesptr))

	for i := range indata {
		indata[i] = make([]byte, chunk_size)
		classes[i] = randombit()
		if classes[i] == 1 {
			randombytes(&indata[i])
		}
	}

}

func main() {}
