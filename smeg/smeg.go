package smeg

import (
	"runtime"
)

type VertexArray interface {
	VertexData() interface{}
	VertexCount() int
	Indices() []uint32
}

/* Vertex Format */
type VertexFormat []struct {
	Name  string
	Count int
	Type  glDataType
}

func init() {
	runtime.LockOSThread()
}
