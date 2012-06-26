/* Copyright (c) 2011, NEC Europe Ltd, Consorzio Nazionale 
 * Interuniversitario per le Telecomunicazioni, Institut 
 * Telecom/Telecom Bretagne, ETH Zürich, INVEA-TECH a.s. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of NEC Europe Ltd, Consorzio Nazionale 
 *      Interuniversitario per le Telecomunicazioni, Institut Telecom/Telecom 
 *      Bretagne, ETH Zürich, INVEA-TECH a.s. nor the names of its contributors 
 *      may be used to endorse or promote products derived from this software 
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT 
 * HOLDERBE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */

#include <cerrno>
#include <cstring>

#include "FileWriter.h"

#include "exceptions/IOError.h"

namespace IPFIX {
  
void FileWriter::_sendMessage(uint8_t *base, size_t len) {
  int rv;
  
  //  std::cerr << "FileWriter::sendMessage(" << reinterpret_cast<unsigned long> (base)<< ", " << len << ")" << std::endl;
  if (!fp_) {
    if (filename_ == "-") {
        fp_ = stdout;
    } else {
        fp_ = fopen(filename_.c_str(), "w");
    }
    if (!fp_) {
      throw IOError(strerror(errno));
    }
  }
	
  //	std::cerr << "fp is " << reinterpret_cast<unsigned long> (fp_) << std::endl;
  if ((rv = fwrite((void *)base, len, 1, fp_)) < 0) {
    throw IOError(strerror(errno));
  }
  
  
  //std::cerr << "after fwrite(), rv = " << rv << std::endl;
}

FileWriter::~FileWriter() {
  if (fp_) {
    flush();
    fclose(fp_);
    fp_ = NULL;
  }

}

}
