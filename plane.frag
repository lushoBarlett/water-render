#version 400

in vec2 vtexcoord;

uniform sampler2D tex;

out vec4 color;

void main() {
	vec4 texcolor = texture(tex, vtexcoord);
	color = texcolor;
}