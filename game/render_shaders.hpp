#ifndef RENDER_SHADERS_HPP
#define RENDER_SHADERS_HPP

#include "render_common.hpp"

namespace render {
namespace shader {

	// GENERIC

	class general { friend class program;
	public:
		general(GLuint type);
		virtual ~general();
		bool compile(char const * src);
	protected:
		GLuint handle;
	};

	class vertex : public general {
	public:
		vertex();
	};

	class fragment : public general {
	public:
		fragment();
	};

	class program {
	public:
		program();
		virtual ~program();
		void attach(general const & shader);
		void detach(general const & shader);
		bool link();
		void use();
		virtual void prepare_uniforms() {}
	protected:
		GLuint handle;
	};

	//IMPLEMENTS

	class program_board : public program {
	public:
		program_board() : program() {}
		virtual void prepare_uniforms();
		void uniform_transform(glm::vec3 translate, glm::vec3 rotation, glm::vec3 scale);
	protected:
		GLint transform_location;
		GLint color_location;
	};

	class program_piece : public program {
	public:
		program_piece() : program() {}
		virtual void prepare_uniforms();
		void uniform_transform(glm::vec3 translate, glm::vec3 rotation, glm::vec3 scale);
		void uniform_color(glm::vec3 rgb);
	protected:
		GLint transform_location;
		GLint color_location;
	};

	class program_postproc_0 : public program {
	public:
		program_postproc_0() : program() {}
	};

	class program_postproc_glow : public program {
	public:
		program_postproc_glow() : program() {}
	};

	//

	extern program_board * board;
	extern program_piece * piece;
	extern program_postproc_0 * postproc_0;
	extern program_postproc_glow * postproc_glow;

	bool init();
}}

#endif //RENDER_SHADERS_HPP
