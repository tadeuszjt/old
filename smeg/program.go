package smeg

import (
	"fmt"
	"github.com/go-gl/gl/v3.3-core/gl"
)

type Program struct {
	VertexSrc   string
	GeometrySrc string
	FragmentSrc string
	VertexFormat
	DrawType glDrawType
}

type programHandle struct {
	vao      uint32
	glID     uint32
	drawType glDrawType
	vertSize int
}

func (w *window) AddProgram(name string, p Program) {
	_, prs := w.programs[name]
	if prs {
		fmt.Println("Error: program name", "'"+name+"'", "already in use")
		return
	}

	w.takeGLContext()
	id, err := makeProgram(p.VertexSrc, p.FragmentSrc, p.GeometrySrc)
	if err != nil {
		fmt.Println("Error: failed to create program", "'"+name+"':", err)
		return
	}

	var prog programHandle
	prog.glID = id
	prog.drawType = p.DrawType

	gl.GenVertexArrays(1, &prog.vao)
	gl.BindVertexArray(prog.vao)
	gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, w.ebo)

	/* get vertex size in bytes */
	vertexSize := 0
	for _, attrib := range p.VertexFormat {
		vertexSize += attrib.Type.SizeOf() * attrib.Count
	}
	prog.vertSize = vertexSize

	/* set up attributes */
	offset := 0
	for _, attrib := range p.VertexFormat {
		loc := gl.GetAttribLocation(id, gl.Str(attrib.Name+"\x00"))
		if loc < 0 {
			fmt.Println("Warning: attribute", "'"+attrib.Name+"'", "not located. (attribute unused?)")
			continue
		}

		gl.EnableVertexAttribArray(uint32(loc))
		gl.VertexAttribPointer(
			uint32(loc),
			int32(attrib.Count),
			uint32(attrib.Type),
			false,
			int32(vertexSize),
			gl.PtrOffset(offset),
		)

		offset += attrib.Type.SizeOf() * attrib.Count
	}

	w.programs[name] = prog
}

func (w *window) UseProgram(name string) {
	prog, prs := w.programs[name]
	if !prs {
		fmt.Println("Error: program", name, "not found")
		return
	}

	if prog.glID != w.currProgram.glID {
		w.takeGLContext()
		gl.UseProgram(prog.glID)
		gl.BindVertexArray(prog.vao)
		w.currProgram = prog
	}
}

func (w *window) SetUniformMat4(name string, m [16]float32) {
	w.takeGLContext()
	loc := gl.GetUniformLocation(w.currProgram.glID, gl.Str(name+"\x00"))
	if loc < 0 {
		fmt.Println("Error: uniform", name, "not located")
		return
	}

	gl.UniformMatrix4fv(loc, 1, true, &m[0])
}
