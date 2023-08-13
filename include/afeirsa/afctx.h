/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_CTX_H
#define AF_CTX_H

#include <afeirsa/aftypes.h>
#include <afeirsa/aferr.h>

struct af_gl_ver {
	af_uint_t major;
	af_uint_t minor;
};

typedef void (*af_free_t)(void*);
typedef void* (*af_malloc_t)(af_size_t);
typedef void* (*af_realloc_t)(void*, af_size_t);

struct af_features {
	af_bool_t buffers; /* TODO: Criteria for this? */
};

struct af_ctx {
	struct af_gl_ver gl_ver;

	af_free_t free;
	af_malloc_t malloc;
	af_realloc_t realloc;

	struct af_features features;
};

enum af_err af_mkctx(struct af_ctx* ctx, const struct af_gl_ver* gl_ver);
enum af_err af_killctx(struct af_ctx* ctx);

#endif
