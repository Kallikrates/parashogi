#ifndef RENDER_SHADERS_HPP
#define RENDER_SHADERS_HPP

#include "render_common.hpp"

namespace render {
namespace shader {
	bool init();
	void use_board_program();

	void uniform_mat4(char const *, glm::mat4);
	void uniform_transform(char const *, glm::vec3 translate, glm::vec3 rotation, glm::vec3 scale);
}}

#endif //RENDER_SHADERS_HPP
