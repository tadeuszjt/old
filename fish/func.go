package main

import (
	"github.com/tadeuszjt/geom"
)

func Mat33To16Float32(m geom.Mat33) [16]float32 {
	return [16]float32{
		float32(m[0][0]), float32(m[0][1]), 0, float32(m[0][2]),
		float32(m[1][0]), float32(m[1][1]), 0, float32(m[1][2]),
		0, 0, 1, 0,
		0, 0, 0, 1,
	}
}
