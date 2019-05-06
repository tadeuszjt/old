package smeg

const vertexSource = `
#version 150
in vec3 vertex;
in vec4 colour;
in vec2 texcoord;

out vec4 vcolour;
out vec2 vtexcoord;
uniform mat4 viewmat;

void main() {
	vcolour = colour;
	vtexcoord = texcoord;
	gl_Position = viewmat * vec4(vertex, 1.0);
}` + "\x00"

const fragmentSource = `
#version 150

in vec2 vtexcoord;
in vec4 vcolour;

uniform sampler2D tex;
out vec4 fcolour;

void main() {
	fcolour = texture(tex, vtexcoord) * vcolour;
	if (fcolour.a < 0.1) {
		discard;
	}
}` + "\x00"
