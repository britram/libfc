/* Hi Emacs, please use -*- mode: C++; -*- */
/* Copyright (c) 2011-2014 ETH Zürich. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of ETH Zürich nor the names of other contributors 
 *      may be used to endorse or promote products derived from this software 
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH 
 * ZURICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file Pointer validity checks.
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 */
#ifndef _LIBFC_POINTER_CHECKS_H_
#  define _LIBFC_POINTER_CHECKS_H_

/** Checks if a pointer is within a given range.
 *
 * This macro checks if a pointer is within a range defined by a
 * pointer to the first element of the range and a pointer to one
 * beyond the last element of the range.
 *
 * @warning The first argument will be evaluated twice!
 *
 * @param p the pointer to check
 * @param start pointer to first element of the range
 * @param end pointer to one beyond the last element of the range
 *
 * @return 0 if the check fails, non-0 otherwise.
 */
#define CHECK_POINTER_WITHIN(p, start, end) \
  ((start) <= (p) && (p) < (end))

/** Checks if a pointer is within a given range, fail if not.
 *
 * This macro checks if a pointer is within a range defined by a
 * pointer to the first element of the range and a pointer to one
 * beyond the last element of the range.  If the check fails, a failed
 * assertion is generated.
 *
 * @warning The first argument will be evaluated twice!
 *
 * @param p the pointer to check
 * @param start pointer to first element of the range
 * @param end pointer to one beyond the last element of the range
 */
#define ASSERT_POINTER_WITHIN(p, start, end) \
  do { \
    assert(CHECK_POINTER_WITHIN((p), (start), (end))); \
  while (0)

/** Checks if a pointer is within a given range.
 *
 * This macro checks if a pointer is within a range defined by a
 * pointer to the first element of the range and a pointer to the last
 * element of the range.
 *
 * @param p the pointer to check
 * @param start pointer to first element of the range
 * @param end pointer to last element of the range
 *
 * @return 0 if the check fails, non-0 otherwise.
 */
#define CHECK_POINTER_WITHIN_I(p, start, end) \
  ((start) <= (p) && (p) <= (end))

/** Checks if a pointer is within a given range, fail if not.
 *
 * This macro checks if a pointer is within a range defined by a
 * pointer to the first element of the range and a pointer to the last
 * element of the range.  If the check fails, a failed assertion is
 * generated.
 *
 * @param p the pointer to check
 * @param start pointer to first element of the range
 * @param end pointer to last element of the range
 */
#define ASSERT_POINTER_WITHIN_I(p, start, end) \
  do { \
    assert(CHECK_POINTER_WITHIN_I((p), (start), (end));	\
  while (0)

#endif // _LIBFC_POINTER_CHECKS_H_
