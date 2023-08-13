/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afdefs.h>

/* TODO: Document this as a build option */
#ifdef USE_STDLIB
# include <string.h>
#endif

void* af_memcpy(void* dst, const void* src, af_size_t n) {
#ifdef USE_STDLIB
	return memcpy(dst, src, n);
#else
	af_size_t i;
	for(i = 0; i < n; ++i) ((char*) dst)[i] = ((char*) src)[i];

	return dst;
#endif
}
