/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_BUF_H
#define AF_BUF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <afeirsa/aftypes.h>
#include <afeirsa/aferr.h>
#include <afeirsa/afctx.h>

enum af_buf_type {
	AF_BUF_VERT,
	AF_BUF_IND,
	AF_BUF_TEX
};

struct af_buf {
	af_uint_t gl_handle;
	enum af_buf_type type;
	const void* storage;
	af_size_t tex_width;
	af_size_t size;
};

/*
 * NOTE: When making texture buffers. `tex_width' must be set before an
 * 		 Upload is performed.
 */
enum af_err af_mkbuf(
		struct af_ctx* ctx, struct af_buf* buf, enum af_buf_type type);
enum af_err af_killbuf(struct af_ctx* ctx, struct af_buf* buf);

enum af_err af_upload(
		struct af_ctx* ctx, struct af_buf* buf,
		const void* data, af_size_t size);

#ifdef __cplusplus
};
#endif

#endif


