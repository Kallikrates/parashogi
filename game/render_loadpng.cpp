#include "render_loadpng.hpp"

#include <png.h>

#include <cstdio>

static inline bool is_power_of_two(GLint n) {
	return ((n != 0) && !(n & (n - 1)));
}

#define ERRRET( msg... ) {printf(msg); return false;}
bool render::texture::load_png(GLuint & handle, const char *path) {
	FILE * f = fopen(path, "rb");
	if (!f) ERRRET("ERROR (load_png): file not found at \"%s\".\n", path);
	unsigned char header[8];
	fread(header, 1, 8, f);
	if (png_sig_cmp(header, 0, 8)) ERRRET("ERROR (load_png): file at \"%s\" is not a PNG.\n", path);
	png_structp pngp = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop infop = png_create_info_struct(pngp);
	if (setjmp(png_jmpbuf(pngp))) ERRRET("ERROR (load_png): unknown error when reading PNG file at \"%s\".\n", path);
	png_init_io(pngp, f);
	png_set_sig_bytes(pngp, 8);
	png_read_info(pngp, infop);
	GLsizei width = png_get_image_width(pngp, infop);
	GLsizei height = png_get_image_height(pngp, infop);
	png_byte color_type = png_get_color_type(pngp, infop);
	png_byte bit_depth = png_get_bit_depth(pngp, infop);
	if (bit_depth != 8) ERRRET("ERROR (load_png): currently, only 8-bit depth is supported (%s).\n", path);
	if (color_type != PNG_COLOR_TYPE_RGB && color_type != PNG_COLOR_TYPE_RGBA) ERRRET("ERROR (load_png): currently, only RGB(A) images are supported (%s).\n", path);
	png_read_update_info(pngp, infop);
	png_bytep * rowp = new png_bytep [height];
	for (int i = 0; i < height; i++) rowp[i] = new png_byte[png_get_rowbytes(pngp,infop)];
	png_read_image(pngp, rowp);

	glCreateTextures(GL_TEXTURE_2D, 1, &handle);
	glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureStorage2D(handle, 1, color_type == PNG_COLOR_TYPE_RGB ? GL_RGB32F : GL_RGBA32F, width, height);
	for (GLsizei i = 0; i < height; i++) glTextureSubImage2D(handle, 0, 0, i, width, 1, color_type == PNG_COLOR_TYPE_RGB ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, rowp[height-i-1]);

	for (int i = 0; i < height; i++) delete [] rowp[i];
	delete [] rowp;
	png_destroy_info_struct(pngp, &infop);
	png_destroy_read_struct(&pngp, nullptr, nullptr);
	return true;
}

