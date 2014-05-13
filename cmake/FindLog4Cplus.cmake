# Copyright (c) 2011-2014 ETH Zürich. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#    * Neither the names of ETH Zürich nor the names of other contributors 
#      may be used to endorse or promote products derived from this softwar 
#      without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH 
# ZURICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

# Check for existence of log4cplus.
# 
# Define Log4CPlus_ROOT as the directory that contains
# include/log4cplus and lib/liblog4cplus.a
#

set (_Log4CPlus_INCLUDE_DIR_ "${Log4CPlus_ROOT}/include")
set (_Log4CPlus_LIB_DIR_ "${Log4CPlus_ROOT}/lib")

find_path(Log4CPlus_INCLUDE_DIR 
          NAMES log4cplus/logger.h
          PATHS /opt/log4cplus/include
          HINTS ${_Log4CPlus_INCLUDE_DIR_})

find_library(Log4CPlus_LIBRARY
             NAMES liblog4cplus.a
             PATHS /opt/log4cplus/lib
             HINTS ${_Log4CPlus_LIB_DIR_})


message (STATUS "Log4cplus include ${Log4CPlus_INCLUDE_DIR}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Log4CPlus DEFAULT_MSG
                                  Log4CPlus_LIBRARY Log4CPlus_INCLUDE_DIR) 
mark_as_advanced(Log4CPlus_INCLUDE_DIR Log4CPlus_LIBRARY )
