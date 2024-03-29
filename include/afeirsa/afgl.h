/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_GL_H
#define AF_GL_H

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(AF_NSGL) && !defined(AF_GLXABI) && !defined(AF_WGL)
# ifdef AF_BUILD
#  pragma "Auto-detecting GL backend during in-tree build"
# endif
# ifdef __APPLE__
#  define AF_NSGL
# elif defined(_WIN32)
#  define AF_WGL
# else
#  define AF_GLXABI
# endif
#endif

/*
 * NOTE: We include GLUT and GLU here even though we don't currently use them
 * 		 internally as it provides the user with a GL ABI consistent version of
 * 		 the respective libraries - which avoids the user needing to use the
 * 		 `AF_NSGL' and `AF_GLXABI' macros themselves.
 */

#ifdef AF_NSGL
# define GL_SILENCE_DEPRECATION 1
# include <OpenGL/gl.h>
# include <OpenGL/glext.h>
# include <OpenGL/glu.h>
# ifdef AF_WANT_GLUT
#  include <GLUT/glut.h>
# endif
# undef GL_SILENCE_DEPRECATION
#endif

#ifdef AF_WGL
/*
 * This is super annoying as it leaks a load of garbage into scope.
 * `windows.h' is needed for declaration attributes.
 * `stddef.h' is needed for `wchar_t'.
 */
# ifndef AF_WGL_SUPPRESS_AUX
#  include <windows.h>
#  include <stddef.h>
# endif
# include <GL/gl.h>
# include <GL/glu.h>
#endif

#ifdef AF_GLXABI
# define GL_GLEXT_PROTOTYPES
# include <GL/gl.h>
# include <GL/glext.h>
# include <GL/glu.h>
# ifdef AF_WANT_GLUT
#  include <GL/glut.h>
# endif
# include <GL/glx.h>
# undef GL_GLEXT_PROTOTYPES
#endif

#ifdef GL10_COMPAT
# undef GL_VERSION_1_1
# undef GL_VERSION_1_2
# undef GL_VERSION_1_3
# undef GL_VERSION_1_4
# undef GL_VERSION_1_5

# undef GL_VERSION_2_0
# undef GL_VERSION_2_1

# undef GL_VERSION_3_0
# undef GL_VERSION_3_1
# undef GL_VERSION_3_2
# undef GL_VERSION_3_3

# undef GL_VERSION_4_0
# undef GL_VERSION_4_1
# undef GL_VERSION_4_2
# undef GL_VERSION_4_3
# undef GL_VERSION_4_4
# undef GL_VERSION_4_5
# undef GL_VERSION_4_6
#endif

#if defined(GL_VERSION_2_0) || \
	defined(GL_ARB_vertex_buffer_object) && !defined(NO_EXT)

/* TODO: Feature macros for all used features. */
# define AF_BUFFER_FEATURE 1
#endif

#ifdef GL_VERSION_1_1
# define AF_TEXTURE_FEATURE 1
#endif

#include <afeirsa/aferr.h>

#define AF_GL_CHK \
	do { \
		enum af_err err = af_gl_chk(); \
		if(err) return err; \
	} while(0)

enum af_err af_gl_chk(void);
void af_gl_err_clear(void);

#ifdef __cplusplus
};
#endif

#endif


