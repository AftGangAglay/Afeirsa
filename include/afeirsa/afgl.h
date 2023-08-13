/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_GL_H
#define AF_GL_H

#ifdef __APPLE__
# define GL_SILENCE_DEPRECATION 1
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED 1
# include <OpenGL/gl.h>
/* TODO: Detect this from SDK ver or otherwise */
# include <OpenGL/gl3.h>
# include <OpenGL/glext.h>
# undef GL_SILENCE_DEPRECATION
#endif

#endif


