/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afgl.h>
#include <afeirsa/aftypes.h>

static enum af_err af_err_from_gl(af_uint_t gl_err) {
	switch(gl_err) {
		default: return AF_ERR_UNKNOWN;
		case GL_NO_ERROR: return AF_ERR_NONE;
		case GL_INVALID_ENUM: return AF_ERR_BAD_PARAM;
		case GL_STACK_OVERFLOW: return AF_ERR_MEM;
		case GL_INVALID_VALUE: return AF_ERR_BAD_PARAM;
		case GL_STACK_UNDERFLOW: return AF_ERR_MEM;
		case GL_INVALID_OPERATION: return AF_ERR_BAD_OP;
		case GL_OUT_OF_MEMORY: return AF_ERR_MEM;
	}
}

enum af_err af_gl_chk(void) {
	enum af_err err = af_err_from_gl(glGetError());
	if(err) return err;

	return AF_ERR_NONE;
}

void af_gl_err_clear(void) {
	while(glGetError()) continue;
}
