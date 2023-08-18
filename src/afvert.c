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
	af_size_t req;

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(vert);
	AF_PARAM_CHK(elements);

	vert->stride = 0;

	for(i = 0; i < len; ++i) {
		switch(elements[i].type) {
			default: {
				req = 0;
				break;
			}
			case AF_VERT_COL: {
				req = 4;
				break;
			}
			case AF_VERT_POS: {
				req = 4;
				break;
			}
			case AF_VERT_UV: {
				req = 4;
				break;
			}
		}

		AF_VERIFY(elements[i].size == sizeof(float) * req, AF_ERR_BAD_PARAM);

		vert->stride += elements[i].size;
	}

	vert->elements = ctx->malloc(len * sizeof(struct af_vert_element));
	if(!vert->elements) return AF_ERR_MEM;
	af_memcpy(vert->elements, elements, len * sizeof(struct af_vert_element));

	vert->len = len;

	if(ctx->features.buffers) {
#ifdef GL_VERSION_2_0
		/* TODO: Native vertex attribs */
		return AF_ERR_NONE;
#endif
	}

	return AF_ERR_NONE;
}

enum af_err af_killvert(struct af_ctx* ctx, struct af_vert* vert) {

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(vert);

	if(ctx->features.buffers) {
#ifdef GL_VERSION_2_0
		/* TODO: Native vertex attribs */
		return AF_ERR_NONE;
#endif
	}

	ctx->free(vert->elements);

	return AF_ERR_NONE;
}
