/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_BUF_H
#define AF_BUF_H

#include <afeirsa/aftypes.h>
#include <afeirsa/aferr.h>
#include <afeirsa/afctx.h>

enum af_buf_type {
	AF_BUF_NONE,
	AF_BUF_VERTEX,
	AF_BUF_INDEX,
	AF_BUF_TEXTURE,
	AF_BUF_UNIFORM
};

struct af_buf {
	af_uint_t gl_handle;
	enum af_buf_type type;
	void* storage;
	af_size_t size;
	af_bool_t native;
};

enum af_err af_mkbuf(
		struct af_ctx* ctx, struct af_buf* buf, enum af_buf_type type);
enum af_err af_killbuf(struct af_ctx* ctx, struct af_buf* buf);

enum af_err af_upload(
		struct af_ctx* ctx, struct af_buf* buf,
		const void* data, af_size_t size);

#endif


