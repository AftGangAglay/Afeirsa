/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afctx.h>
#include <afeirsa/afdefs.h>

enum af_err af_mkctx(struct af_ctx* ctx, const struct af_gl_ver* gl_ver) {
	AF_PARAM_CHK(ctx);
	AF_PARAM_CHK(gl_ver);

	ctx->gl_ver = *gl_ver;

	return AF_ERR_NONE;
}

