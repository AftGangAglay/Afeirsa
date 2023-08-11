/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_ERR_H
#define AF_ERR_H

enum af_err {
	AF_ERR_NONE,
	AF_ERR_BAD_PARAM,
	AF_ERR_BAD_CTX
};

typedef void (*af_err_callback_t)(enum af_err, const char*);

#endif


