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
 */

#include "SwitchImp.h"

namespace fcold {

    SwitchImp::SwitchImp(libfc::PlacementCollector::Protocol protocol):
                    Imp(protocol),
                    exporter(nullptr),
                    meta_exp(nullptr),
                    flow4_exp(nullptr),
                    flow6_exp(nullptr),
                    v5_flow4_col(nullptr),
                    v9_flow4_sdas_col(nullptr),
                    v9_flow6_sdas_col(nullptr),
                    v9_flow4_pnas_col(nullptr),
                    v9_flow6_pnas_col(nullptr),
    {

        
        // Set up placement templates for exporter
        // Metadata export -- FIXME we need a way to make this an options template
        meta_exp = new libfc::PlacementTemplate()
        meta_exp->register_placement(model.lookupIE("messageScope[2]"),
                                                 &cm.messageScope);
        meta_exp->register_placement(model.lookupIE("exporterTransportPort[2]"),
                                                 &cm.exporterTransportPort);
        meta_exp->register_placement(model.lookupIE("exporterIPv4Address[4]"),
                                                 &cm.exporterIPv4Address);
        meta_exp->register_placement(model.lookupIE("systemInitTimeMilliseconds[8]"),
                                                 &cm.systemInitTimeMilliseconds);

        // IPv4 record export with absolute timestamps
        flow4_exp = new libfc::PlacementTemplate()
        flow4_exp->register_placement(model.lookupIE("packetDeltaCount"),
                                                  &cf.packetDeltaCount);
        flow4_exp->register_placement(model.lookupIE("octetDeltaCount"),
                                                  &cf.octetDeltaCount);
        flow4_exp->register_placement(model.lookupIE("flowStartMilliseconds"),
                                                  &cf.flowStartMilliseconds);
        flow4_exp->register_placement(model.lookupIE("flowEndMilliseconds"),
                                                  &cf.flowEndMilliseconds);
        flow4_exp->register_placement(model.lookupIE("sourceIPv4Address"),
                                                  &cf.sourceIPv4Address);
        flow4_exp->register_placement(model.lookupIE("destinationIPv4Address"),
                                                  &cf.destinationIPv4Address);
        flow4_exp->register_placement(model.lookupIE("nextHopIPv4Address"),
                                                  &cf.nextHopIPv4Address);
        flow4_exp->register_placement(model.lookupIE("ingressInterface"),
                                                  &cf.ingressInterface);
        flow4_exp->register_placement(model.lookupIE("egressInterface"),
                                                  &cf.egressInterface);
        flow4_exp->register_placement(model.lookupIE("bgpSourceAsNumber"),
                                                  &cf.bgpSourceAsNumber);
        flow4_exp->register_placement(model.lookupIE("bgpDestinationAsNumber"),
                                                  &cf.bgpDestinationAsNumber);
        flow4_exp->register_placement(model.lookupIE("sourceTransportPort"),
                                                  &cf.sourceTransportPort);
        flow4_exp->register_placement(model.lookupIE("destinationTransportPort"),
                                                  &cf.destinationTransportPort);
        flow4_exp->register_placement(model.lookupIE("tcpControlBits"),
                                                  &cf.tcpControlBits);
        flow4_exp->register_placement(model.lookupIE("protocolIdentifier"),
                                                  &cf.protocolIdentifier);
        flow4_exp->register_placement(model.lookupIE("ipClassOfService"),
                                                  &cf.ipClassOfService);
        flow4_exp->register_placement(model.lookupIE("sourceIPv4PrefixLength"),
                                                  &cf.sourcePrefixLength);
        flow4_exp->register_placement(model.lookupIE("destinationIPv4PrefixLength"),
                                                  &cf.destinationPrefixLength);
        flow4_exp->register_placement(model.lookupIE("flowDirection"),
                                                  &cf.flowDirection);

        // IPv6 record export with absolute timestamps
        flow6_exp = new libfc::PlacementTemplate()
        flow6_exp->register_placement(model.lookupIE("packetDeltaCount"),
                                      &cf.packetDeltaCount);
        flow6_exp->register_placement(model.lookupIE("octetDeltaCount"),
                                      &cf.octetDeltaCount);
        flow6_exp->register_placement(model.lookupIE("flowStartMilliseconds"),
                                      &cf.flowStartMilliseconds);
        flow6_exp->register_placement(model.lookupIE("flowEndMilliseconds"),
                                      &cf.flowEndMilliseconds);
        flow6_exp->register_placement(model.lookupIE("sourceIPv6Address"),
                                      &cf.sourceIPv6Address);
        flow6_exp->register_placement(model.lookupIE("destinationIPv6Address"),
                                      &cf.destinationIPv6Address);
        flow6_exp->register_placement(model.lookupIE("nextHopIPv6Address"),
                                      &cf.nextHopIPv6Address);
        flow6_exp->register_placement(model.lookupIE("ingressInterface"),
                                      &cf.ingressInterface);
        flow6_exp->register_placement(model.lookupIE("egressInterface"),
                                      &cf.egressInterface);
        flow6_exp->register_placement(model.lookupIE("bgpSourceAsNumber"),
                                      &cf.bgpSourceAsNumber);
        flow6_exp->register_placement(model.lookupIE("bgpDestinationAsNumber"),
                                      &cf.bgpDestinationAsNumber);
        flow6_exp->register_placement(model.lookupIE("sourceTransportPort"),
                                      &cf.sourceTransportPort);
        flow6_exp->register_placement(model.lookupIE("destinationTransportPort"),
                                      &cf.destinationTransportPort);
        flow6_exp->register_placement(model.lookupIE("tcpControlBits"),
                                      &cf.tcpControlBits);
        flow6_exp->register_placement(model.lookupIE("protocolIdentifier"),
                                      &cf.protocolIdentifier);
        flow6_exp->register_placement(model.lookupIE("ipClassOfService"),
                                      &cf.ipClassOfService);
        flow6_exp->register_placement(model.lookupIE("sourceIPv4PrefixLength"),
                                      &cf.sourcePrefixLength);
        flow6_exp->register_placement(model.lookupIE("destinationIPv4PrefixLength"),
                                      &cf.destinationPrefixLength);
        flow6_exp->register_placement(model.lookupIE("flowDirection"),
                                      &cf.flowDirection);

        // Set up placement templates for collector
        // NetFlow V5
        v5_flow4_col = new libfc::PlacementTemplate()
        v5_flow4_col->register_placement(model.lookupIE("packetDeltaCount"),
                                         &cf.packetDeltaCount);
        v5_flow4_col->register_placement(model.lookupIE("octetDeltaCount"),
                                         &cf.octetDeltaCount);
        v5_flow4_col->register_placement(model.lookupIE("flowStartSysUpTime"),
                                         &cf.flowStartSysUpTime);
        v5_flow4_col->register_placement(model.lookupIE("flowEndSysUpTime"),
                                         &cf.flowEndSysUpTime);
        v5_flow4_col->register_placement(model.lookupIE("sourceIPv4Address"),
                                         &cf.sourceIPv4Address);
        v5_flow4_col->register_placement(model.lookupIE("destinationIPv4Address"),
                                         &cf.destinationIPv4Address);
        v5_flow4_col->register_placement(model.lookupIE("nextHopIPv4Address"),
                                         &cf.nextHopIPv4Address);
        v5_flow4_col->register_placement(model.lookupIE("ingressInterface"),
                                         &cf.ingressInterface);
        v5_flow4_col->register_placement(model.lookupIE("egressInterface"),
                                         &cf.egressInterface);
        v5_flow4_col->register_placement(model.lookupIE("bgpSourceAsNumber"),
                                         &cf.bgpSourceAsNumber);
        v5_flow4_col->register_placement(model.lookupIE("bgpDestinationAsNumber"),
                                         &cf.bgpDestinationAsNumber);
        v5_flow4_col->register_placement(model.lookupIE("sourceTransportPort"),
                                         &cf.sourceTransportPort);
        v5_flow4_col->register_placement(model.lookupIE("destinationTransportPort"),
                                         &cf.destinationTransportPort);
        v5_flow4_col->register_placement(model.lookupIE("tcpControlBits"),
                                         &cf.tcpControlBits);
        v5_flow4_col->register_placement(model.lookupIE("protocolIdentifier"),
                                         &cf.protocolIdentifier);
        v5_flow4_col->register_placement(model.lookupIE("ipClassOfService"),
                                         &cf.ipClassOfService);
        v5_flow4_col->register_placement(model.lookupIE("sourceIPv4PrefixLength"),
                                         &cf.sourcePrefixLength);
        v5_flow4_col->register_placement(model.lookupIE("destinationIPv4PrefixLength"),
                                         &cf.destinationPrefixLength);

        // NetFlow V9, IPv4, source/destination AS
        v9_flow4_sdas_col = new libfc::PlacementTemplate()
        v9_flow4_sdas_col->register_placement(model.lookupIE("packetDeltaCount"),
                                              &cf.packetDeltaCount);
        v9_flow4_sdas_col->register_placement(model.lookupIE("octetDeltaCount"),
                                              &cf.octetDeltaCount);
        v9_flow4_sdas_col->register_placement(model.lookupIE("flowStartSysUpTime"),
                                              &cf.flowStartSysUpTime);
        v9_flow4_sdas_col->register_placement(model.lookupIE("flowEndSysUpTime"),
                                              &cf.flowEndSysUpTime);
        v9_flow4_sdas_col->register_placement(model.lookupIE("sourceIPv4Address"),
                                              &cf.sourceIPv4Address);
        v9_flow4_sdas_col->register_placement(model.lookupIE("destinationIPv4Address"),
                                              &cf.destinationIPv4Address);
        v9_flow4_sdas_col->register_placement(model.lookupIE("nextHopIPv4Address"),
                                              &cf.nextHopIPv4Address);
        v9_flow4_sdas_col->register_placement(model.lookupIE("ingressInterface"),
                                              &cf.ingressInterface);
        v9_flow4_sdas_col->register_placement(model.lookupIE("egressInterface"),
                                              &cf.egressInterface);
        v9_flow4_sdas_col->register_placement(model.lookupIE("bgpSourceAsNumber"),
                                              &cf.bgpSourceAsNumber);
        v9_flow4_sdas_col->register_placement(model.lookupIE("bgpDestinationAsNumber"),
                                              &cf.bgpDestinationAsNumber);
        v9_flow4_sdas_col->register_placement(model.lookupIE("sourceTransportPort"),
                                              &cf.sourceTransportPort);
        v9_flow4_sdas_col->register_placement(model.lookupIE("destinationTransportPort"),
                                              &cf.destinationTransportPort);
        v9_flow4_sdas_col->register_placement(model.lookupIE("tcpControlBits"),
                                              &cf.tcpControlBits);
        v9_flow4_sdas_col->register_placement(model.lookupIE("protocolIdentifier"),
                                              &cf.protocolIdentifier);
        v9_flow4_sdas_col->register_placement(model.lookupIE("ipClassOfService"),
                                              &cf.ipClassOfService);
        v9_flow4_sdas_col->register_placement(model.lookupIE("sourceIPv4PrefixLength"),
                                              &cf.sourcePrefixLength);
        v9_flow4_sdas_col->register_placement(model.lookupIE("destinationIPv4PrefixLength"),
                                              &cf.destinationPrefixLength);
        
        // NetFlow V9, IPv6, source/destination AS
        v9_flow6_sdas_col = new libfc::PlacementTemplate()
        v9_flow6_sdas_col->register_placement(model.lookupIE("packetDeltaCount"),
                                              &cf.packetDeltaCount);
        v9_flow6_sdas_col->register_placement(model.lookupIE("octetDeltaCount"),
                                              &cf.octetDeltaCount);
        v9_flow6_sdas_col->register_placement(model.lookupIE("flowStartSysUpTime"),
                                              &cf.flowStartSysUpTime);
        v9_flow6_sdas_col->register_placement(model.lookupIE("flowEndSysUpTime"),
                                              &cf.flowEndSysUpTime);
        v9_flow6_sdas_col->register_placement(model.lookupIE("sourceIPv6Address"),
                                              &cf.sourceIPv6Address);
        v9_flow6_sdas_col->register_placement(model.lookupIE("destinationIPv6Address"),
                                              &cf.destinationIPv6Address);
        v9_flow6_sdas_col->register_placement(model.lookupIE("nextHopIPv6Address"),
                                              &cf.nextHopIPv6Address);
        v9_flow6_sdas_col->register_placement(model.lookupIE("ingressInterface"),
                                              &cf.ingressInterface);
        v9_flow6_sdas_col->register_placement(model.lookupIE("egressInterface"),
                                              &cf.egressInterface);
        v9_flow6_sdas_col->register_placement(model.lookupIE("bgpSourceAsNumber"),
                                              &cf.bgpSourceAsNumber);
        v9_flow6_sdas_col->register_placement(model.lookupIE("bgpDestinationAsNumber"),
                                              &cf.bgpDestinationAsNumber);
        v9_flow6_sdas_col->register_placement(model.lookupIE("sourceTransportPort"),
                                              &cf.sourceTransportPort);
        v9_flow6_sdas_col->register_placement(model.lookupIE("destinationTransportPort"),
                                              &cf.destinationTransportPort);
        v9_flow6_sdas_col->register_placement(model.lookupIE("tcpControlBits"),
                                              &cf.tcpControlBits);
        v9_flow6_sdas_col->register_placement(model.lookupIE("protocolIdentifier"),
                                              &cf.protocolIdentifier);
        v9_flow6_sdas_col->register_placement(model.lookupIE("ipClassOfService"),
                                              &cf.ipClassOfService);
        v9_flow6_sdas_col->register_placement(model.lookupIE("sourceIPv6PrefixLength"),
                                              &cf.sourcePrefixLength);
        v9_flow6_sdas_col->register_placement(model.lookupIE("destinationIPv6PrefixLength"),
                                              &cf.destinationPrefixLength);

        // NetFlow V9, IPv4, prev/next AS
        v9_flow4_pnas_col = new libfc::PlacementTemplate()
        v9_flow4_pnas_col->register_placement(model.lookupIE("packetDeltaCount"),
                                              &cf.packetDeltaCount);
        v9_flow4_pnas_col->register_placement(model.lookupIE("octetDeltaCount"),
                                              &cf.octetDeltaCount);
        v9_flow4_pnas_col->register_placement(model.lookupIE("flowStartSysUpTime"),
                                              &cf.flowStartSysUpTime);
        v9_flow4_pnas_col->register_placement(model.lookupIE("flowEndSysUpTime"),
                                              &cf.flowEndSysUpTime);
        v9_flow4_pnas_col->register_placement(model.lookupIE("sourceIPv4Address"),
                                              &cf.sourceIPv4Address);
        v9_flow4_pnas_col->register_placement(model.lookupIE("destinationIPv4Address"),
                                              &cf.destinationIPv4Address);
        v9_flow4_pnas_col->register_placement(model.lookupIE("nextHopIPv4Address"),
                                              &cf.nextHopIPv4Address);
        v9_flow4_pnas_col->register_placement(model.lookupIE("ingressInterface"),
                                              &cf.ingressInterface);
        v9_flow4_pnas_col->register_placement(model.lookupIE("egressInterface"),
                                              &cf.egressInterface);
        v9_flow4_pnas_col->register_placement(model.lookupIE("bgpPrevAdjacentAsNumber"),
                                              &cf.bgpPrevAdjacentAsNumber);
        v9_flow4_pnas_col->register_placement(model.lookupIE("bgpNextAdjacentAsNumber"),
                                              &cf.bgpNextAdjacentAsNumber);
        v9_flow4_pnas_col->register_placement(model.lookupIE("sourceTransportPort"),
                                              &cf.sourceTransportPort);
        v9_flow4_pnas_col->register_placement(model.lookupIE("destinationTransportPort"),
                                              &cf.destinationTransportPort);
        v9_flow4_pnas_col->register_placement(model.lookupIE("tcpControlBits"),
                                              &cf.tcpControlBits);
        v9_flow4_pnas_col->register_placement(model.lookupIE("protocolIdentifier"),
                                              &cf.protocolIdentifier);
        v9_flow4_pnas_col->register_placement(model.lookupIE("ipClassOfService"),
                                              &cf.ipClassOfService);
        v9_flow4_pnas_col->register_placement(model.lookupIE("sourceIPv4PrefixLength"),
                                              &cf.sourcePrefixLength);
        v9_flow4_pnas_col->register_placement(model.lookupIE("destinationIPv4PrefixLength"),
                                              &cf.destinationPrefixLength);

        // NetFlow V9, IPv4, prev/next AS
        v9_flow6_pnas_col = new libfc::PlacementTemplate()
        v9_flow6_pnas_col->register_placement(model.lookupIE("packetDeltaCount"),
                                              &cf.packetDeltaCount);
        v9_flow6_pnas_col->register_placement(model.lookupIE("octetDeltaCount"),
                                              &cf.octetDeltaCount);
        v9_flow6_pnas_col->register_placement(model.lookupIE("flowStartSysUpTime"),
                                              &cf.flowStartSysUpTime);
        v9_flow6_pnas_col->register_placement(model.lookupIE("flowEndSysUpTime"),
                                              &cf.flowEndSysUpTime);
        v9_flow6_pnas_col->register_placement(model.lookupIE("sourceIPv6Address"),
                                              &cf.sourceIPv6Address);
        v9_flow6_pnas_col->register_placement(model.lookupIE("destinationIPv6Address"),
                                              &cf.destinationIPv6Address);
        v9_flow6_pnas_col->register_placement(model.lookupIE("nextHopIPv6Address"),
                                              &cf.nextHopIPv6Address);
        v9_flow6_pnas_col->register_placement(model.lookupIE("ingressInterface"),
                                              &cf.ingressInterface);
        v9_flow6_pnas_col->register_placement(model.lookupIE("egressInterface"),
                                              &cf.egressInterface);
        v9_flow6_pnas_col->register_placement(model.lookupIE("bgpPrevAdjacentAsNumber"),
                                              &cf.bgpPrevAdjacentAsNumber);
        v9_flow6_pnas_col->register_placement(model.lookupIE("bgpNextAdjacentAsNumber"),
                                              &cf.bgpNextAdjacentAsNumber);
        v9_flow6_pnas_col->register_placement(model.lookupIE("sourceTransportPort"),
                                              &cf.sourceTransportPort);
        v9_flow6_pnas_col->register_placement(model.lookupIE("destinationTransportPort"),
                                              &cf.destinationTransportPort);
        v9_flow6_pnas_col->register_placement(model.lookupIE("tcpControlBits"),
                                              &cf.tcpControlBits);
        v9_flow6_pnas_col->register_placement(model.lookupIE("protocolIdentifier"),
                                              &cf.protocolIdentifier);
        v9_flow6_pnas_col->register_placement(model.lookupIE("ipClassOfService"),
                                              &cf.ipClassOfService);
        v9_flow6_pnas_col->register_placement(model.lookupIE("sourceIPv6PrefixLength"),
                                              &cf.sourcePrefixLength);
        v9_flow6_pnas_col->register_placement(model.lookupIE("destinationIPv6PrefixLength"),
                                              &cf.destinationPrefixLength);
        
        // now the exporter
        exporter = new libfc::PlacementExporter()
                                                
        // FIXME dump export templates
                    
    }

    SwitchImp::~SwitchImp()
    {
        if (exporter) delete exporter;
        if (meta_exp) delete meta_exp;
        if (flow4_exp) delete flow4_exp;
        if (flow6_exp) delete flow6_exp;
        if (v5_flow4_col) delete v5_flow4_col;
        if (v9_flow4_sdas_col) delete v9_flow4_sdas_col;
        if (v9_flow6_sdas_col) delete v9_flow6_sdas_col;
        if (v9_flow4_pnas_col) delete v9_flow4_pnas_col;
        if (v9_flow6_pnas_col) delete v9_flow6_pnas_col;
        
    }

    
    std::shared_ptr<libfc::ErrorContext>
                    start_message()
    {
        // FIXME When a message starts, get information from its header
        cm.messageScope = 0;
        cm.exporterIPv4Address = 0;
        cm.exporterTransportPort = 0;
        cm.basetimeCorrectionFactor = 0; // FIXME calculate and export

        // FIXME set the observation domain
        
        
        // and write a new metadata record
        exporter->place_values(meta_exp);
    }

    std::shared_ptr<libfc::ErrorContext>
                    end_message()
    {
        // FIXME When a message ends, flush the exporter.
        exporter.flush();
    }

    
    std::shared_ptr<libfc::ErrorContext> SwitchImp::start_placement(
                               const libfc::PlacementTemplate* tmpl)
    {
        /* Zero the flow record */
        memset(&cf, 0, sizeof(cf));
    }
    
    std::shared_ptr<libfc::ErrorContext> SwitchImp::end_placement(
                             const libfc::PlacementTemplate* ctmpl)
    {
        libfc::PlacementTemplate *etmpl;
        
        // Determine whether we're dealing with an IPv4 or IPv6 flow
        if (tmpl == v9_flow6_sdas_col || tmpl == v9_flow6_pnas_col) {
            etmpl = flow6_exp;
        } else {
            etmpl = flow4_exp;
        }
        
        // (Naively) convert timestamp to absolute
        // FIXME add inline basetime correction
        cf.flowStartMilliseconds = cm.systemInitTimeMilliseconds +
                                   cf.flowStartSysUpTime;
        cf.flowEndMilliseconds =   cm.systemInitTimeMilliseconds +
                                   cf.flowEndSysUpTime;
        
        // FIXME other sanity checking code goes here
        
        // Now re-export the flow.
        exporter->place_values(etmpl);

        
    }
    
    
}