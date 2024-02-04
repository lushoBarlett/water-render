#include <assert.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "log.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "renderer.hpp"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

static void fps(GLFWwindow* window) {
	static double previous_seconds = glfwGetTime();
	static int frame_count;

	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;

	if (elapsed_seconds > 0.25) {
		previous_seconds = current_seconds;

		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		sprintf(tmp, "opengl @ fps: %.2f", fps);
		GL_CALL(glfwSetWindowTitle(window, tmp));

		frame_count = 0;
	}

	frame_count++;
}

int main() {
	const int VERTEX_SIZE = 5;
	const int SIZE = 2;

	float tesselated_plane[VERTEX_SIZE * (SIZE + 1) * (SIZE + 1)];
	unsigned int tesselated_plane_indices[6 * SIZE * SIZE];

	const float Z = 1.0f;

	int p = 0;
	for (int x = 0; x < SIZE + 1; x++) {
		for (int y = 0; y < SIZE + 1; y++) {
			tesselated_plane[p++] = x;
			tesselated_plane[p++] = y;
			tesselated_plane[p++] = Z;

			tesselated_plane[p++] = x / (float)SIZE;
			tesselated_plane[p++] = y / (float)SIZE;
		}
	}

	int i = 0;
	for (int x = 0; x < SIZE; x++) {
		for (int y = 0; y < SIZE; y++) {
			auto zero = x * (SIZE + 1) + y;
			auto one = zero + 1;
			auto two = (x + 1) * (SIZE + 1) + y;
			auto three = two + 1;

			/**
			 * .---.
			 * | /
			 * .
			 */
			tesselated_plane_indices[i++] = zero;
			tesselated_plane_indices[i++] = one;
			tesselated_plane_indices[i++] = three;

			/**
			 *     .
			 *   / |
			 * .---.
			 */
			tesselated_plane_indices[i++] = zero;
			tesselated_plane_indices[i++] = two;
			tesselated_plane_indices[i++] = three;
		}
	}

	for (int i = 0; i < sizeof(tesselated_plane) / sizeof(float); i++)
		tesselated_plane[i] *= 0.2f;

	Renderer* renderer = new Renderer(640, 480, "water");
	renderer->start_window();
	GLFWwindow* window = renderer->get_window();

	renderer->blend();

	Mesh* plane = new Mesh();
	plane->data(sizeof(tesselated_plane), tesselated_plane, GL_STATIC_DRAW);
	plane->attributes<float>(3, false, VERTEX_SIZE * sizeof(float));
	plane->attributes<float>(2, false, VERTEX_SIZE * sizeof(float));
	plane->indices(sizeof(tesselated_plane_indices), tesselated_plane_indices, GL_STATIC_DRAW);
	plane->mode(GL_TRIANGLES);

	const int texture_slot = 0;

	Texture* texture = new Texture("texture.png");
	texture->bind(texture_slot);

	Shader* shader = new Shader("plane.vert", "plane.frag");

	Location1F utime = shader->uniform1f("time");
	utime.set(0.0f);

	Location1I utex = shader->uniform1i("tex");
	utex.set(texture_slot);

	glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));

	glm::mat4 mvp = projection * view * model;

	LocationMat4F umvp = shader->uniformMat4f("mvp");
	umvp.set(&mvp);

	while (!renderer->window_should_close()) {

		fps(window);

		renderer->clear();
		renderer->render(plane, shader);

		utime.set(utime.get() + 1.0f / 60.0f);

		renderer->swap_buffers();
		renderer->poll_events();

		if (renderer->pressed(GLFW_KEY_ESCAPE))
			renderer->close_window();

		if (renderer->pressed(GLFW_KEY_R))
			shader->reload();
	}

	delete plane;
	delete shader;
	delete texture;
	delete renderer;
}
