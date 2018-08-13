package main

// #include <stdlib.h>
// #include <stdint.h>
import "C"

import (
	"reflect"
	"unsafe"
)

// this is an unsafe way to create a slice pointing to memory
// allocated in C, pointed to by the ptr ... but it allows working
// with "native" byte-slices in go
func makeslice(ptr *C.uint8_t, length int) (slice []byte) {
	header := (*reflect.SliceHeader)(unsafe.Pointer(&slice))
	header.Cap = length
	header.Len = length
	header.Data = uintptr(unsafe.Pointer(ptr))
	return
}
