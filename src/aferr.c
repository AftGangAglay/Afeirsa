/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/aferr.h>
#include <afeirsa/afgl.h>

enum af_err af_err_from_gl(af_uint_t gl_err) {
    switch(gl_err) {
        default: return AF_ERR_UNKNOWN;
        case GL_NO_ERROR: return AF_ERR_NONE;
        case GL_INVALID_ENUM: return AF_ERR_BAD_PARAM;
        case GL_INVALID_VALUE: return AF_ERR_BAD_PARAM;
        case GL_INVALID_OPERATION: return AF_ERR_BAD_OP;
        case GL_STACK_OVERFLOW: return AF_ERR_MEM;
        case GL_STACK_UNDERFLOW: return AF_ERR_MEM;
        case GL_OUT_OF_MEMORY: return AF_ERR_MEM;
    }
}