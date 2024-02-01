#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "log.hpp"
#include "vendor/stb/stb_image.hpp"

struct Texture {
	GLuint texture;
	const char* path;
	stbi_uc* buffer;
	int width, height;
	int channels;

	Texture(const char* path) : path(path) {
		// OpenGL expects the 0.0 coordinate on the Y-axis to be on the bottom
		stbi_set_flip_vertically_on_load(true);

		const auto desired_channels = 4;

		buffer = stbi_load(path, &width, &height, &channels, desired_channels);

		GL_CALL(glGenTextures(1, &texture));

		bind();

		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));

		unbind();

		if (buffer)
			stbi_image_free(buffer);
		else
			gl_log_error("Failed to load texture: %s", path);
	}

	~Texture() {
		GL_CALL(glDeleteTextures(1, &texture));
	}

	void bind(unsigned int slot = 0) {
		GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));
	}

	void unbind() {
		GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	}
};