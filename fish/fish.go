package main

import (
	"github.com/tadeuszjt/geom"
	"github.com/tadeuszjt/phys2D"
	"github.com/tadeuszjt/smeg/sprite"
)

type Fishes struct {
	sprites sprite.Sprites
	bodies  phys2D.World

	length     int
	spriteKeys []sprite.Key
	bodyKeys   []phys2D.Key
	jointKeys  []phys2D.Key
}

func (f *Fishes) push(orientation geom.Ori2) {
	o := [3]geom.Ori2{
		{0, 0, 0},
		{-20, 0, 0},
		{-40, 0, 0},
	}
	for i := range o {
		o[i] = orientation.ToWorld(o[i])
	}

	// sprites
	s := [3]sprite.Key{}
	for i := range s {
		s[i] = f.sprites.AddSprite(o[i], geom.RectCentre(20, 20))
	}
	f.sprites.SetTextureRect(s[2], geom.Rect{geom.Vec2{0, 0}, geom.Vec2{1. / 3., 1}})
	f.sprites.SetTextureRect(s[1], geom.Rect{geom.Vec2{1. / 3., 0}, geom.Vec2{2. / 3., 1}})
	f.sprites.SetTextureRect(s[0], geom.Rect{geom.Vec2{2. / 3., 0}, geom.Vec2{3. / 3., 1}})
	f.spriteKeys = append(f.spriteKeys, s[:]...)

	// bodies
	m := [3]geom.Ori2{
		{1, 1, 66},
		{1, 1, 66},
		{1, 1, 66},
	}
	d := [3]geom.Mat33{
		{
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1},
		},
		{
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1},
		},
		{
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1},
		},
	}

	b := [3]phys2D.Key{
		f.bodies.AddBody(o[0], m[0], d[0]),
		f.bodies.AddBody(o[1], m[1], d[1]),
		f.bodies.AddBody(o[2], m[2], d[2]),
	}
	f.bodyKeys = append(f.bodyKeys, b[:]...)

	// joints
	f.jointKeys = append(f.jointKeys,
		f.bodies.AddJoint(b[0], b[1], geom.Vec2{-10, 0}, geom.Vec2{10, 0}),
		f.bodies.AddJoint(b[1], b[2], geom.Vec2{-10, 0}, geom.Vec2{10, 0}),
	)

	f.length++
}

func (f *Fishes) pop() {
	sEnd := len(f.spriteKeys) - 1
	f.sprites.Delete(
		f.spriteKeys[sEnd-0],
		f.spriteKeys[sEnd-1],
		f.spriteKeys[sEnd-2],
	)
	f.spriteKeys = f.spriteKeys[:len(f.spriteKeys)-3]

	bEnd := len(f.bodyKeys) - 1
	f.bodies.DeleteBody(
		f.bodyKeys[bEnd-0],
		f.bodyKeys[bEnd-1],
		f.bodyKeys[bEnd-2],
	)
	f.bodyKeys = f.bodyKeys[:len(f.bodyKeys)-3]

	jEnd := len(f.jointKeys) - 1
	f.bodies.DeleteJoint(
		f.jointKeys[jEnd-0],
		f.jointKeys[jEnd-1],
	)
	f.jointKeys = f.jointKeys[:len(f.jointKeys)-2]

	f.length--
}

func (f *Fishes) swap(i, j int) {
	i3 := i * 3
	j3 := j * 3
	f.spriteKeys[i3+0], f.spriteKeys[j3+0] = f.spriteKeys[j3+0], f.spriteKeys[i3+0]
	f.spriteKeys[i3+1], f.spriteKeys[j3+1] = f.spriteKeys[j3+1], f.spriteKeys[i3+1]
	f.spriteKeys[i3+2], f.spriteKeys[j3+2] = f.spriteKeys[j3+2], f.spriteKeys[i3+2]
	f.bodyKeys[i3+0], f.bodyKeys[j3+0] = f.bodyKeys[j3+0], f.bodyKeys[i3+0]
	f.bodyKeys[i3+1], f.bodyKeys[j3+1] = f.bodyKeys[j3+1], f.bodyKeys[i3+1]
	f.bodyKeys[i3+2], f.bodyKeys[j3+2] = f.bodyKeys[j3+2], f.bodyKeys[i3+2]
	f.jointKeys[i*2+0], f.jointKeys[j*2+0] = f.jointKeys[j*2+0], f.jointKeys[i*2+0]
	f.jointKeys[i*2+1], f.jointKeys[j*2+1] = f.jointKeys[j*2+1], f.jointKeys[i*2+1]
}

func (f *Fishes) Add(orientation geom.Ori2) {
	f.push(orientation)
}

func (f *Fishes) Delete(index int) {
	end := f.length - 1
	if index != end {
		f.swap(index, end)
	}
	f.pop()
}

func (f *Fishes) BodyAt(pos geom.Vec2) {
}

func (f *Fishes) Update(dt float64) {
	f.bodies.Update(dt)
	f.sprites.SetOrientations(f.spriteKeys, f.bodies.GetOrientations(f.bodyKeys))
	f.sprites.Update()
}
