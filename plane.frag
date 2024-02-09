#version 400

in vec3 normal;

out vec4 fragColor;

void main() {
	vec3 light = normalize(vec3(0.5, 0.5, 0.5));

	float lambertian = max(dot(normal, light), 0.0);

	vec3 ambient = vec3(0.1, 0.3, 0.1);

	vec3 viewDir = normalize(vec3(-0.5, 0.1, 0.5));

	vec3 specular = vec3(0.1, 0.1, 0.1) * pow(dot(light + viewDir, normal), 16);

	vec3 color = vec3(0.1, 0.3, 1.0) * lambertian + ambient + specular;

	fragColor = vec4(color, 1.0);
}