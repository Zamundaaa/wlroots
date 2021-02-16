#include <drm_fourcc.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "render/gles2.h"
#include "render/shm_format.h"

/*
 * The DRM formats are little endian while the GL formats are big endian,
 * so DRM_FORMAT_ARGB8888 is actually compatible with GL_BGRA_EXT.
 */
static const struct wlr_gles2_pixel_format formats[] = {
	{
		.drm_format = DRM_FORMAT_ARGB8888,
		.depth = 32,
		.bpp = 32,
		.gl_format = GL_BGRA_EXT,
		.gl_type = GL_UNSIGNED_BYTE,
		.has_alpha = true,
	},
	{
		.drm_format = DRM_FORMAT_XRGB8888,
		.depth = 24,
		.bpp = 32,
		.gl_format = GL_BGRA_EXT,
		.gl_type = GL_UNSIGNED_BYTE,
		.has_alpha = false,
	},
	{
		.drm_format = DRM_FORMAT_XBGR8888,
		.depth = 24,
		.bpp = 32,
		.gl_format = GL_RGBA,
		.gl_type = GL_UNSIGNED_BYTE,
		.has_alpha = false,
	},
	{
		.drm_format = DRM_FORMAT_ABGR8888,
		.depth = 32,
		.bpp = 32,
		.gl_format = GL_RGBA,
		.gl_type = GL_UNSIGNED_BYTE,
		.has_alpha = true,
	},
};

// TODO: more pixel formats

const struct wlr_gles2_pixel_format *get_gles2_format_from_wl(
		enum wl_shm_format fmt) {
	for (size_t i = 0; i < sizeof(formats) / sizeof(*formats); ++i) {
		if (convert_drm_format_to_wl_shm(formats[i].drm_format) == fmt) {
			return &formats[i];
		}
	}
	return NULL;
}

const struct wlr_gles2_pixel_format *get_gles2_format_from_gl(
		GLint gl_format, GLint gl_type, bool alpha) {
	for (size_t i = 0; i < sizeof(formats) / sizeof(*formats); ++i) {
		if (formats[i].gl_format == gl_format &&
				formats[i].gl_type == gl_type &&
				formats[i].has_alpha == alpha) {
			return &formats[i];
		}
	}
	return NULL;
}

const enum wl_shm_format *get_gles2_wl_formats(size_t *len) {
	static enum wl_shm_format wl_formats[sizeof(formats) / sizeof(formats[0])];
	*len = sizeof(formats) / sizeof(formats[0]);
	for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); i++) {
		wl_formats[i] = convert_drm_format_to_wl_shm(formats[i].drm_format);
	}
	return wl_formats;
}
