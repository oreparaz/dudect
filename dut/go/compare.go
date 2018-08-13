package main

import "crypto/subtle"

// prepare byte slices .. some with zeroes, some with random data
func prepareData(data *[][]byte, classes *[]byte) {

	for i := range *data {
		(*classes)[i] = randombit()
		if (*classes)[i] == 1 {
			randombytes(&(*data)[i])
		}
	}

}

// do work, compare to byte slices
func comparebyteslices(a, b []byte) int {

	// probably not constant time
	//return bytes.Compare(a, b)

	// should be constant time
	return subtle.ConstantTimeCompare(a, b)

	// very much not constant time
	// for i := range a {
	// 	if a[i] != b[i] {
	// 		return 1
	// 	}
	// }
	// return

}
