package smeg

import (
	"github.com/go-gl/gl/v3.3-core/gl"
)

/* OpenGL Data-Types */
type glDataType int

const (
	GlInt    = glDataType(gl.INT)
	GlUInt   = glDataType(gl.UNSIGNED_INT)
	GlFloat  = glDataType(gl.FLOAT)
	GlDouble = glDataType(gl.DOUBLE)
)

func (t glDataType) SizeOf() int {
	switch t {
	case GlInt:
		return 4
	case GlUInt:
		return 4
	case GlFloat:
		return 4
	case GlDouble:
		return 8
	default:
		return 0
	}
}

/* OpenGL Drawing Modes */
type glDrawType int

const (
	GlPoints    = glDrawType(gl.POINTS)
	GlLines     = glDrawType(gl.LINES)
	GlTriangles = glDrawType(gl.TRIANGLES)
)
