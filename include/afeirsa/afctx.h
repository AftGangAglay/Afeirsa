/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_CTX_H
#define AF_CTX_H

#include <afeirsa/aftypes.h>
#include <afeirsa/aferr.h>

struct af_gl_ver {
        af_uint_t major;
        af_uint_t minor;
};

struct af_ctx {
	struct af_gl_ver gl_ver;

	af_err_callback_t err_callback;
};

enum af_err af_mkctx(struct af_ctx* ctx, const struct af_gl_ver* gl_ver);

#endif


