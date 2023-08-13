/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afgl.h>
#include <afeirsa/afdefs.h>
#include <afeirsa/afdraw.h>

enum af_err af_drawbuf(
		struct af_ctx* ctx, struct af_buf* buf, struct af_vert* vert) {

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(buf);
	AF_PARAM_CHK(vert);

	/*
	 * TODO: Should we be checking ctx.feature or buf here?
	 * 		 The end result is the same but I'm not sure what the clearer/
	 * 		 More consistent option is.
	 */
	if(buf->native) {
#ifdef GL_VERSION_2_0
		/* TODO: Native buffer draw */
		return AF_ERR_NONE;
#endif
	}

	glBegin(GL_TRIANGLES);
	{
		af_size_t i;
		for(i = 0; i < buf->size; i += vert->stride) {
			af_size_t j;
			float* vertex = (float*) (((char*) buf->storage) + i);
			af_size_t off = 0;
			for(j = 0; j < vert->len; ++j) {
				const struct af_vert_element* element = &vert->elements[j];
				void (*vproc)(const float*) = glVertex4fv;

				switch(element->type) {
					default: break;
					case AF_VERT_COL: {
						vproc = glColor4fv;
						AF_FALLTHROUGH;
						/* FALLTHRU */
					}
					case AF_VERT_POS: {
						vproc(vertex + off);
						off += 4;
						break;
					}
				}
			}
		}
	}

    glEnd();
	AF_GL_CHK;

	return AF_ERR_NONE;
}
