#version 400

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec2 texcoord;

uniform float time;

uniform mat4 mvp;

out vec2 vtexcoord;

void main() {
	gl_Position = mvp * vposition;
	vtexcoord = texcoord;
}