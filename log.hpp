#pragma once

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdarg.h>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GL_LOG_FILE "gl.log"

#define GL_CALL(x)\
	gl_clear_error();\
	x;\
	assert(gl_check_error(#x, __FILE__, __LINE__))

void gl_clear_error() {
	while (glGetError() != GL_NO_ERROR);
}

bool gl_check_error(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << ") " << function << " " << file << ":" << line << std::endl;
		return false;
	}

	return true;
}

bool restart_gl_log() {
	std::fstream file;

	file.open(GL_LOG_FILE, std::ios::out);

	if (!file) {
		std::cerr << "ERROR: could not open GL_LOG_FILE log file " << GL_LOG_FILE << " for writing" << std::endl;
		return false;
	}

	time_t now = time(NULL);
	char* date = ctime(&now);

	std::cout << "GL_LOG_FILE log. local time " << date << std::endl;

	file.close();

	return true;
}

bool gl_log(const char* message, ...) {
	va_list argptr;

	// FIXME: use fstrem instead of fopen
	FILE* file = fopen(GL_LOG_FILE, "a");

	if (!file) {
		std::cerr << "ERROR: could not open GL_LOG_FILE " << GL_LOG_FILE << " file for appending" << std::endl;
		return false;
	}

	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);

	fclose(file);

	return true;
}

bool gl_log_error(const char* message, ...) {
	va_list argptr;

	// FIXME: use fstrem instead of fopen
	FILE* file = fopen(GL_LOG_FILE, "a");

	if (!file) {
		std::cerr << "ERROR: could not open GL_LOG_FILE " << GL_LOG_FILE << " file for appending" << std::endl;
		return false;
	}

	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);

	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);

	fclose(file);

	return true;
}

void glfw_error_callback(int error, const char* description) {
	gl_log_error("GLFW ERROR: code %i msg: %s\n", error, description);
}

void log_gl_params() {
	GLenum params[] = {
	    GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
	    GL_MAX_CUBE_MAP_TEXTURE_SIZE,
	    GL_MAX_DRAW_BUFFERS,
	    GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
	    GL_MAX_TEXTURE_IMAGE_UNITS,
	    GL_MAX_TEXTURE_SIZE,
	    GL_MAX_VARYING_FLOATS,
	    GL_MAX_VERTEX_ATTRIBS,
	    GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
	    GL_MAX_VERTEX_UNIFORM_COMPONENTS,
	    GL_MAX_VIEWPORT_DIMS,
	    GL_STEREO,
	};
	const char* names[] = {
	    "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
	    "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
	    "GL_MAX_DRAW_BUFFERS",
	    "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
	    "GL_MAX_TEXTURE_IMAGE_UNITS",
	    "GL_MAX_TEXTURE_SIZE",
	    "GL_MAX_VARYING_FLOATS",
	    "GL_MAX_VERTEX_ATTRIBS",
	    "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
	    "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
	    "GL_MAX_VIEWPORT_DIMS",
	    "GL_STEREO",
	};

	gl_log("GL Context Params:\n");

	char msg[256];
	// integers - only works if the order is 0-10 integer return types
	for (int i = 0; i < 10; i++) {
		int v = 0;
		glGetIntegerv(params[i], &v);
		gl_log("%s %i\n", names[i], v);
	}
	// others

	int v[2] = {0, 0};
	glGetIntegerv(params[10], v);
	gl_log("%s %i %i\n", names[10], v[0], v[1]);

	unsigned char s = 0;
	glGetBooleanv(params[11], &s);
	gl_log("%s %u\n", names[11], (unsigned int)s);
	gl_log("-----------------------------\n");
}

void log_shader_info(GLuint shader_index, const char* shader_name) {
	int max_length = 2048;
	int actual_length = 0;
	char shader_log[2048];
	glGetShaderInfoLog(shader_index, max_length, &actual_length, shader_log);
	gl_log("shader info log for GL index %u (%s)\n%s\n", shader_index, shader_name, shader_log);
}

void log_program_info(GLuint program, const char* program_name) {
	int max_length = 2048;
	int actual_length = 0;
	char program_log[2048];
	glGetProgramInfoLog(program, max_length, &actual_length, program_log);
	gl_log("program info log for GL index %u (%s)\n%s\n", program, program_name, program_log);
}

const char* GL_type_to_string(GLenum type) {
	switch (type) {
		case GL_BOOL: return "bool";
		case GL_INT: return "int";
		case GL_FLOAT: return "float";
		case GL_FLOAT_VEC2: return "vec2";
		case GL_FLOAT_VEC3: return "vec3";
		case GL_FLOAT_VEC4: return "vec4";
		case GL_FLOAT_MAT2: return "mat2";
		case GL_FLOAT_MAT3: return "mat3";
		case GL_FLOAT_MAT4: return "mat4";
		case GL_SAMPLER_2D: return "sampler2D";
		case GL_SAMPLER_3D: return "sampler3D";
		case GL_SAMPLER_CUBE: return "samplerCube";
		case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
		default: break;
	}
	return "other";
}

void print_all(GLuint program) {
	int params = -1;

	gl_log("---------------\nshader program %i info:\n", program);

	glGetProgramiv(program, GL_LINK_STATUS, &params);
	gl_log("GL_LINK_STATUS = %i\n", params);

	glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
	gl_log("GL_ATTACHED_SHADERS = %i\n", params);

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);
	gl_log("GL_ACTIVE_ATTRIBUTES = %i\n", params);

	for (int i = 0; i < params; i++) {
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveAttrib(program, i, max_length, &actual_length, &size, &type, name);

		if (size > 1) {
			for (int j = 0; j < size; j++) {
				char long_name[64];
				sprintf(long_name, "%s[%i]", name, j);
				int location = glGetAttribLocation(program, long_name);
				gl_log("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), long_name, location);
			}

		} else {
			int location = glGetAttribLocation(program, name);
			gl_log("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), name, location);
		}
	}

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params);
	gl_log("GL_ACTIVE_UNIFORMS = %i\n", params);

	for (int i = 0; i < params; i++) {
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveUniform(program, i, max_length, &actual_length, &size, &type, name);

		if (size > 1) {
			for (int j = 0; j < size; j++) {
				char long_name[64];
				sprintf(long_name, "%s[%i]", name, j);
				int location = glGetUniformLocation(program, long_name);
				gl_log("  %i) type:%s name %s location:%i\n", i, GL_type_to_string(type), long_name, location);
			}

		} else {
			int location = glGetUniformLocation(program, name);
			gl_log("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), name, location);
		}
	}

	log_program_info(program, "shader program");
}

bool is_valid(GLuint program) {
	int params = -1;

	glValidateProgram(program);

	glGetProgramiv(program, GL_VALIDATE_STATUS, &params);

	std::cout << "program " << program << " GL_VALIDATE_STATUS = " << params << std::endl;

	if (GL_TRUE != params) {
		log_program_info(program, "INVALID shader program");
		return false;
	}

	return true;
}