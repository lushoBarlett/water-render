#version 400

in vec4 vposition;

uniform float time;

uniform mat4 mvp;

void main() {
	gl_Position = mvp * vposition;
}