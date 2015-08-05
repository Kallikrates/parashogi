#include "render_common.hpp"
#include "render_shaders.hpp"

#include <cstdio>
#include <vector>

static GLuint current_program = 0;
static GLuint board_shader_program = 0;

#define ERRRET( msg... ) {printf(msg); return false;}
bool render::shader::init() {
	GLuint board_vertex_shader, board_fragment_shader;
	GLint success;

	board_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GLchar const * board_vshader_source =
			"#version 450 core \n"
			"layout(location = 0) in vec3 pos;"
			"layout(location = 1) in vec2 uv_in;"
			"uniform mat4 trans;"
			"out vec2 uv;"
			"void main() {"
			"uv = uv_in;"
			"gl_Position = trans * vec4(pos, 1);"
			"}";
	glShaderSource(board_vertex_shader, 1, &board_vshader_source, 0);
	glCompileShader(board_vertex_shader);
	glGetShaderiv(board_vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLint maxLength = 0;
		glGetShaderiv(board_vertex_shader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(board_vertex_shader, maxLength, &maxLength, &infoLog[0]);
		ERRRET("ERROR (render::shader::init): board vertex shader failed to compile:\n%s\n", infoLog.data())
	}

	board_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar const * board_fshader_source =
			"#version 450 core \n"
			"layout(binding = 0) uniform sampler2D board;"
			"in vec2 uv;"
			"out vec3 color;"
			"void main() {"
			"color = texture(board, uv).rgb;"
			"}";
	glShaderSource(board_fragment_shader, 1, &board_fshader_source, 0);
	glCompileShader(board_fragment_shader);
	glGetShaderiv(board_fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLint maxLength = 0;
		glGetShaderiv(board_fragment_shader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(board_fragment_shader, maxLength, &maxLength, &infoLog[0]);
		ERRRET("ERROR (render::shader::init): board fragment shader failed to compile:\n%s\n", infoLog.data());
	}

	board_shader_program = glCreateProgram();
	glAttachShader(board_shader_program, board_vertex_shader);
	glAttachShader(board_shader_program, board_fragment_shader);
	glLinkProgram(board_shader_program);
	glGetProgramiv(board_shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		GLint maxLength = 0;
		glGetProgramiv(board_shader_program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(board_shader_program, maxLength, &maxLength, &infoLog[0]);
		ERRRET("ERROR (render::shader::init): board shader program failed to link:\n%s\n", infoLog.data());
	}

	return true;
}

void render::shader::use_board_program() {
	current_program = board_shader_program;
	glUseProgram(board_shader_program);
}

void render::shader::uniform_mat4(char const * uniform, glm::mat4 matrix) {
	glUniformMatrix4fv(glGetUniformLocation(current_program, uniform), 1, GL_FALSE, &matrix[0][0]);
}

void render::shader::uniform_transform(char const * uniform, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) {
	glm::mat4 translation = glm::translate(glm::mat4{1.0f}, translate);
	glm::mat4 rotation = (glm::mat4)(glm::quat)rotate;
	glm::mat4 scaled = glm::scale(glm::mat4{1.0f}, scale);
	uniform_mat4(uniform, scaled * rotation * translation);
}
