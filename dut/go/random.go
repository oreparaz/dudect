package main

import "crypto/rand"

// either 0x00 or 0x01
func randombit() byte {
	b := make([]byte, 1)
	_, err := rand.Read(b)
	if err != nil {
		panic(err)
	}
	return b[0] & 0x01
}

// fill slice with random data
func randombytes(b *[]byte) {
	_, err := rand.Read(*b)
	if err != nil {
		panic(err)
	}
}
