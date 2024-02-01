#version 400

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec2 texcoord;

uniform float time;

out vec2 vtexcoord;

void main() {
	gl_Position = vposition;
	vtexcoord = texcoord;
}