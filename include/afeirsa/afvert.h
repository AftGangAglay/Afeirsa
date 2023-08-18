/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_VERT_H
#define AF_VERT_H

#include <afeirsa/aferr.h>
#include <afeirsa/aftypes.h>
#include <afeirsa/afctx.h>

enum af_vert_element_type {
	AF_VERT_NONE,
	AF_VERT_POS,
	AF_VERT_COL,
	AF_VERT_UV
};

struct af_vert_element {
	af_size_t size;
	enum af_vert_element_type type;
};

struct af_vert {
	struct af_vert_element* elements;
	af_size_t len;
	af_size_t stride;

	af_uint_t gl_handle;
};

enum af_err af_mkvert(
		struct af_ctx* ctx, struct af_vert* vert,
		const struct af_vert_element* elements, af_size_t len);

enum af_err af_killvert(struct af_ctx* ctx, struct af_vert* vert);

#endif
