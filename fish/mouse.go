package main

import (
	"github.com/tadeuszjt/geom"
	"github.com/tadeuszjt/smeg"
)

var mouse struct {
	pos           geom.Vec2
	held, clicked bool
}

func mouseFunc(ev smeg.MouseEvent) {
	mouse.pos = geom.Vec2{ev.Position.X, ev.Position.Y}
	if ev.State == smeg.MousePress {
		if mouse.held == false { // rising edge
			mouse.clicked = true
			mouse.held = true
		}
	} else if ev.State == smeg.MouseRelease {
		mouse.held = false
	}
}
