package sprite

import (
	"github.com/tadeuszjt/smeg"
)

type vec2 [2]float32

type vertex struct {
	position vec2
	texCoord vec2
	colour   [4]float32
}

var (
	SmegProgram = smeg.Program{
		VertexSrc: `
		#version 150
		in vec2 position;
		in vec2 texcoord;
		in vec4 colour;
		out vec2 vtexcoord;
		out vec4 vcolour;
		uniform mat4 viewmat;
		void main() {
			vtexcoord = texcoord;
			vcolour = colour;
			gl_Position = viewmat * vec4(position, 0, 1);
		}`,
		FragmentSrc: `
		#version 150
		in vec2 vtexcoord;
		in vec4 vcolour;
		out vec4 fcolour;
		uniform sampler2D tex;
		void main() {
			fcolour = texture(tex, vtexcoord) * vcolour;
			if (fcolour.a < 0.1) {
				discard;
			}
		}`,
		VertexFormat: smeg.VertexFormat{
			{"position", 2, smeg.GlFloat},
			{"texcoord", 2, smeg.GlFloat},
			{"colour", 4, smeg.GlFloat},
		},
		DrawType: smeg.GlTriangles,
	}
)
