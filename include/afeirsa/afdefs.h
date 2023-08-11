/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_DEFS_H
#define AF_DEFS_H

#include <afeirsa/aferr.h>

#ifdef AF_NO_VERIFY
# define AF_PARAM_CHK(p)
#else
# define AF_PARAM_CHK(p) if(!p) return AF_ERR_BAD_PARAM;
#endif

#endif


