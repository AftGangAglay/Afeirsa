/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_DRAW_H
#define AF_DRAW_H

#include <afeirsa/aferr.h>
#include <afeirsa/afctx.h>
#include <afeirsa/afbuf.h>
#include <afeirsa/afvert.h>

enum af_drawop_type {
	/*
	 * Normal drawing ops w/ vertices.
	 */
	AF_DRAWBUF,
	/*
	 * Set/re-upload texture.
	 */
	AF_SETTEX,
	/*
	 * Set material info.
	 */
	AF_SETMAT,
	/*
	 * Run a different drawlist.
	 */
	AF_RUNDRAW
};

struct af_drawlist;
struct af_drawop {
	enum af_drawop_type type;
	union af_drawop_data {
		struct af_drawop_drawbuf {
			struct af_buf* buf;
			struct af_vert* vert;
		} drawbuf;
		struct af_buf* settex;
		/* TODO: Mat */
		struct af_drawlist* rundraw;
	} data;
};

struct af_drawlist {
	struct af_drawop* ops;
	af_size_t len;

	af_uint_t gl_handle;
};

enum af_err af_drawbuf(
		struct af_ctx* ctx, struct af_buf* buf, struct af_vert* vert);

enum af_err af_mkdrawlist(
		struct af_ctx* ctx, struct af_drawlist* drawlist,
		struct af_drawop* ops, af_size_t len);

#endif
