package sprite

import (
	"github.com/tadeuszjt/geom"
	"math"
)

type Key int

type Sprites struct {
	geometry
	elements
	elemToGeom stack
	geomToElem stack
	emptyGeom  stack
}

func (s *Sprites) AddSprite(orientation geom.Ori2, rectangle geom.Rect) Key {
	elemIndex := s.elements.length
	geomIndex := 0

	for {
		// append new
		if len(s.emptyGeom) == 0 {
			geomIndex = s.geometry.length
			s.geometry.push(orientation, rectangle)
			s.geomToElem.push(elemIndex)
			break
		}

		// use empty
		geomIndex = s.emptyGeom.pop()
		if geomIndex == s.geometry.length-1 { // eat
			s.geometry.pop()
			s.geomToElem.pop()
			continue
		}

		s.geometry.set(geomIndex, orientation, rectangle)
		s.geomToElem[geomIndex] = elemIndex
		break
	}

	s.elements.push()
	s.elemToGeom.push(geomIndex)
	return Key(geomIndex)
}

func (s *Sprites) Delete(keys ...Key) {
	for _, key := range keys {
		geomIndex := int(key)
		elemIndex := s.geomToElem[geomIndex]

		if geomIndex == s.geometry.length-1 {
			s.geometry.pop()
			s.geomToElem.pop()
		} else {
			s.emptyGeom.push(geomIndex)
		}

		endElemIndex := s.elements.length - 1
		if elemIndex != endElemIndex {
			endGeomIndex := s.elemToGeom[endElemIndex]
			s.geomToElem[endGeomIndex] = elemIndex
			s.elemToGeom[elemIndex] = endGeomIndex
			s.elements.swap(elemIndex, endElemIndex)
		}
		s.elements.pop()
		s.elemToGeom.pop()
	}
}

func (s *Sprites) SetOrientations(keys []Key, orientations []geom.Ori2) {
	for i := range keys {
		s.geometry.orientation[keys[i]] = orientations[i]
	}
}

func (s *Sprites) Orientations() []geom.Ori2 {
	return s.orientation
}

func (s *Sprites) Update() {
	for elemIndex, geomIndex := range s.elemToGeom {
		verts := s.geometry.rectangle[geomIndex].Vertices()
		orientation := s.geometry.orientation[geomIndex]

		// rotate
		if orientation.Theta != 0.0 {
			s := math.Sin(orientation.Theta)
			c := math.Cos(orientation.Theta)
			for i, v := range verts {
				verts[i] = geom.Vec2{
					c*v.X - s*v.Y,
					s*v.X + c*v.Y,
				}
			}
		}

		// translate
		for i := range verts {
			verts[i].PlusEquals(orientation.Vec2())
		}

		// set vertex
		vi := elemIndex * 4
		for i := 0; i < 4; i++ {
			s.elements.vertexs[vi+i].position = vec2{
				float32(verts[i].X),
				float32(verts[i].Y),
			}
		}
	}
}

/* smeg stuff */
func (s *Sprites) VertexData() interface{} {
	return s.elements.vertexs
}

func (s *Sprites) VertexCount() int {
	return len(s.elements.vertexs)
}

func (s *Sprites) Indices() []uint32 {
	return s.elements.indices
}
