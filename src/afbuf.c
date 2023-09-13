/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afbuf.h>
#include <afeirsa/afdefs.h>
#include <afeirsa/afgl.h>

static af_uint_t af_gl_buf_type(enum af_buf_type type) {
	switch(type) {
		case AF_BUF_VERT:  return GL_ARRAY_BUFFER;
		case AF_BUF_IND:   return GL_ELEMENT_ARRAY_BUFFER;
		/*
		 * We use GL_NONE to indicate that no true implementation
		 * is available and to use our own emulation of the feature.
		 */
		default: return GL_NONE;
	}
}

enum af_err af_mkbuf(
		struct af_ctx* ctx, struct af_buf* buf, enum af_buf_type type) {

	af_uint_t gl_type = af_gl_buf_type(type);

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(buf);

	buf->type = type;

	/*
	 * Textures are obviously separate entities in GL but this makes the API
	 * Nice and consistent between buffers and textures, so it's nice.
	 */
	if(type == AF_BUF_TEX) {
#ifdef GL_VERSION_1_1
		if(ctx->features.multitexture) {
			glGenTextures(1, &buf->gl_handle);
			AF_GL_CHK;
		}
#endif
		return AF_ERR_NONE;
	}

	{
		void (*proc)(int, af_uint_t*);
#ifdef GL_VERSION_2_0
		if(ctx->features.buffers == AF_CORE) proc = glGenBuffers;
#endif
#ifdef GL_ARB_vertex_buffer_object
		if(ctx->features.buffers == AF_ARB) proc = glGenBuffersARB;
#endif

		if(ctx->features.buffers && gl_type) {
			proc(1, &buf->gl_handle);
			AF_GL_CHK;
			return AF_ERR_NONE;
		}
	}

	return AF_ERR_NONE;
}

enum af_err af_killbuf(struct af_ctx* ctx, struct af_buf* buf) {
	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(buf);

	if(buf->type == AF_BUF_TEX) {
		if(ctx->features.multitexture) {
#ifdef GL_VERSION_1_1
			glDeleteTextures(1, &buf->gl_handle);
			AF_GL_CHK;
			return AF_ERR_NONE;
#endif
		}
		return AF_ERR_NONE;
	}

	{
		void (*proc)(int, const af_uint_t*);
#ifdef GL_VERSION_2_0
		if(ctx->features.buffers == AF_CORE) proc = glDeleteBuffers;
#endif
#ifdef GL_ARB_vertex_buffer_object
		if(ctx->features.buffers == AF_ARB) proc = glDeleteBuffersARB;
#endif

		if(ctx->features.buffers) {
			proc(1, &buf->gl_handle);
			AF_GL_CHK;
			return AF_ERR_NONE;
		}
	}

	return AF_ERR_NONE;
}

enum af_err af_upload(
		struct af_ctx* ctx, struct af_buf* buf,
		const void* data, af_size_t size) {

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(buf);

	buf->size = size;

	if(buf->type == AF_BUF_TEX) {
		if(ctx->features.multitexture) {
#ifdef GL_VERSION_1_1
			glBindTexture(GL_TEXTURE_2D, buf->gl_handle);
			AF_GL_CHK;

			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA,
				(int) buf->tex_width, (int) (buf->size / (4 * buf->tex_width)),
				0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			AF_GL_CHK;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			AF_GL_CHK;

			return AF_ERR_NONE;
#endif
		}
	}

	{
		void (*bind_proc)(af_uint_t, af_uint_t);
		void (*data_proc)(af_uint32_t, long, const void*, af_uint32_t);
#ifdef GL_VERSION_2_0
		if(ctx->features.buffers == AF_CORE) {
			bind_proc = glBindBuffer;
			data_proc = glBufferData;
		}
#endif
#ifdef GL_ARB_vertex_buffer_object
		if(ctx->features.buffers == AF_ARB) {
			bind_proc = glBindBufferARB;
			data_proc = glBufferDataARB;
		}
#endif

		if(ctx->features.buffers) {
			af_uint_t type = af_gl_buf_type(buf->type);
			bind_proc(type, buf->gl_handle);
			AF_GL_CHK;
			data_proc(type, (long) size, data, GL_STATIC_DRAW);
			AF_GL_CHK;
			return AF_ERR_NONE;
		}
	}

	buf->storage = data;

	return AF_ERR_NONE;
}
