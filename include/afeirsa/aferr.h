/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_ERR_H
#define AF_ERR_H

#include <afeirsa/aftypes.h>

enum af_err {
	AF_ERR_NONE,
	AF_ERR_UNKNOWN,
	AF_ERR_BAD_PARAM,
	AF_ERR_BAD_CTX,
	AF_ERR_BAD_OP,
	AF_ERR_MEM
};

enum af_err af_err_from_gl(af_uint_t gl_err);

/*
 * NOTE: We rely on the user to include GL for this to
 * 	 avoid polluting the global public API with GL
 * 	 includes.
 */
#define AF_GL_CHK \
		do { \
			af_uint_t gl_err; \
			while(( gl_err = glGetError() )) { \
	            enum af_err err = af_err_from_gl(gl_err); \
				if(err) return err; \
			} \
		} while(0)

#endif


