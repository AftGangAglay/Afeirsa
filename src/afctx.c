/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afctx.h>
#include <afeirsa/afdefs.h>
#include <afeirsa/afgl.h>

#ifdef USE_STDLIB
# include <stdlib.h>
#endif

static enum af_err af_set_gl_hints(enum af_fidelity fidelity);

enum af_err af_mkctx(
		struct af_ctx* ctx, const struct af_gl_ver* gl_ver,
		enum af_fidelity fidelity) {

	enum af_err err;
	const char* extensions;

	AF_PARAM_CHK(ctx);
	AF_PARAM_CHK(gl_ver);

	ctx->gl_ver = *gl_ver;

	af_memset(&ctx->features, 0, sizeof(struct af_features));

	err = af_set_gl_hints(fidelity);
	if(err) return err;

#ifdef USE_STDLIB
	ctx->malloc = malloc;
	ctx->free = free;
	ctx->realloc = realloc;
#endif

	extensions = (const char*) glGetString(GL_EXTENSIONS);

	/*
	 * It's unclear what the empty extension list will be, so NULL or len<2
	 * Seem like sensible assumptions.
	 */
	if(extensions && af_strlen(extensions) > 2) {
		af_size_t n = 0;
		af_size_t i;

		ctx->extensions = 0;
		ctx->extensions_len = 0;
		for(i = 0; extensions[i]; ++i) {
			++n;
			if(extensions[i] == ' ') {
				char* extp;

				ctx->extensions = ctx->realloc(
					ctx->extensions,
					++ctx->extensions_len * sizeof(const char*));
				if(!ctx->extensions) return AF_ERR_MEM;

				extp =
					(ctx->extensions[ctx->extensions_len - 1]
						= ctx->malloc(n));
				if(!extp) return AF_ERR_MEM;

				af_memcpy(extp, &extensions[i] - (n - 1), n - 1);
				extp[n - 1] = 0;

				n = 0;
			}
		}
	}

	return AF_ERR_NONE;
}

enum af_err af_killctx(struct af_ctx* ctx) {
	AF_CTX_CHK(ctx);

	return AF_ERR_NONE;
}

enum af_err af_setview(struct af_ctx* ctx, af_uint_t w, af_uint_t h) {
	AF_CTX_CHK(ctx);

	glViewport(0, 0, (int) w, (int) h);
	AF_GL_CHK;

	return AF_ERR_NONE;
}

static enum af_err af_set_gl_hints(enum af_fidelity fidelity) {
	af_uint_t mode;
	af_size_t i;

	const af_uint_t gl_hints[] = {
#ifdef GL_FOG_HINT
		GL_FOG_HINT,
#endif
#ifdef GL_GENERATE_MIPMAP_HINT
		GL_GENERATE_MIPMAP_HINT,
#endif
#ifdef GL_LINE_SMOOTH_HINT
		GL_LINE_SMOOTH_HINT,
#endif
#ifdef GL_PERSPECTIVE_CORRECTION_HINT
		GL_PERSPECTIVE_CORRECTION_HINT,
#endif
#ifdef GL_POINT_SMOOTH_HINT
		GL_POINT_SMOOTH_HINT,
#endif
#ifdef GL_POLYGON_SMOOTH_HINT
		GL_POLYGON_SMOOTH_HINT,
#endif
#ifdef GL_TEXTURE_COMPRESSION_HINT
		GL_TEXTURE_COMPRESSION_HINT,
#endif
#ifdef GL_FRAGMENT_SHADER_DERIVATIVE_HINT
		GL_FRAGMENT_SHADER_DERIVATIVE_HINT
#endif
	};

	switch(fidelity) {
		case AF_FIDELITY_NONE: {
			mode = GL_DONT_CARE;
			break;
		}
        case AF_FIDELITY_NICE: {
			mode = GL_NICEST;
			break;
		}
		case AF_FIDELITY_FAST: {
			mode = GL_FASTEST;
			break;
		}
	}

	for(i = 0; i < AF_ARRLEN(gl_hints); ++i) {
		glHint(gl_hints[i], mode);
		/*
		 * NOTE: We clear error here because the only real error state is if we
		 * 		 Use an invalid enum - in which case it doesn't matter if it's
		 * 		 Unset. Checking for the availability of these at runtime is
		 * 		 A bit tedious so this works.
		 */
		af_gl_err_clear();
	}

	return AF_ERR_NONE;
}
