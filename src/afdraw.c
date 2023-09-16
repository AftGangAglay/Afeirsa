/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
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
	/* GCC fails to see this is unreachable (assuming well-behaved enum) */
	return GL_NONE;
}

enum af_err af_drawbuf(
		struct af_ctx* ctx, struct af_buf* buf, struct af_vert* vert,
		enum af_primitive primitive) {

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(buf);
	AF_PARAM_CHK(vert);

	AF_VERIFY(buf->type == AF_BUF_VERT, AF_ERR_BAD_PARAM);

	if(ctx->features.buffers) {
		/* TODO: Unclear how to use this from extension */
#ifdef GL_VERSION_2_0
		glDrawArrays(
			af_gl_primitive(primitive),
			0, (int) (buf->size / vert->stride));
		AF_GL_CHK;
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
				const af_size_t count = element->size / sizeof(float);
				typedef void (*af_attrib_proc_t)(const float*);
				const af_attrib_proc_t* procs = 0;
				af_size_t min;
				af_size_t max;

				/* GL procs might not be compile-time constants (e.g. glad) */
				af_attrib_proc_t col_procs[2];
				af_attrib_proc_t pos_procs[3];
				af_attrib_proc_t uv_procs[4];
				af_attrib_proc_t norm_proc = glNormal3fv;
				switch(element->type) {
					default: break;
					case AF_VERT_COL: {
						col_procs[0] = glColor3fv;
						col_procs[1] = glColor4fv;
						procs = col_procs;
						min = 3;
						max = 4;
						break;
					}
					case AF_VERT_POS: {
						pos_procs[0] = glVertex2fv;
						pos_procs[1] = glVertex3fv;
						pos_procs[2] = glVertex4fv;
						procs = pos_procs;
						min = 2;
						max = 4;
						break;
					}
					case AF_VERT_UV: {
						uv_procs[0] = glTexCoord1fv;
						uv_procs[1] = glTexCoord2fv;
						uv_procs[2] = glTexCoord3fv;
						uv_procs[3] = glTexCoord4fv;
						procs = uv_procs;
						min = 1;
						max = 4;
						break;
					}
					case AF_VERT_NORM: {
						procs = &norm_proc;
						min = 3;
						max = 3;
						break;
					}
				}

				if(procs) {
					if(count < min || count > max) return AF_ERR_BAD_OP;
					procs[count - min](vertex + off);
					off += count;
				}
			}
		}
	}

	glEnd();
	AF_GL_CHK;

	return AF_ERR_NONE;
}

enum af_err af_settex(struct af_ctx* ctx, struct af_buf* tex) {
	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(tex);
	AF_VERIFY(tex->type == AF_BUF_TEX, AF_ERR_BAD_PARAM);

	if(ctx->features.multitexture) {
#ifdef GL_VERSION_1_1
		glBindTexture(GL_TEXTURE_2D, tex->gl_handle);
		AF_GL_CHK;

		return AF_ERR_NONE;
#endif
	}

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		(int) tex->tex_width, (int) (tex->size / (4 * tex->tex_width)), 0,
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
	drawlist->ops = ctx->malloc(len * sizeof(struct af_drawop));
	AF_VERIFY(drawlist->ops, AF_ERR_MEM);

	af_memcpy(drawlist->ops, ops, len * sizeof(struct af_drawop));

	if(ctx->features.buffers) {
#ifdef GL_VERSION_2_0
		/* TODO: Native buffer drawlists */
		return AF_ERR_NONE;
#endif
	}

	{
		af_size_t i;
		AF_CHK(af_mkhandle(ctx, &ctx->drawlists, &drawlist->gl_handle));

		glNewList(drawlist->gl_handle, GL_COMPILE);
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
					case AF_DRAW: {
						AF_CHK(af_draw(ctx, drawlist));
						break;
					}
					case AF_SETMAT: {
						break;
					}
					case AF_SETTEX: {
						struct af_buf* tex = ops[i].data.settex;
						AF_CHK(af_settex(ctx, tex));
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

	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(drawlist);

	ctx->free(drawlist->ops);

	if(ctx->features.buffers) {
#ifdef GL_VERSION_2_0
		/* TODO: Native buffer drawlists */
		return AF_ERR_NONE;
#endif
	}

	AF_CHK(af_killhandle(ctx, &ctx->drawlists, drawlist->gl_handle));

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

enum af_err af_clear(struct af_ctx* ctx, const float* col4f) {
	AF_CTX_CHK(ctx);
	AF_PARAM_CHK(col4f);

	glClearColor(col4f[0], col4f[1], col4f[2], col4f[3]);
	AF_GL_CHK;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	AF_GL_CHK;

	return AF_ERR_NONE;
}
