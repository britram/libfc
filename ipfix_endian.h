/* endian.h -- copied from boost/detail/endian.h to be independent of boost.
 * 
 * Modified 2014-04-22 by Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 * for libfc.
 */

/*
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

// Copyright 2005 Caleb Epstein
// Copyright 2006 John Maddock
// Copyright 2010 Rene Rivera
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/*
 * Copyright (c) 1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/*
 * Copyright notice reproduced from <boost/detail/limits.hpp>, from
 * which this code was originally taken.
 *
 * Modified by Caleb Epstein to use <endian.h> with GNU libc and to
 * defined the BOOST_ENDIAN macro.
 */

#ifndef _IPFIX_ENDIAN_H_
#  define _IPFIX_ENDIAN_H_

// GNU libc offers the helpful header <endian.h> which defines
// __BYTE_ORDER

/* For some reason, clang lies and defines __GNUC__. WTF, clang?? */
#  if defined(__GNUC__) && !defined(__clang__)
#   include <endian.h>
#   if (__BYTE_ORDER == __LITTLE_ENDIAN)
#    define IPFIX_LITTLE_ENDIAN
#   elif (__BYTE_ORDER == __BIG_ENDIAN)
#    define IPFIX_BIG_ENDIAN
#   elif (__BYTE_ORDER == __PDP_ENDIAN)
#    define IPFIX_PDP_ENDIAN
#   else
#    error Unknown machine endianness detected.
#   endif
#   define IPFIX_BYTE_ORDER __BYTE_ORDER
#  elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN) || \
    defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__) || \
    defined(_STLP_BIG_ENDIAN) && !defined(_STLP_LITTLE_ENDIAN)
#   define IPFIX_BIG_ENDIAN
#   define IPFIX_BYTE_ORDER 4321
#  elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN) || \
    defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__) || \
    defined(_STLP_LITTLE_ENDIAN) && !defined(_STLP_BIG_ENDIAN)
#   define IPFIX_LITTLE_ENDIAN
#   define IPFIX_BYTE_ORDER 1234
#  elif defined(__sparc) || defined(__sparc__) \
   || defined(_POWER) || defined(__powerpc__) \
   || defined(__ppc__) || defined(__hpux) || defined(__hppa) \
   || defined(_MIPSEB) || defined(_POWER) \
   || defined(__s390__)
#   define IPFIX_BIG_ENDIAN
#   define IPFIX_BYTE_ORDER 4321
#  elif defined(__i386__) || defined(__alpha__) \
   || defined(__ia64) || defined(__ia64__) \
   || defined(_M_IX86) || defined(_M_IA64) \
   || defined(_M_ALPHA) || defined(__amd64) \
   || defined(__amd64__) || defined(_M_AMD64) \
   || defined(__x86_64) || defined(__x86_64__) \
   || defined(_M_X64) || defined(__bfin__)

#   define IPFIX_LITTLE_ENDIAN
#   define IPFIX_BYTE_ORDER 1234
#  else
#   error The file endian.h needs to be set up for your CPU type.
#  endif


#endif /* _IPFIX_ENDIAN_H_ */
