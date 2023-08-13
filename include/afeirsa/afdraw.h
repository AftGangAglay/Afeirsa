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

enum af_err af_drawbuf(
		struct af_ctx* ctx, struct af_buf* buf, struct af_vert* vert);

#endif
