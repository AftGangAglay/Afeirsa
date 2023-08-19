/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_GL_H
#define AF_GL_H

#ifdef __APPLE__
# define GL_SILENCE_DEPRECATION 1
# include <OpenGL/gl.h>
# include <OpenGL/glext.h>
/* TODO: Detect this from SDK ver or otherwise
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED 1
# include <OpenGL/gl3.h>
# include <OpenGL/gl3ext.h>
*/
# include <OpenGL/glu.h>
# undef GL_SILENCE_DEPRECATION
#else
# include <GL/gl.h>
# include <GL/glext.h>
# include <GL/glu.h>
#endif

#include <afeirsa/aferr.h>

#define AF_GL_CHK \
	do { \
		enum af_err err = af_gl_chk(); \
		if(err) return err; \
	} while(0)

enum af_err af_gl_chk(void);
void af_gl_err_clear(void);

#endif


