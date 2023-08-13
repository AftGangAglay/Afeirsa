/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afvert.h>
#include <afeirsa/afdefs.h>

enum af_err af_mkvert(
		struct af_ctx* ctx, struct af_vert* vert,
		const struct af_vert_element* elements, af_size_t len) {

	af_size_t i;

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(vert);
	AF_PARAM_CHK(elements);

	vert->stride = 0;
	for(i = 0; i < len; ++i) {
		switch(elements[i].type) {
			default: break;
			case AF_VERT_COL:
				AF_FALLTHROUGH;
				/* FALLTHRU */
			case AF_VERT_POS: {
				AF_VERIFY(
					elements[i].size == sizeof(float[4]), AF_ERR_BAD_PARAM);
				break;
			}
		}

		vert->stride += elements[i].size;
	}

	vert->elements = elements;
	vert->len = len;

	if(ctx->features.buffers) {
#ifdef GL_VERSION_2_0
		return AF_ERR_NONE;
#endif
	}

	return AF_ERR_NONE;
}
