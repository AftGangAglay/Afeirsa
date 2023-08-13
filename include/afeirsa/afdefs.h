/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_DEFS_H
#define AF_DEFS_H

#include <afeirsa/aferr.h>
#include <afeirsa/aftypes.h>

#ifdef AF_NO_VERIFY
# define AF_PARAM_CHK(ctx)
# define AF_CTX_CHK(p)
# define AF_VERIFY(expr, err)
#else
# define AF_PARAM_CHK(ctx) if(!ctx) return AF_ERR_BAD_PARAM
# define AF_CTX_CHK(p) if(!p) return AF_ERR_BAD_CTX
# define AF_VERIFY(expr, err) if(!(expr)) return (err)
#endif

#if defined(__clang__) || defined(__GNUC__)
# define AF_HAVE_GNU
#endif

#ifdef AF_HAVE_GNU
# if __has_attribute(fallthrough)
#  define AF_FALLTHROUGH __attribute__((fallthrough))
# endif
#endif

#define AF_ARRLEN(arr) (sizeof((arr)) / sizeof((arr)[0]))
#define AF_MEMBSIZE(t, memb) (sizeof((t*) 0)->memb)

void* af_memcpy(void* dst, const void* src, af_size_t n);
void* af_memset(void* dst, int c, af_size_t n);

#endif


