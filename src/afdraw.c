/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afgl.h>
#include <afeirsa/afdefs.h>
#include <afeirsa/afdraw.h>

static af_uint_t af_gl_primitive(enum af_primitive primitive) {
	switch(primitive) {
		case AF_TRIANGLES: return GL_TRIANGLES;
        case AF_TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
        case AF_TRIANGLE_FAN: return GL_TRIANGLE_FAN;
		case AF_LINES: return GL_LINES;
        case AF_LINE_STRIP: return GL_LINE_STRIP;
        case AF_LINE_LOOP: return GL_LINE_LOOP;
		case AF_QUADS: return GL_QUADS;
        case AF_QUAD_STRIP: return GL_QUAD_STRIP;
		case AF_POINTS: return GL_POINTS;
		case AF_POLYGON: return GL_POLYGON;
	}
}

enum af_err af_drawbuf(
		struct af_ctx* ctx, struct af_buf* buf, struct af_vert* vert,
		enum af_primitive primitive) {

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(buf);
	AF_PARAM_CHK(vert);

	if(ctx->features.buffers) {
#ifdef GL_VERSION_2_0
		/* TODO: Native buffer draw */
		return AF_ERR_NONE;
#endif
	}

	glBegin(af_gl_primitive(primitive));
	{
		af_size_t i;
		for(i = 0; i < buf->size; i += vert->stride) {
			af_size_t j;
			float* vertex = (float*) (((char*) buf->storage) + i);
			af_size_t off = 0;
			for(j = 0; j < vert->len; ++j) {
				const struct af_vert_element* element = &vert->elements[j];
				void (*vproc)(const float*) = 0;

				af_size_t stride;
				switch(element->type) {
					default: break;
					case AF_VERT_COL: {
						vproc = glColor4fv;
						stride = 4;
						break;
					}
					case AF_VERT_POS: {
						vproc = glVertex4fv;
						stride = 4;
						break;
					}
					case AF_VERT_UV: {
						vproc = glTexCoord4fv;
						stride = 4;
						break;
					}
				}

				if(vproc) {
					vproc(vertex + off);
					off += stride;
				}
			}
		}
	}

    glEnd();
	AF_GL_CHK;

	return AF_ERR_NONE;
}

enum af_err af_settex(
		struct af_ctx* ctx, struct af_buf* tex, af_size_t width) {

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(tex);
	AF_VERIFY(tex->type == AF_BUF_TEXTURE, AF_ERR_BAD_PARAM);

	if(ctx->features.multitexture) {
#ifdef GL_VERSION_1_1
		/* TODO: Textures w/ explicit binding points */
		return AF_ERR_NONE;
#endif
	}

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		(int) width, (int) (tex->size / (4 * width)), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tex->storage);
	AF_GL_CHK;

	/* No mipmapping yet */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	AF_GL_CHK;

	return AF_ERR_NONE;
}

enum af_err af_mkdrawlist(
		struct af_ctx* ctx, struct af_drawlist* drawlist,
		struct af_drawop* ops, af_size_t len) {

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(drawlist);
	AF_PARAM_CHK(ops);

	drawlist->len = len;
	if(!( drawlist->ops = ctx->malloc(len * sizeof(struct af_drawop)) )) {
		return AF_ERR_MEM;
	}

	af_memcpy(drawlist->ops, ops, len * sizeof(struct af_drawop));

	if(ctx->features.buffers) {
#ifdef GL_VERSION_2_0
		/* TODO: Native buffer drawlists */
		return AF_ERR_NONE;
#endif
	}

	{
		af_uint_t gl_handle = 0;
		af_size_t i;

		continue2:;
		gl_handle++;

		for(i = 0; i < ctx->drawlists_len; ++i) {
			if(ctx->drawlists[i] == gl_handle) goto continue2;
		}

		drawlist->gl_handle = gl_handle;

		ctx->drawlists = ctx->realloc(
								ctx->drawlists,
								++ctx->drawlists_len * sizeof(af_uint_t));
		if(!( ctx->drawlists )) return AF_ERR_MEM;
		ctx->drawlists[ctx->drawlists_len - 1] = gl_handle;

		glNewList(gl_handle, GL_COMPILE);
		{
			for(i = 0; i < len; ++i) {
				switch(ops[i].type) {
					case AF_DRAWBUF: {
						struct af_drawop_drawbuf* drawbuf =
							&ops[i].data.drawbuf;
						AF_CHK(af_drawbuf(
								ctx, drawbuf->buf, drawbuf->vert,
								drawbuf->primitive));
						break;
					}
					case AF_RUNDRAW: {
						AF_CHK(af_draw(ctx, drawlist));
						break;
					}
					case AF_SETMAT: {
						break;
					}
					case AF_SETTEX: {
						struct af_drawop_settex* settex =
							&ops[i].data.settex;
						AF_CHK(af_settex(ctx, settex->tex, settex->width));
						break;
					}
				}
			}
		}
		glEndList();
		AF_GL_CHK;
	}

	return AF_ERR_NONE;
}

enum af_err af_killdrawlist(struct af_ctx* ctx, struct af_drawlist* drawlist) {

	af_uint_t* copy;
	af_size_t i;
	af_size_t pos = 0;

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(drawlist);

	ctx->free(drawlist->ops);

	if(ctx->features.buffers) {
#ifdef GL_VERSION_2_0
		/* TODO: Native buffer drawlists */
		return AF_ERR_NONE;
#endif
	}

	copy = ctx->malloc((ctx->drawlists_len - 1) * sizeof(af_uint_t));
	if(!copy) return AF_ERR_MEM;

	for(i = 0; i < ctx->drawlists_len; ++i) {
		if(ctx->drawlists[i] == drawlist->gl_handle) continue;

		copy[pos++] = ctx->drawlists[i];
	}

	ctx->free(ctx->drawlists);
	ctx->drawlists = copy;
	ctx->drawlists_len--;

	glDeleteLists(drawlist->gl_handle, 1);
	AF_GL_CHK;

	return AF_ERR_NONE;
}

enum af_err af_draw(struct af_ctx* ctx, struct af_drawlist* drawlist) {

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(drawlist);

	if(ctx->features.buffers) {
#ifdef GL_VERSION_2_0
		/* TODO: Native buffer drawlists */
		return AF_ERR_NONE;
#endif
	}

	glCallList(drawlist->gl_handle);
	AF_GL_CHK;

	return AF_ERR_NONE;
}
