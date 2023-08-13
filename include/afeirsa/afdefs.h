/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_DEFS_H
#define AF_DEFS_H

#include <afeirsa/aferr.h>

#ifdef AF_NO_VERIFY
# define AF_PARAM_CHK(p)
# define AF_CTX_CHK(p)
#else
# define AF_PARAM_CHK(p) if(!p) return AF_ERR_BAD_PARAM
# define AF_CTX_CHK(p) if(!p) return AF_ERR_BAD_CTX
#endif

#define AF_ARRLEN(arr) (sizeof((arr)) / sizeof((arr)[0]))

void* af_memcpy(void* dst, const void* src, af_size_t n);

#endif


