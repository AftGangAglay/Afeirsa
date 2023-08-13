/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afctx.h>
#include <afeirsa/afdefs.h>

#ifdef USE_STDLIB
# include <stdlib.h>
#endif

enum af_err af_mkctx(struct af_ctx* ctx, const struct af_gl_ver* gl_ver) {
	AF_PARAM_CHK(ctx);
	AF_PARAM_CHK(gl_ver);

	ctx->gl_ver = *gl_ver;

	af_memset(&ctx->features, 0, sizeof(struct af_features));

#ifdef USE_STDLIB
	ctx->malloc = malloc;
	ctx->free = free;
	ctx->realloc = realloc;
#endif

	return AF_ERR_NONE;
}

enum af_err af_killctx(struct af_ctx* ctx) {
	AF_CTX_CHK(ctx);

	return AF_ERR_NONE;
}
