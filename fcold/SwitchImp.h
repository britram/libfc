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
 * @file
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 * Implements the Intermediate Process for collecting flow records as from the SWITCH
 */

#ifndef _FCOLD_SWITCHIMP_H_
#  define _FCOLD_SWITCHIMP_H_

#include "Imp.h"
#include "PlacementExporter.h"

namespace fcold {
    
    /** This structure contains storage for a NetFlow version 5, 9,
        or IPFIX 5655 Store flow record. Not all fields are used in every
        application. Records of this type are used by the various placement
        templates in the intermediate process.
     */
    struct SwitchFlowRec {
        uint64_t packetDeltaCount;
        uint64_t octetDeltaCount;
        uint64_t flowStartMilliseconds;
        uint64_t flowEndMilliseconds;
        uint32_t flowStartSysUpTime;
        uint32_t flowEndSysUpTime;
        uint8_t  sourceIPv6Address[16];
        uint8_t  destinationIPv6Address[16];
        uint8_t  nextHopIPv6Address[16];
        uint32_t sourceIPv4Address;
        uint32_t destinationIPv4Address;
        uint32_t nextHopIPv4Address;
        uint32_t ingressInterface;
        uint32_t egressInterface;
        uint32_t bgpSourceAsNumber;
        uint32_t bgpDestinationAsNumber;
        uint32_t bgpPrevHopAsNumber;
        uint32_t bgpNextHopAsNumber;
        uint16_t sourceTransportPort;
        uint16_t destinationTransportPort;
        uint8_t tcpControlBits;
        uint8_t protocolIdentifier;
        uint8_t ipClassOfService;
        uint8_t sourcePrefixLength;
        uint8_t destinationrefixLength;
        uint8_t flowDirection;
    };
    
    struct SwitchMetadataRec {
        uint16_t messageScope;
        uint16_t exporterTransportPort;
        uint32_t exporterIPv4Address;
        uint64_t systemInitTimeMilliseconds;
        int16_t  basetimeCorrectionFactor;
    };
    
    class SwitchImp : public Imp {
        
    private:
        
        SwitchFlowRec            cf;
        SwitchMetadataRec        cm;
        
        libfc::PlacementExporter *exporter;

        libfc::PlacementTemplate *meta_exp;
        libfc::PlacementTemplate *flow4_exp;
        libfc::PlacementTemplate *flow6_exp;
        
        libfc::PlacementTemplate *v5_flow4_col;
        libfc::PlacementTemplate *v9_flow4_sdas_col;
        libfc::PlacementTemplate *v9_flow6_sdas_col;
        libfc::PlacementTemplate *v9_flow4_pnas_col;
        libfc::PlacementTemplate *v9_flow6_pnas_col;
        
    public:
        SwitchImp(std::string outfile_name,
                  uint32_t exporterIPv4Address,
                  uint16_t exporterTransportPort,
                  std::string libfc::PlacementCollector::Protocol protocol);
        ~SwitchImp();
    
    protected:
        virtual std::shared_ptr<libfc::ErrorContext>
                        start_message(std::shared_ptr<MessageBuffer> msg);
        virtual std::shared_ptr<libfc::ErrorContext> start_placement(
                                    const libfc::PlacementTemplate* tmpl);
        virtual std::shared_ptr<libfc::ErrorContext> end_placement(
                                    const libfc::PlacementTemplate* tmpl);


    };
} /* namespace fcold */

#endif /* defined(_FCOLD_SWITCHIMP_H_) */
