#include "render_framebuffers.hpp"

GLenum const render::framebuffer::framebuffer_cc::bufs [] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

render::framebuffer::framebuffer_cc::framebuffer_cc(GLsizei width, GLsizei height) {
	glCreateFramebuffers(1, &handle);

	glCreateTextures(GL_TEXTURE_2D, 1, &colatch0);
	glTextureStorage2D(colatch0, 1, GL_RGBA32F, width, height);
	glTextureParameteri(colatch0, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(colatch0, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glNamedFramebufferTexture(handle, GL_COLOR_ATTACHMENT0 ,colatch0, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &colatch1);
	glTextureStorage2D(colatch1, 1, GL_RGBA32F, width, height);
	glTextureParameteri(colatch1, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(colatch1, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glNamedFramebufferTexture(handle, GL_COLOR_ATTACHMENT1 ,colatch1, 0);

	glNamedFramebufferDrawBuffers(handle, 2, bufs);
}

render::framebuffer::framebuffer_cc::~framebuffer_cc() {
	glDeleteTextures(1, &colatch0);
	glDeleteTextures(1, &colatch1);
	glDeleteFramebuffers(1, &handle);
}

void render::framebuffer::framebuffer_cc::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void render::framebuffer::framebuffer_cc::bind_as_textures() {
	glBindTextureUnit(0, colatch0);
	glBindTextureUnit(1, colatch1);
}

void render::framebuffer::framebuffer_cc::update_size(GLsizei w, GLsizei h) {
	glDeleteTextures(1, &colatch0);
	glDeleteTextures(1, &colatch1);

	glCreateTextures(GL_TEXTURE_2D, 1, &colatch0);
	glTextureStorage2D(colatch0, 1, GL_RGBA32F, w, h);
	glTextureParameteri(colatch0, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(colatch0, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glNamedFramebufferTexture(handle, GL_COLOR_ATTACHMENT0 ,colatch0, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &colatch1);
	glTextureStorage2D(colatch1, 1, GL_RGBA32F, w, h);
	glTextureParameteri(colatch1, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(colatch1, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glNamedFramebufferTexture(handle, GL_COLOR_ATTACHMENT1 ,colatch1, 0);
}

render::framebuffer::framebuffer_cc * render::framebuffer::cc;

bool render::framebuffer::init(GLsizei w, GLsizei h) {
	cc = new framebuffer_cc(w, h);
	return true;
}

void render::framebuffer::use_mainfb() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void render::framebuffer::update_framebuffer_sizes(GLsizei w, GLsizei h) {
	cc->update_size(w, h);
}

void render::framebuffer::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
