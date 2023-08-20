/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_CTX_H
#define AF_CTX_H

#include <afeirsa/aftypes.h>
#include <afeirsa/aferr.h>

enum af_fidelity {
	AF_FIDELITY_NONE,
	AF_FIDELITY_NICE,
	AF_FIDELITY_FAST
};

typedef void (*af_free_t)(void*);
typedef void* (*af_malloc_t)(af_size_t);
typedef void* (*af_realloc_t)(void*, af_size_t);

enum af_feature_level {
	AF_NOT_PRESENT,
	AF_CORE,
	AF_EXT,
	AF_ARB
};

struct af_features {
	enum af_feature_level buffers;
	enum af_feature_level multitexture;
	enum af_feature_level shaders;
};

struct af_handleset {
	af_uint_t* handles;
	af_size_t len;
};

struct af_ctx {
	af_uint_t gl_ver[2];

	/*
	 * NOTE: These need to be set before the initial call to `af_mkctx' if
	 * 		 `USE_STDLIB' is not defined.
	 */
	af_free_t free;
	af_malloc_t malloc;
	af_realloc_t realloc;

	af_size_t extensions_len;
	char** extensions;

	struct af_handleset drawlists;

	struct af_features features;
};

enum af_err af_mkctx(struct af_ctx* ctx, enum af_fidelity fidelity);
enum af_err af_killctx(struct af_ctx* ctx);

af_bool_t af_haveext(struct af_ctx* ctx, const char* ext);

enum af_err af_setview(struct af_ctx* ctx, af_uint_t w, af_uint_t h);

enum af_err af_mkhandle(
		struct af_ctx* ctx,
		struct af_handleset* set, af_uint_t* handle);

enum af_err af_killhandle(
		struct af_ctx* ctx,
		struct af_handleset* set, af_uint_t handle);

#endif
