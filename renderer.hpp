#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "mesh.hpp"
#include "shader.hpp"

struct Renderer {

	static inline size_t window_width;
	static inline size_t window_height;
	static inline const char* window_title;

	Renderer(size_t width, size_t height, const char* title) {
		window_width = width;
		window_height = height;
		window_title = title;
	}

	~Renderer() {
		if (window)
			glfwDestroyWindow(window);

		glfwTerminate();
	}

	void blend(bool enable = true, GLenum src = GL_SRC_ALPHA, GLenum dst = GL_ONE_MINUS_SRC_ALPHA) {
		if (enable) {
			GL_CALL(glEnable(GL_BLEND));
		} else {
			GL_CALL(glDisable(GL_BLEND));
		}

		GL_CALL(glBlendFunc(src, dst));
	}

	bool start_window() {
		assert(restart_gl_log());

		gl_log("starting GLFW\n%s\n", glfwGetVersionString());

		glfwSetErrorCallback(glfw_error_callback);

		if (!glfwInit())
			return false;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_SAMPLES, 4);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* videomode = glfwGetVideoMode(monitor);

		window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);

		if (!window) {
			glfwTerminate();
			return false;
		}

		glfwSetWindowSizeCallback(window, on_window_resize);

		glfwMakeContextCurrent(window);

		glewInit();

		log_gl_params();

		return true;
	}

	void clear() {
		assert(window);
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GL_CALL(glViewport(0, 0, window_width, window_height));
	}

	void render(Mesh* mesh, Shader* shader) {
		assert(window && mesh && shader);

		mesh->bind();
		shader->bind();

		mesh->draw();
	}

	void swap_buffers() {
		assert(window);
		GL_CALL(glfwSwapBuffers(window));
	}

	void poll_events() {
		GL_CALL(glfwPollEvents());
	}

	void close_window() {
		assert(window);
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	bool window_should_close() {
		assert(window);
		return glfwWindowShouldClose(window);
	}

	void culling(bool enable = true, GLenum face = GL_BACK, GLenum front = GL_CW) {
		if (enable) {
			GL_CALL(glEnable(GL_CULL_FACE));
		} else {
			GL_CALL(glDisable(GL_CULL_FACE));
		}

		GL_CALL(glCullFace(face));
		GL_CALL(glFrontFace(front));
	}

	GLFWwindow* get_window() {
		assert(window);
		return window;
	}

	GLenum pressed(GLenum key) {
		assert(window);
		return glfwGetKey(window, key);
	}

private:

	GLFWwindow* window;

	static void on_window_resize(GLFWwindow* window, int width, int height) {
		window_width = width;
		window_height = height;
	}
};