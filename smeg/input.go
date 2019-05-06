package smeg

import (
	"github.com/go-gl/glfw/v3.2/glfw"
)

type KeyEvent struct {
	Key
	KeyState
}

type Key int

const (
	KeyA = Key(glfw.KeyA)
	KeyB = Key(glfw.KeyB)
	KeyC = Key(glfw.KeyC)
	KeyD = Key(glfw.KeyD)
	KeyE = Key(glfw.KeyE)
	KeyF = Key(glfw.KeyF)
	KeyG = Key(glfw.KeyG)
	KeyH = Key(glfw.KeyH)
	KeyI = Key(glfw.KeyI)
	KeyJ = Key(glfw.KeyJ)
	KeyK = Key(glfw.KeyK)
	KeyL = Key(glfw.KeyL)
	KeyM = Key(glfw.KeyM)
	KeyN = Key(glfw.KeyN)
	KeyO = Key(glfw.KeyO)
	KeyP = Key(glfw.KeyP)
	KeyQ = Key(glfw.KeyQ)
	KeyR = Key(glfw.KeyR)
	KeyS = Key(glfw.KeyS)
	KeyT = Key(glfw.KeyT)
	KeyU = Key(glfw.KeyU)
	KeyV = Key(glfw.KeyV)
	KeyW = Key(glfw.KeyW)
	KeyX = Key(glfw.KeyX)
	KeyY = Key(glfw.KeyY)
	KeyZ = Key(glfw.KeyZ)
)

type KeyState int

const (
	KeyPress   = KeyState(glfw.Press)
	KeyRelease = KeyState(glfw.Release)
	KeyRepeat  = KeyState(glfw.Release)
)
