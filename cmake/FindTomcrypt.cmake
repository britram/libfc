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

# Check for existence of tomcrypt.
# 
# Define Tomcrypt_ROOT as the directory that contains
# include/tomcrypt.h and lib/libtomcrypt.a
#

set (_Tomcrypt_INCLUDE_DIR_ "${Tomcrypt_ROOT}/include")
set (_Tomcrypt_LIBRARY_DIR_ "${Tomcrypt_ROOT}/lib")

find_path(Tomcrypt_INCLUDE_DIR 
          NAMES tomcrypt.h
          HINTS ${_Tomcrypt_INCLUDE_DIR_})

find_library(Tomcrypt_LIBRARY
             NAMES tomcrypt libtomcrypt
             HINTS ${_Tomcrypt_LIBRARY_DIR_})

set(Tomcrypt_LIBRARIES ${Tomcrypt_LIBRARY})
set(Tomcrypt_INCLUDE_DIRS ${Tomcrypt_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Tomcrypt DEFAULT_MSG
                                  Tomcrypt_LIBRARY Tomcrypt_INCLUDE_DIR) 
mark_as_advanced(Tomcrypt_INCLUDE_DIR Tomcrypt_LIBRARY Tomcrypt_LIBRARY_DIR)
