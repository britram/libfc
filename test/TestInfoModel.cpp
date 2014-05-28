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

#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "InfoElement.h"
#include "InfoModel.h"

BOOST_AUTO_TEST_SUITE(Basics)

BOOST_AUTO_TEST_CASE(InfoModel) {
    libfc::InfoModel& m = libfc::InfoModel::instance();

    // we're going to do default info model stuff
    m.defaultIPFIX();
    
    // make sure we only have one instance
    libfc::InfoModel& mcheck = libfc::InfoModel::instance();
    BOOST_CHECK_EQUAL(&m, &mcheck);

    // check a few IEs that should be there
    BOOST_CHECK_EQUAL(m.lookupIE("octetDeltaCount")->number(), 1);
    BOOST_CHECK_EQUAL(m.lookupIE("octetDeltaCount")->pen(), 0U);
    BOOST_CHECK_EQUAL(m.lookupIE("octetDeltaCount")->len(), 8);
    
    // check an IE that shouldn't
    BOOST_CHECK_EQUAL(m.lookupIE("thisIsNotAnInformationElement"), (void *)0);
}

BOOST_AUTO_TEST_CASE(InfoElement01) {
  libfc::InfoModel& m = libfc::InfoModel::instance();

  m.defaultIPFIX();
    
  const libfc::InfoElement* e = m.lookupIE("octetDeltaCount");
  BOOST_REQUIRE(e != 0);

  BOOST_CHECK_EQUAL(e->toIESpec(), "octetDeltaCount(1)<unsigned64>[8]");
}

BOOST_AUTO_TEST_SUITE_END()
