/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_ERR_H
#define AF_ERR_H

#ifdef __cplusplus
extern "C" {
#endif

	enum af_err {
	AF_ERR_NONE,
	AF_ERR_UNKNOWN,
	AF_ERR_BAD_PARAM,
	AF_ERR_BAD_CTX,
	AF_ERR_BAD_OP,
	AF_ERR_NO_GL,
	AF_ERR_MEM
};

#define AF_CHK(c) \
	do { \
		enum af_err err = c; \
		if(err) return err; \
	} while(0)

#ifdef __cplusplus
};
#endif

#endif
