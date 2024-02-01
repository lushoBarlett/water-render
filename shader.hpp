#pragma once

#include <assert.h>

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "log.hpp"

struct Shader {
	const char* vertex_shader;
	const char* fragment_shader;

	Shader(const char* vertex_shader_file, const char* fragment_shader_file);
	~Shader();

	void reload();
	void bind();
	void unbind();
	GLuint id();

	struct Location1F uniform1f(const char* name);
	struct Location1I uniform1i(const char* name);

private:
	GLuint program;
	void compile_shaders();
	char* read_file(const char* filename);
};

#define LOCATION_CLASS(NAME, TYPE, GL_UNIFORM_CALL) \
struct Location##NAME { \
	const char* name; \
	\
	Location##NAME(Shader* shader, const char* name) : shader(shader), name(name) { \
		location = glGetUniformLocation(shader->id(), name); \
	} \
	\
	TYPE get() { \
		return current; \
	} \
	\
	void set(TYPE value) { \
		current = value; \
		GL_UNIFORM_CALL(shader->id(), location, value); \
	} \
	\
	GLint id() { \
		return location; \
	} \
	\
private: \
	Shader* shader; \
	GLint location; \
	TYPE current; \
}

LOCATION_CLASS(1F, float, glProgramUniform1f);
LOCATION_CLASS(1I, int, glProgramUniform1i);

Shader::Shader(const char* vertex_shader_file, const char* fragment_shader_file) : vertex_shader(vertex_shader_file), fragment_shader(fragment_shader_file) {
	compile_shaders();
}

Shader::~Shader() {
	GL_CALL(glDeleteProgram(program));
}

void Shader::reload() {
	GL_CALL(glDeleteProgram(program));
	compile_shaders();
}

void Shader::bind() {
	GL_CALL(glUseProgram(program));
}

void Shader::unbind() {
	GL_CALL(glUseProgram(0));
}

GLuint Shader::id() {
	return program;
}

Location1F Shader::uniform1f(const char* name) {
	return Location1F(this, name);
}

Location1I Shader::uniform1i(const char* name) {
	return Location1I(this, name);
}

void Shader::compile_shaders() {
	char* vs_string = read_file(vertex_shader);
	char* fs_string = read_file(fragment_shader);

	GL_CALL(GLuint vs = glCreateShader(GL_VERTEX_SHADER));
	GL_CALL(glShaderSource(vs, 1, &vs_string, NULL));
	GL_CALL(glCompileShader(vs));

	log_shader_info(vs, "vertex shader");

	GL_CALL(GLuint fs = glCreateShader(GL_FRAGMENT_SHADER));
	GL_CALL(glShaderSource(fs, 1, &fs_string, NULL));
	GL_CALL(glCompileShader(fs));

	log_shader_info(fs, "fragment shader");

	GL_CALL(program = glCreateProgram());
	GL_CALL(glAttachShader(program, fs));
	GL_CALL(glAttachShader(program, vs));
	GL_CALL(glLinkProgram(program));

	log_program_info(program, "shader program");

	int params = -1;
	GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &params));

	if (GL_TRUE != params) {
		print_all(program);
		gl_log_error("ERROR: could not link shader program GL index %u\n", program);
	}

	delete vs_string;
	delete fs_string;
}

char* Shader::read_file(const char* filename) {
	FILE* file = fopen(filename, "rb");

	if (!file) {
		gl_log_error("ERROR: opening file for reading: %s\n", filename);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = new char[length + 1];

	if (!buffer) {
		gl_log_error("ERROR: allocating %ld bytes for file contents: %s\n", length + 1, filename);
		return NULL;
	}

	fread(buffer, 1, length, file);
	fclose(file);

	buffer[length] = '\0';

	return buffer;
}
