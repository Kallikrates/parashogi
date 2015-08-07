#ifndef RENDER_FRAMEBUFFERS_HPP
#define RENDER_FRAMEBUFFERS_HPP

#include "render_common.hpp"

namespace render {
namespace framebuffer {

	class framebuffer_cc {
	public:
		framebuffer_cc(GLsizei w, GLsizei h);
		virtual ~framebuffer_cc();
		void bind();
		void bind_as_textures();
		void update_size(GLsizei w, GLsizei h);
	protected:
		GLuint handle;
		GLuint colatch0, colatch1;
		static GLenum const bufs [];
	};

	extern framebuffer_cc * cc;

	bool init(GLsizei w, GLsizei h);
	void use_mainfb();
	void update_framebuffer_sizes(GLsizei w, GLsizei h);
	void clear();
}}

#endif // RENDER_FRAMEBUFFERS_HPP
