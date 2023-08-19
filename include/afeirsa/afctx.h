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

struct af_gl_ver {
	af_uint_t major;
	af_uint_t minor;
};

typedef void (*af_free_t)(void*);
typedef void* (*af_malloc_t)(af_size_t);
typedef void* (*af_realloc_t)(void*, af_size_t);

struct af_features {
	af_bool_t buffers; /* TODO: Criteria for this? */
	af_bool_t multitexture;
};

struct af_ctx {
	struct af_gl_ver gl_ver;

	/*
	 * NOTE: These need to be set before the initial call to `af_mkctx' if
	 * 		 `USE_STDLIB' is not defined.
	 */
	af_free_t free;
	af_malloc_t malloc;
	af_realloc_t realloc;

	af_size_t extensions_len;
	char** extensions;

	af_size_t drawlists_len;
	af_uint_t* drawlists;

	struct af_features features;
};

enum af_err af_mkctx(
		struct af_ctx* ctx, const struct af_gl_ver* gl_ver,
		enum af_fidelity fidelity);
enum af_err af_killctx(struct af_ctx* ctx);

enum af_err af_setview(struct af_ctx* ctx, af_uint_t w, af_uint_t h);

#endif
