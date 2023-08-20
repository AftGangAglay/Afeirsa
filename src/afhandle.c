/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afctx.h>
#include <afeirsa/afdefs.h>

enum af_err af_mkhandle(
		struct af_ctx* ctx,
		struct af_handleset* set, af_uint_t* handle) {

	af_uint_t new_handle = 0;
	af_size_t i;

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(set);
	AF_PARAM_CHK(handle);

	continue2:;
	new_handle++;

	for(i = 0; i < set->len; ++i) {
		if(set->handles[i] == new_handle) goto continue2;
	}

	*handle = new_handle;

	set->handles = ctx->realloc(
						set->handles,
						++set->len * sizeof(af_uint_t));
	if(!( set->handles )) return AF_ERR_MEM;
	set->handles[set->len - 1] = new_handle;

	return AF_ERR_NONE;
}

enum af_err af_killhandle(
		struct af_ctx* ctx,
		struct af_handleset* set, af_uint_t handle) {

	af_uint_t* copy;
	af_size_t i;
	af_size_t pos = 0;

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(set);

	copy = ctx->malloc((set->len - 1) * sizeof(af_uint_t));
	if(!copy) return AF_ERR_MEM;

	for(i = 0; i < set->len; ++i) {
		if(set->handles[i] == handle) continue;

		copy[pos++] = set->handles[i];
	}

	ctx->free(set->handles);
	set->handles = copy;
	set->len--;

	return AF_ERR_NONE;
}
