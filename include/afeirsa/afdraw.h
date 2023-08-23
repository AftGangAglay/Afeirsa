/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
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
	AF_DRAW
};

enum af_primitive {
	AF_TRIANGLES,
	AF_TRIANGLE_STRIP,
	AF_TRIANGLE_FAN,

	AF_LINES,
	AF_LINE_STRIP,
	AF_LINE_LOOP,

	AF_QUADS,
	AF_QUAD_STRIP,

	AF_POINTS,
	AF_POLYGON
};

struct af_drawlist;
struct af_drawop {
	enum af_drawop_type type;
	union af_drawop_data {
		struct af_drawop_drawbuf {
			struct af_buf* buf;
			struct af_vert* vert;
			enum af_primitive primitive;
		} drawbuf;
		struct af_buf* settex;
		/* TODO: Mat */
		struct af_drawlist* draw;
	} data;
};

struct af_drawlist {
	struct af_drawop* ops;
	af_size_t len;

	af_uint_t gl_handle;
};

enum af_err af_drawbuf(
		struct af_ctx* ctx, struct af_buf* buf, struct af_vert* vert,
		enum af_primitive primitive);

enum af_err af_settex(struct af_ctx* ctx, struct af_buf* tex);

enum af_err af_mkdrawlist(
		struct af_ctx* ctx, struct af_drawlist* drawlist,
		struct af_drawop* ops, af_size_t len);

/*
 * NOTE: Does not kill associated data from passed operations. User is
 * 		 Responsible for killing the drawlist operation data passed to
 * 		 `af_mkdrawlist'.
 */
enum af_err af_killdrawlist(struct af_ctx* ctx, struct af_drawlist* drawlist);

enum af_err af_draw(struct af_ctx* ctx, struct af_drawlist* drawlist);

enum af_err af_clear(struct af_ctx* ctx, const float* col4f);

#endif
