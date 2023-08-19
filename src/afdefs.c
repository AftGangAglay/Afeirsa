/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
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

void* af_memset(void* dst, int c, af_size_t n) {
#ifdef USE_STDLIB
	return memset(dst, c, n);
#else
	af_size_t i;
	for(i = 0; i < n; ++i) ((char*) dst)[i] = c;

	return dst;
#endif
}

af_size_t af_strlen(const char* s) {
#ifdef USE_STDLIB
	return strlen(s);
#else
	af_size_t n;
	for(n = 0; s[n]; ++n) continue;
	return n;
#endif
}

af_bool_t af_streql(const char* a, const char* b) {
#ifdef USE_STDLIB
	return !strcmp(a, b);
#else
	af_size_t n;
	while(AF_TRUE) {
		if(a[n] != b[n]) return AF_FALSE;
		if(!a[n]) return AF_TRUE;
		++n;
	}
#endif
}
