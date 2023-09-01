/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_PARAM_H
#define AF_PARAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <afeirsa/aferr.h>
#include <afeirsa/afctx.h>
#include <afeirsa/aftypes.h>

/*
 * NOTE: "Params" effectively act as a way to abstract fixed-pipeline
 * 		 State such as the matrix stack in a manner which can also be used
 * 		 For shaders through uniforms.
 */

enum af_param_type {
	AF_PARAM_NONE,
	AF_PARAM_MODELVIEW,
	AF_PARAM_PROJECTION
};

struct af_param {
	enum af_param_type type;
	enum af_datatype datatype;
	af_size_t count;
};

/*
 * NOTE: `name' does not serve a purpose outside of shader-based environments.
 */
enum af_err af_setparam(
		struct af_ctx* ctx, const char* name, const struct af_param* param,
		const void* data);

#ifdef __cplusplus
};
#endif

#endif
