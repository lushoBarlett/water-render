#pragma once

#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "log.hpp"

struct Mesh {

	Mesh() {
		GL_CALL(glGenVertexArrays(1, &vao));
		GL_CALL(glBindVertexArray(vao));

		GL_CALL(glGenBuffers(1, &vbo));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

		GL_CALL(glGenBuffers(1, &ibo));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

		bound = true;
	}

	~Mesh() {
		GL_CALL(glDeleteBuffers(1, &vbo));
		GL_CALL(glDeleteBuffers(1, &ibo));
		GL_CALL(glDeleteVertexArrays(1, &vao));
	}

	void bind() {
		GL_CALL(glBindVertexArray(vao));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		bound = true;
	}

	void unbind() {
		GL_CALL(glBindVertexArray(0));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		bound = false;
	}

	void data(GLsizeiptr size, const void* data, GLenum usage) {
		assert(bound && "Mesh not bound");
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
		vertex_count = size / sizeof(float); // FIXME: this is wrong
	}

	void indices(GLsizeiptr size, const unsigned int* data, GLenum usage) {
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage));
		index_count = size / sizeof(unsigned int);
	}

	// TODO: move layout setup to a different class
	template<typename T>
	void attributes(GLint size, bool normalized, GLsizei stride) {
		assert(false);
	}
	void mode(GLenum draw_mode) {
		this->draw_mode = draw_mode;
	}

	void draw() {
		assert(bound && "Mesh not bound");
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		GL_CALL(glDrawElements(draw_mode, index_count, GL_UNSIGNED_INT, nullptr));
	}

private:
	GLuint vao;

	GLuint vbo;
	size_t vertex_count;

	size_t attribute_id = 0;
	const void* pointer = nullptr;

	GLuint ibo;
	size_t index_count;

	GLenum draw_mode;

	bool bound = false;
};

template<>
void Mesh::attributes<float>(GLint size, bool normalized, GLsizei stride) {
	assert(bound && "Mesh not bound");

	const auto glnormalized = normalized ? GL_TRUE : GL_FALSE;

	GL_CALL(glEnableVertexAttribArray(attribute_id));
	GL_CALL(glVertexAttribPointer(attribute_id++, size, GL_FLOAT, glnormalized, stride, pointer));

	pointer = (const void*)((size_t)pointer + size * sizeof(GLfloat));
}
