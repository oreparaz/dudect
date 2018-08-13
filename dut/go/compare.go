package main

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

	var comp int

	//bytes.Compare(secret, indata[idx])
	//comp := subtle.ConstantTimeCompare(secret, indata[idx])

	// very much not constant time
	for i := range a {
		if a[i] != b[i] {
			comp = 1
			break
		}
	}

	return comp

}
