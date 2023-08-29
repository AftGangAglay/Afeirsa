/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afctx.h>
#include <afeirsa/afdefs.h>
#include <afeirsa/afgl.h>

#ifdef USE_STDLIB
# include <stdlib.h>
#endif

static enum af_err af_set_gl_hints(enum af_fidelity fidelity);
static enum af_err af_populate_features(struct af_ctx* ctx);

enum af_err af_mkctx(struct af_ctx* ctx, enum af_fidelity fidelity) {
	const char* extensions;

	AF_PARAM_CHK(ctx);

	af_memset(&ctx->features, 0, sizeof(struct af_features));

	af_memset(&ctx->drawlists, 0, sizeof(struct af_handleset));

#ifdef USE_STDLIB
	ctx->malloc = malloc;
	ctx->free = free;
	ctx->realloc = realloc;
#else
	/*
	 * NOTE: This check is not robust as we assume the ctx struct is
	 * 		 Uninitialized when given to us by the user.
	 */
	AF_VERIFY(
		ctx->malloc &&
		ctx->free &&
		ctx->realloc,
		AF_ERR_BAD_CTX
	);
#endif

	{
		const char* ver = (const char*) glGetString(GL_VERSION);
		if(!ver) {
			/* GL context appears to be broken. */
			return AF_ERR_NO_GL;
		}
		ctx->gl_ver[0] = ver[0] - '0';
		ctx->gl_ver[1] = ver[2] - '0';
	}

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
				char* ptr;

				ctx->extensions = ctx->realloc(
					ctx->extensions,
					++ctx->extensions_len * sizeof(const char*));
				if(!ctx->extensions) return AF_ERR_MEM;

				ptr =
					(ctx->extensions[ctx->extensions_len - 1]
						= ctx->malloc(n));
				if(!ptr) return AF_ERR_MEM;

				af_memcpy(ptr, &extensions[i] - (n - 1), n - 1);
				ptr[n - 1] = 0;

				n = 0;
			}
		}
	}

	AF_CHK(af_set_gl_hints(fidelity));

	glEnable(GL_TEXTURE_2D);
	AF_GL_CHK;

	glEnable(GL_DEPTH_TEST);
	AF_GL_CHK;

	AF_CHK(af_populate_features(ctx));

	return AF_ERR_NONE;
}

enum af_err af_killctx(struct af_ctx* ctx) {
	AF_CTX_CHK(ctx);

	ctx->free(ctx->extensions);
	ctx->free(ctx->drawlists.handles);

	return AF_ERR_NONE;
}

enum af_err af_setview(struct af_ctx* ctx, af_uint_t w, af_uint_t h) {
	AF_CTX_CHK(ctx);

	glViewport(0, 0, (int) w, (int) h);
	AF_GL_CHK;

	return AF_ERR_NONE;
}

static enum af_err af_set_gl_hints(enum af_fidelity fidelity) {
	af_uint_t mode = GL_DONT_CARE;
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

static enum af_err af_populate_features(struct af_ctx* ctx) {
	AF_CTX_CHK(ctx);

#ifdef GL_VERSION_1_1
	if(ctx->gl_ver[1] > 0) ctx->features.multitexture = AF_CORE;
#endif

#ifdef GL_VERSION_2_0
	if(ctx->gl_ver[0] > 1) ctx->features.buffers = AF_CORE;
#else
# if defined(GL_ARB_vertex_buffer_object) && !defined(NO_EXT)
	if(af_haveext(ctx, "GL_ARB_vertex_buffer_object")) {
		ctx->features.buffers = AF_ARB;
	}
# endif
#endif

#ifdef GL_VERSION_2_0
	if(ctx->gl_ver[0] > 1) ctx->features.shaders = AF_CORE;
#else
# if defined(GL_ARB_shader_objects) && !defined(NO_EXT)
	if(af_haveext(ctx, "GL_ARB_shader_objects")) {
		ctx->features.shaders = AF_ARB;
	}
# endif
#endif

	return AF_ERR_NONE;
}

af_bool_t af_haveext(struct af_ctx* ctx, const char* ext) {
	af_size_t i;

	for(i = 0; i < ctx->extensions_len; ++i) {
		if(af_streql(ctx->extensions[i], ext)) return AF_TRUE;
	}

	return AF_FALSE;
}
