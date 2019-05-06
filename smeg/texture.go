package smeg

import (
	"github.com/go-gl/gl/v3.3-core/gl"
	"image"
	"image/draw"
	_ "image/png"
	"os"
)

func (w *window) AddTexture(filename string) (uint32, error) {
	w.takeGLContext()

	file, err := os.Open(filename)
	if err != nil {
		return 0, err
	}

	img, _, err := image.Decode(file)
	file.Close()
	if err != nil {
		return 0, err
	}

	rgba := image.NewRGBA(img.Bounds())
	draw.Draw(rgba, rgba.Bounds(), img, image.Point{0, 0}, draw.Src)

	// Generate texture
	var tex uint32
	gl.GenTextures(1, &tex)
	gl.BindTexture(gl.TEXTURE_2D, tex)
	gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT)
	gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT)
	gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR)
	gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR)

	// Upload
	gl.TexImage2D(
		gl.TEXTURE_2D,
		0,
		gl.RGBA,
		int32(rgba.Rect.Size().X),
		int32(rgba.Rect.Size().Y),
		0,
		gl.RGBA,
		gl.UNSIGNED_BYTE,
		gl.Ptr(rgba.Pix),
	)

	return tex, nil
}

func (w *window) UseTexture(texture uint32) {
	w.takeGLContext()
	gl.BindTexture(gl.TEXTURE_2D, texture)
}
