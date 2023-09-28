/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_TYPES_H
#define AF_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char	af_uchar_t;
typedef unsigned short	af_ushort_t;
typedef unsigned int	af_uint_t;
#ifdef _WINDOWS
	typedef unsigned long long af_ulong_t;
#else
	typedef unsigned long af_ulong_t;
#endif

typedef signed char		af_schar_t;
typedef signed short	af_sshort_t;
typedef signed int		af_sint_t;
#ifdef _WINDOWS
	typedef signed long long af_slong_t;
#else
	typedef signed long af_slong_t;
#endif

typedef af_uchar_t		af_uint8_t;
typedef af_ushort_t		af_uint16_t;
typedef af_uint_t		af_uint32_t;
typedef af_ulong_t		af_uint64_t;

typedef af_schar_t		af_sint8_t;
typedef af_sshort_t		af_sint16_t;
typedef af_sint_t		af_sint32_t;
typedef af_slong_t		af_sint64_t;

typedef af_ulong_t af_size_t;

typedef af_uchar_t af_bool_t;

#define AF_TRUE (1)
#define AF_FALSE (0)

enum af_datatype {
	AF_UCHAR,
	AF_USHORT,
	AF_UINT,
	AF_ULONG,

	AF_SCHAR,
	AF_SSHORT,
	AF_SINT,
	AF_SLONG,

	AF_FLOAT,
	AF_DOUBLE
};

#ifdef __cplusplus
};
#endif

#endif
