#version 400

in vec4 vposition;

uniform float time;

uniform mat4 mvp;

out vec3 normal;

void main() {

	// sum of sines
	float dy = 0.0;

	float amplitude = 0.1;
	float frequency = 10.0;
	float speed = 1.0;

	float partialD = 0.0;

	for (int i = 0; i < 2; i++) {
		dy += amplitude * sin(frequency * (vposition.x + vposition.z) + speed * time);

		partialD += amplitude * frequency * cos(frequency * (vposition.x + vposition.z) + speed * time);

		amplitude *= 0.82;
		frequency *= 1.18;
	}

	gl_Position = mvp * vec4(vposition.x, vposition.y + dy, vposition.z, 1.0);

	vec3 partialDerivativeX = vec3(1.0, partialD, 0.0);
	vec3 partialDerivativeZ = vec3(0.0, partialD, 1.0);

	normal = normalize(cross(partialDerivativeZ, partialDerivativeX));
}