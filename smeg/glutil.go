package smeg

import (
	"fmt"
	"github.com/go-gl/gl/v3.3-core/gl"
	"strings"
)

func compileShader(source string, shaderType uint32) (uint32, error) {
	shader := gl.CreateShader(shaderType)

	csources, free := gl.Strs(source + "\x00")
	gl.ShaderSource(shader, 1, csources, nil)
	free()
	gl.CompileShader(shader)

	var status int32
	gl.GetShaderiv(shader, gl.COMPILE_STATUS, &status)
	if status == gl.FALSE {
		var logLength int32
		gl.GetShaderiv(shader, gl.INFO_LOG_LENGTH, &logLength)

		log := strings.Repeat("\x00", int(logLength+1))
		gl.GetShaderInfoLog(shader, logLength, nil, gl.Str(log))

		return 0, fmt.Errorf("shader failed to compile:%v\n%v", source, log)
	}

	return shader, nil
}

func makeProgram(vertexSrc, fragmentSrc, geometrySrc string) (uint32, error) {
	vertexShader, err := compileShader(vertexSrc, gl.VERTEX_SHADER)
	defer gl.DeleteShader(vertexShader)
	if err != nil {
		return 0, err
	}

	fragmentShader, err := compileShader(fragmentSrc, gl.FRAGMENT_SHADER)
	defer gl.DeleteShader(fragmentShader)
	if err != nil {
		return 0, err
	}

	program := gl.CreateProgram()

	gl.AttachShader(program, vertexShader)
	gl.AttachShader(program, fragmentShader)

	// optional geometry shader
	if geometrySrc != "" {
		geometryShader, err := compileShader(geometrySrc, gl.GEOMETRY_SHADER)
		defer gl.DeleteShader(geometryShader)
		if err != nil {
			return 0, err
		}
		gl.AttachShader(program, geometryShader)
	}

	gl.LinkProgram(program)

	var status int32
	gl.GetProgramiv(program, gl.LINK_STATUS, &status)
	if status == gl.FALSE {
		var logLength int32
		gl.GetProgramiv(program, gl.INFO_LOG_LENGTH, &logLength)

		log := strings.Repeat("\x00", int(logLength+1))
		gl.GetProgramInfoLog(program, logLength, nil, gl.Str(log))

		return 0, fmt.Errorf("failed to link program: %v", log)
	}

	return program, nil
}
