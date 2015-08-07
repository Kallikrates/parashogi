#include "render_common.hpp"
#include "render_shaders.hpp"

#include <cstdio>
#include <vector>

static GLuint current_program = 0;

render::shader::general::general(GLuint type) : handle(glCreateShader(type)) { }
render::shader::general::~general() {glDeleteShader(handle);}

bool render::shader::general::compile(char const * src) {
	GLint success;
	glShaderSource(handle, 1, &src, 0);
	glCompileShader(handle);
	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLint maxLength = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(handle, maxLength, &maxLength, &infoLog[0]);
		printf("ERROR (render::shader::general): shader failed to compile:\n%s\n", infoLog.data());
	}
	return success;
}

render::shader::vertex::vertex() : general(GL_VERTEX_SHADER) { }

render::shader::fragment::fragment() : general(GL_FRAGMENT_SHADER) { }

render::shader::program::program() : handle(glCreateProgram()) { }
render::shader::program::~program() { glDeleteProgram(handle); }

void render::shader::program::attach(general const & shader) {
	glAttachShader(handle, shader.handle);
}

void render::shader::program::detach(general const & shader) {
	glDetachShader(handle, shader.handle);
}

bool render::shader::program::link() {
	GLint success;
	glLinkProgram(handle);
	glGetProgramiv(handle, GL_LINK_STATUS, &success);
	if (!success) {
		GLint maxLength = 0;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(handle, maxLength, &maxLength, &infoLog[0]);
		printf("shader program failed to link:\n%s\n", infoLog.data());
	}
	return success;
}

void render::shader::program::use() {
	current_program = this->handle;
	glUseProgram(handle);
}

render::shader::program_board * render::shader::board = nullptr;
render::shader::program_piece * render::shader::piece = nullptr;
render::shader::program_postproc_0 * render::shader::postproc_0 = nullptr;
render::shader::program_postproc_glow * render::shader::postproc_glow = nullptr;

void render::shader::program_board::prepare_uniforms() {
	transform_location = glGetUniformLocation(this->handle, "trans");
}

void render::shader::program_piece::prepare_uniforms() {
	transform_location = glGetUniformLocation(this->handle, "trans");
	color_location = glGetUniformLocation(this->handle, "gcolor");
}

void render::shader::program_board::uniform_transform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) {
	glm::mat4 translation = glm::translate(glm::mat4{1.0f}, translate);
	glm::mat4 rotation = (glm::mat4)(glm::quat)rotate;
	glm::mat4 scaled = glm::scale(glm::mat4{1.0f}, scale);
	glm::mat4 transformation = translation * rotation * scaled;
	glUniformMatrix4fv(this->transform_location, 1, GL_FALSE, &transformation[0][0]);
}

void render::shader::program_piece::uniform_transform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) {
	glm::mat4 translation = glm::translate(glm::mat4{1.0f}, translate);
	glm::mat4 rotation = (glm::mat4)(glm::quat)rotate;
	glm::mat4 scaled = glm::scale(glm::mat4{1.0f}, scale);
	glm::mat4 transformation = translation * rotation * scaled;
	glUniformMatrix4fv(this->transform_location, 1, GL_FALSE, &transformation[0][0]);
}

void render::shader::program_piece::uniform_color(glm::vec3 rgb) {
	glUniform3fv(this->color_location, 1, &rgb[0]);
}


#define ERRRET( msg... ) {printf(msg); return false;}
bool render::shader::init() {

	render::shader::vertex board_vertex;
	if (!board_vertex.compile(
			"#version 450 core \n"
			"layout(location = 0) in vec3 pos;"
			"layout(location = 1) in vec2 uv_in;"
			"uniform mat4 trans;"
			"out vec2 uv;"
			"void main() {"
			"uv = uv_in;"
			"gl_Position = trans * vec4(pos, 1);"
			"}"
			))
		ERRRET("ERROR (render::shader::init): board vertex shader failed to compile!");
	render::shader::fragment board_fragment;
	if (!board_fragment.compile(
			"#version 450 core \n"
			"layout(binding = 0) uniform sampler2D tex0;"
			"in vec2 uv;"
			"uniform vec3 gcolor;"
			"layout(location = 0) out vec4 color;"
			"layout(location = 1) out vec4 gcolor_fb;"
			"void main() {"
			"vec4 color4 = texture(tex0, uv);"
			"color = color4.rgba;"
			"gcolor_fb = vec4(0, 0, 0, 1);"
			"}"
			))
			ERRRET("ERROR (render::shader::init): board fragment shader failed to compile!");
	board = new render::shader::program_board;
	board->attach(board_vertex);
	board->attach(board_fragment);
	if (!board->link()) ERRRET("ERROR (render::shader::init): board program failed to link!");
	board->prepare_uniforms();

	render::shader::vertex piece_vertex;
	if (!piece_vertex.compile(
			"#version 450 core \n"
			"layout(location = 0) in vec3 pos;"
			"layout(location = 1) in vec2 uv_in;"
			"uniform mat4 trans;"
			"out vec2 uv;"
			"void main() {"
			"uv = uv_in;"
			"gl_Position = trans * vec4(pos, 1);"
			"}"
			))
		ERRRET("ERROR (render::shader::init): piece vertex shader failed to compile!");
	render::shader::fragment piece_fragment;
	if (!piece_fragment.compile(
			"#version 450 core \n"
			"layout(binding = 0) uniform sampler2D tex0;"
			"layout(binding = 1) uniform sampler2D mask;"
			"in vec2 uv;"
			"uniform vec3 gcolor;"
			"layout(location = 0) out vec4 color;"
			"layout(location = 1) out vec4 gcolor_fb;"
			"void main() {"
			"vec4 color4 = texture(tex0, uv);"
			"color = color4.rgba;"
			"if (texture(mask, uv).r > 0.0f) {"
			"gcolor_fb = vec4(texture(mask, uv).r * gcolor, 1);"
			"}"
			"}"
			))
			ERRRET("ERROR (render::shader::init): piece fragment shader failed to compile!");
	piece = new render::shader::program_piece;
	piece->attach(piece_vertex);
	piece->attach(piece_fragment);
	if (!piece->link()) ERRRET("ERROR (render::shader::init): piece program failed to link!");
	piece->prepare_uniforms();

	render::shader::vertex postproc_0_vertex;
	if (!postproc_0_vertex.compile(
			"#version 450 core \n"
			"layout(location = 0) in vec3 pos;"
			"layout(location = 1) in vec2 uv_in;"
			"out vec2 uv;"
			"void main() {"
			"uv = uv_in;"
			"gl_Position.xyz = pos;"
			"}"
			))
		ERRRET("ERROR (render::shader::init): postproc_0 vertex shader failed to compile!");
	render::shader::fragment postproc_0_fragment;
	if (!postproc_0_fragment.compile(
			"#version 450 core \n"
			"layout(binding = 0) uniform sampler2D famebuffer_color;"
			"layout(binding = 1) uniform sampler2D famebuffer_gcolor;"
			"in vec2 uv;"
			"out vec3 color;"
			"void main() {"
			"color = max(texture(famebuffer_gcolor, uv).rgb, texture(famebuffer_color, uv).rgb);"
			"}"
			))
		ERRRET("ERROR (render::shader::init): postproc_0 fragment shader failed to compile!");
	postproc_0 = new render::shader::program_postproc_0;
	postproc_0->attach(postproc_0_vertex);
	postproc_0->attach(postproc_0_fragment);
	if (!postproc_0->link()) ERRRET("ERROR (render::shader::init): postproc_0 program failed to link!");
	postproc_0->prepare_uniforms();

	render::shader::vertex postproc_glow_vertex;
	if (!postproc_glow_vertex.compile(
			"#version 450 core \n"
			"layout(location = 0) in vec3 pos;"
			"layout(location = 1) in vec2 uv_in;"
			"out vec2 uv;"
			"void main() {"
			"uv = uv_in;"
			"gl_Position.xyz = pos;"
			"}"
			))
		ERRRET("ERROR (render::shader::init): postproc_glow vertex shader failed to compile!");
	render::shader::fragment postproc_glow_fragment;
	if (!postproc_glow_fragment.compile(
			"#version 450 core \n"
			"layout(binding = 0) uniform sampler2D famebuffer_color;"
			"layout(binding = 1) uniform sampler2D famebuffer_gcolor;"
			"in vec2 uv;"
			"out vec3 color;"
			"void main() {"
			"vec4 avg_col;"
			"for (int i = -8; i < 9; i++) { for (int j = -8; j < 9; j++) {"
			"avg_col += texture(famebuffer_gcolor, vec2(uv.x + (i * 0.003f), uv.y + (j * 0.003f)));"
			"}}"
			"color = (avg_col / 64).rgb;"
			"}"
			))
		ERRRET("ERROR (render::shader::init): postproc_glow fragment shader failed to compile!");
	postproc_glow = new render::shader::program_postproc_glow;
	postproc_glow->attach(postproc_glow_vertex);
	postproc_glow->attach(postproc_glow_fragment);
	if (!postproc_glow->link()) ERRRET("ERROR (render::shader::init): postproc_glow program failed to link!");
	postproc_glow->prepare_uniforms();

	return true;
}
