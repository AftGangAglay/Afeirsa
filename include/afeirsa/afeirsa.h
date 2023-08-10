/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_AFERISA_H
#define AF_AFEIRSA_H

#include <afeirsa/aftypes.h>

struct af_ver {
	af_uint_t major;
	af_uint_t minor;
};

void af_getver(struct af_ver* const ver);

#endif


