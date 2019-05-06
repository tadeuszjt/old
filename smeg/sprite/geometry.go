package sprite

import (
	"github.com/tadeuszjt/geom"
)

type geometry struct {
	length      int
	orientation []geom.Ori2
	rectangle   []geom.Rect
}

func (g *geometry) push(orientation geom.Ori2, rectangle geom.Rect) {
	g.orientation = append(g.orientation, orientation)
	g.rectangle = append(g.rectangle, rectangle)
	g.length++
}

func (g *geometry) pop() {
	end := g.length - 1
	g.orientation = g.orientation[:end]
	g.rectangle = g.rectangle[:end]
	g.length--
}

func (g *geometry) set(index int, orientation geom.Ori2, rectangle geom.Rect) {
	g.orientation[index] = orientation
	g.rectangle[index] = rectangle
}
