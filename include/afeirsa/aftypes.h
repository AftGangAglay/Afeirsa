/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#ifndef AF_TYPES_H
#define AF_TYPES_H

typedef unsigned char	af_uchar_t;
typedef unsigned short	af_ushort_t;
typedef unsigned int	af_uint_t;
typedef unsigned long	af_ulong_t;

typedef signed char		af_schar_t;
typedef signed short	af_sshort_t;
typedef signed int		af_sint_t;
typedef signed long		af_slong_t;

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

#endif
