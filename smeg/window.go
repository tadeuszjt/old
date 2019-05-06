package smeg

import (
	"fmt"
	"github.com/go-gl/gl/v3.3-core/gl"
	"github.com/go-gl/glfw/v3.2/glfw"
)

var (
	contextHolder *glfw.Window = nil
	windowCount   int          = 0
)

type window struct {
	glfwWindow  *glfw.Window
	currProgram programHandle
	vbo, ebo    uint32
	programs    map[string]programHandle
	keyCallback func(Key)
}

func resize(w *glfw.Window, width, height int) {
	gl.Viewport(0, 0, int32(width), int32(height))
}

func cursorPosCallback(w *glfw.Window, xpos, ypos float64) {
}

func (w *window) SetKeyCallback(f func(KeyEvent)) {
	w.glfwWindow.SetKeyCallback(
		func(_ *glfw.Window, k glfw.Key, _ int, action glfw.Action, _ glfw.ModifierKey) {
			f(KeyEvent{Key(k), KeyState(action)})
		},
	)
}

func CreateWindow() (w window) {
	if windowCount == 0 {
		glfw.Init()
		fmt.Println("glfw initialised")
	}
	windowCount++

	glfw.WindowHint(glfw.Resizable, glfw.True)
	glfw.WindowHint(glfw.ContextVersionMajor, 3)
	glfw.WindowHint(glfw.ContextVersionMinor, 3)
	glfw.WindowHint(glfw.OpenGLProfile, glfw.OpenGLCoreProfile)
	glfw.WindowHint(glfw.OpenGLForwardCompatible, glfw.True)

	w.glfwWindow, _ = glfw.CreateWindow(640, 480, "Smeg", nil, nil)
	w.glfwWindow.SetFramebufferSizeCallback(resize)
	w.glfwWindow.SetCursorPosCallback(cursorPosCallback)
	w.takeGLContext()

	glfw.SwapInterval(1)
	gl.Init()
	gl.Enable(gl.DEPTH_TEST)
	gl.Enable(gl.BLEND)
	gl.BlendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA)
	gl.DepthFunc(gl.LESS)
	gl.ClearColor(1, 1, 1, 1)

	gl.GenBuffers(1, &w.vbo)
	gl.BindBuffer(gl.ARRAY_BUFFER, w.vbo)
	gl.GenBuffers(1, &w.ebo)

	w.programs = make(map[string]programHandle)
	w.AddProgram(
		"default",
		Program{
			VertexSrc:   vertexSource,
			FragmentSrc: fragmentSource,
			GeometrySrc: "",
			VertexFormat: VertexFormat{
				{"vertex", 3, GlFloat},
				{"colour", 4, GlFloat},
				{"texcoord", 2, GlFloat},
			},
			DrawType: GlTriangles,
		},
	)
	w.UseProgram("default")
	w.SetUniformMat4("viewmat", [16]float32{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	})

	return
}

func (w *window) IsOpen() bool {
	return !w.glfwWindow.ShouldClose()
}

func (w *window) Clear() {
	w.takeGLContext()
	gl.Clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT)
}

func (w *window) Draw(v VertexArray) {
	indices := v.Indices()
	if len(indices) <= 0 || v.VertexCount() <= 0 {
		return
	}

	w.takeGLContext()
	gl.BufferData(gl.ARRAY_BUFFER, w.currProgram.vertSize*v.VertexCount(), gl.Ptr(v.VertexData()), gl.STREAM_DRAW)

	gl.BufferData(gl.ELEMENT_ARRAY_BUFFER, len(indices)*4, gl.Ptr(indices), gl.STREAM_DRAW)
	gl.DrawElements(uint32(w.currProgram.drawType), int32(len(indices)), gl.UNSIGNED_INT, nil)
}

func (w *window) PollEvents() {
	w.takeGLContext()
	glfw.PollEvents()
}

func (w *window) Display() {
	w.glfwWindow.SwapBuffers()
}

func (w *window) GetSize() (width, height int) {
	return w.glfwWindow.GetSize()
}

func (w *window) takeGLContext() {
	if w.glfwWindow != contextHolder {
		w.glfwWindow.MakeContextCurrent()
		contextHolder = w.glfwWindow
	}
}

func (w *window) Delete() {
	w.glfwWindow.Destroy()
	windowCount--
	if windowCount == 0 {
		glfw.Terminate()
		fmt.Println("glfw terminated")
	}
}
