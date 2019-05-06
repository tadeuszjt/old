package main

import (
	"fmt"
	"github.com/tadeuszjt/geom"
	"github.com/tadeuszjt/geom/transforms"
	"github.com/tadeuszjt/smeg"
	"github.com/tadeuszjt/smeg/sprite"
)

func main() {
	win := smeg.CreateWindow()
	defer win.Delete()
	win.AddProgram("sprite", sprite.SmegProgram)
	win.UseProgram("sprite")
	tex, err := win.AddTexture("fish.png")
	if err != nil {
		print(err)
		return
	}
	win.UseTexture(tex)
	win.SetMouseButtonCallback(mouseFunc)
	win.SetMouseMoveCallback(mouseFunc)

	var f Fishes
	f.bodies.Gravity = geom.Ori2{0, 0, 1}
	arena := geom.RectCentre(1000, 1000)
	for i := 0; i < 100; i++ {
		f.Add(geom.Ori2Rand(arena))
	}

	f.bodies.Drag = 1

	f.Delete(1)
	f.Delete(2)
	f.Delete(1)

	for win.IsOpen() {
		win.Clear()

		f.Update(1. / 60)

		w, h := win.GetSize()
		winRect := geom.RectOrigin(float64(w), float64(h))
		glRect := geom.RectCentre(2, -2)
		camRect := geom.RectCentre(float64(w), float64(h))
		viewMat := transforms.Cam2DToDisplay(camRect, glRect, geom.Ori2{})
		win.SetUniformMat4("viewmat", Mat33To16Float32(viewMat))

		if mouse.clicked {
			mouse.clicked = false
			mWorld := transforms.Cam2DToCamera(camRect, winRect, geom.Ori2{}).TimesVec2(
				mouse.pos, 1,
			)
			fmt.Println(mouse.pos, mWorld)
		}

		win.Draw(&f.sprites)
		win.Display()
		win.PollEvents()
	}
}
