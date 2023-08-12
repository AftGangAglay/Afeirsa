/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afbuf.h>
#include <afeirsa/afdefs.h>
#include <afeirsa/afgl.h>

static af_uint_t af_gl_buf_type(enum af_buf_type type) {
	switch(type) {
		case AF_BUF_VERTEX:  return GL_ARRAY_BUFFER;
		case AF_BUF_INDEX:   return GL_ELEMENT_ARRAY_BUFFER;
#ifdef GL_TEXTURE_BUFFER
		case AF_BUF_TEXTURE: return GL_TEXTURE_BUFFER;
#endif
#ifdef GL_UNIFORM_BUFFER
		case AF_BUF_UNIFORM: return GL_UNIFORM_BUFFER;
#endif
		/*
		 * We use GL_NONE to indicate that no true implementation
		 * is available and to use our own emulation of the feature.
		 */
		default: return GL_NONE;
	}
}

enum af_err af_mkbuf(struct af_ctx* ctx, struct af_buf* buf, enum af_buf_type type) {
    af_uint_t gl_type = af_gl_buf_type(type);

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(buf);

    buf->type = type;

    if(ctx->features.buffers && gl_type) {
#ifdef GL_VERSION_2_0
        buf->native = AF_TRUE;

        glGenBuffers(1, &buf->gl_handle);
		AF_GL_CHK;

        /* We don't mind leaving stale state in bind points. */
		glBindBuffer(gl_type, buf->gl_handle);
		AF_GL_CHK;

        return AF_ERR_NONE;
#endif
	}

    buf->native = AF_FALSE;

    return AF_ERR_NONE;
}

enum af_err af_killbuf(struct af_ctx* ctx, struct af_buf* buf) {
	AF_CTX_CHK(ctx);

	if(ctx->features.buffers) {
        /* TODO: Native buffering cleanup */
	}
	else {
		ctx->free(buf->storage);
	}

	return AF_ERR_NONE;
}
