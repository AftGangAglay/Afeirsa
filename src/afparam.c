/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afparam.h>
#include <afeirsa/afdefs.h>
#include <afeirsa/afgl.h>

enum af_err af_setparam(
		struct af_ctx* ctx, const char* name, struct af_param* param,
		const void* data) {

	af_uint_t target;

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(name);
	AF_PARAM_CHK(param);
	AF_PARAM_CHK(data);

	if(ctx->features.shaders) {
#ifdef GL_VERSION_2_0
		/* TODO: Params as uniforms. */
#else
# ifdef GL_ARB_shader_objects
# endif
#endif
		return AF_ERR_NONE;
	}

	target = GL_PROJECTION;
	switch(param->type) {
		case AF_PARAM_NONE: break;
		case AF_PARAM_MODELVIEW: target = GL_MODELVIEW;
			AF_FALLTHROUGH;
			/* FALLTHRU */
		case AF_PARAM_PROJECTION: {
			AF_VERIFY(param->datatype == AF_FLOAT, AF_ERR_BAD_PARAM);
			AF_VERIFY(param->count == 16, AF_ERR_BAD_PARAM);

			glMatrixMode(target);
			AF_GL_CHK;
			glLoadMatrixf(data);
			AF_GL_CHK;
			break;
		}
	}

	return AF_ERR_NONE;
}
