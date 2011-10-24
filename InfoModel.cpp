#include "InfoModel.h"
#include "Constants.h"

#include <sstream>
#include <boost/lexical_cast.hpp>

namespace IPFIX {

const InfoElement InfoModel::parseIESpec(const std::string& iespec) const {
  char            ignore, nc;
  std::stringbuf  ignoresb, namesb, sizesb, ietnamesb;
  std::string     name;
  unsigned int    number = 0, pen = 0, size = 0;

  // parse IE specifier on delimiters
  std::istringstream   iestream(iespec);

  while (!iestream.eof()) {
    if (iestream.peek() == '(') {         // parse number
      iestream >> ignore;
      iestream >> number;
      if (iestream.peek() == '/') {       // number has a pen
        pen = number;
        iestream >> ignore;
        iestream >> number;
      }
      iestream >> ignore; // eat the trailing )
    } else if (iestream.peek() == '<') {  // parse typename
      iestream >> ignore;
      iestream.get(ietnamesb, '>');
      iestream >> ignore; // eat the trailing >
    } else if (iestream.peek() == '[') {  // parse size
        iestream >> ignore;
        iestream.get(sizesb, ']');
        iestream >> ignore; // eat the trailing ]
        if (sizesb.str()[0] == 'v') {
          size = kVarlen;
        } else {
          // parse size
          try {
            size = boost::lexical_cast<uint16_t>( sizesb.str() );
          } catch (boost::bad_lexical_cast &e) {
            throw IESpecError("Bad size in IESpec");
          }
        }
    } else if (iestream.peek() == '{') {  // parse (and ignore) context
      iestream >> ignore;
      iestream.get(ignoresb, '}');
      iestream >> ignore;
    } else { // in name state, add to name
      nc = iestream.get();
      if (!iestream.eof()) {
        namesb.sputc(nc);
      }
    }
  }
  
  const IEType *ietype = lookupIEType(ietnamesb.str());
  InfoElement ie(namesb.str(), pen, number, ietype, size);
  return ie;
}

const InfoElement InfoModel::parseIESpec(const char* iespec) const {
  return parseIESpec(std::string(iespec));
}

void InfoModel::add(const InfoElement& ie) {
  // Short circuit unless we have a record valid for insertion:
  // at least a name, number, and valid, known type
  if (!ie.name().size() || !ie.number() || ie.ietype() == IEType::unknown()) {
    throw IESpecError("Incomplete IESpec for InfoModel addition");
  }

  if (ie.pen()) {
    name_registry_[ie.name()] = 
      pen_registry_[ie.pen()][ie.number()] = 
      std::tr1::shared_ptr<InfoElement>(new InfoElement(ie));
    //std::cerr << "add  PEN IE " << ie.pen() << "/" << ie.number() << " " << ie.name() << std::endl;
  } else {
    name_registry_[ie.name()] = 
      iana_registry_[ie.number()] = 
      std::tr1::shared_ptr<InfoElement>(new InfoElement(ie));
    //std::cerr << "add IANA IE " << ie.number() << " " << ie.name() << std::endl;
  }
}

void InfoModel::add(const std::string& iespec) {
  add(parseIESpec(iespec));
}
  
const InfoElement *InfoModel::lookupIE(uint32_t pen, uint16_t number, uint16_t len) const {  
  std::map<uint16_t, std::tr1::shared_ptr<InfoElement> >::const_iterator iter;

  std::cerr << "lookupIE (" << pen << "/" << number << ")[" << len << "]" << std::endl;
  if (pen) {
    std::map<uint32_t, std::map<uint16_t, std::tr1::shared_ptr<InfoElement> > >::const_iterator peniter;

    if ((peniter = pen_registry_.find(pen)) == pen_registry_.end()) {
      std::cerr << "    no such pen" << std::endl;
      return NULL;
    } else {
      if ((iter = peniter->second.find(number)) == peniter->second.end()) {
        std::cerr << "    not in pen registry" << std::endl;
        return NULL;
      }
    }
  } else {
    if ((iter = iana_registry_.find(number)) == iana_registry_.end()) {
      std::cerr << "    not in iana registry" << std::endl;
      return NULL;
    }
  }
    
  return iter->second->forLen(len);
}

const InfoElement *InfoModel::lookupIE(const InfoElement& specie) const {
  
  if (specie.number()) {
    return lookupIE(specie.pen(), specie.number(), specie.len());
  } else if (specie.name().empty()) {
    // Nothing to look up.
    throw IESpecError("Incomplete IESpec for InfoModel lookup.");
  } else {
    std::cerr << "lookupIE " << specie.name() << std::endl;
    std::map<std::string, std::tr1::shared_ptr<InfoElement> >::const_iterator iter = name_registry_.find(specie.name());
    if (iter == name_registry_.end()) {
      std::cerr << "    not in name registry" << std::endl;
      return NULL;
    } else {
      return iter->second->forLen(specie.len());
    }
  }
}

const InfoElement *InfoModel::lookupIE(const std::string& iespec) const {
  // Parse the Information Element and look it up
  return lookupIE(parseIESpec(iespec));
}    

const InfoElement *InfoModel::lookupIE(const char* iespec) const {
  // Parse the Information Element and look it up
  return lookupIE(parseIESpec(iespec));
}    


void InfoModel::dump(std::ostream &os) const {
  
  std::map<uint16_t, std::tr1::shared_ptr<InfoElement> >::const_iterator   iana_keyiter;

  for (iana_keyiter = iana_registry_.begin();
       iana_keyiter != iana_registry_.end();
       iana_keyiter++) {
      os << iana_keyiter->second->toIESpec() << std::endl;
  }
}

void InfoModel::registerIEType(const IEType *iet) {
  ietypes_bynum_[iet->number()] = iet;
  ietypes_byname_[iet->name()] = iet;
}

void InfoModel::initTypes() {
    ietypes_bynum_.resize(IEType::kIETypeCount);
    registerIEType(IEType::octetArray());
    registerIEType(IEType::unsigned8());
    registerIEType(IEType::unsigned16());
    registerIEType(IEType::unsigned32());
    registerIEType(IEType::unsigned64());
    registerIEType(IEType::signed8());
    registerIEType(IEType::signed16());
    registerIEType(IEType::signed32());
    registerIEType(IEType::signed64());
    registerIEType(IEType::float32());
    registerIEType(IEType::float64());
    registerIEType(IEType::boolean());
    registerIEType(IEType::macAddress());
    registerIEType(IEType::string());
    registerIEType(IEType::dateTimeSeconds());
    registerIEType(IEType::dateTimeMilliseconds());
    registerIEType(IEType::dateTimeMicroseconds());
    registerIEType(IEType::dateTimeNanoseconds()); 
    registerIEType(IEType::ipv4Address());
    registerIEType(IEType::ipv6Address());
}

void InfoModel::defaultV9() {
  add(std::string("octetDeltaCount(1)<unsigned64>[8]"));
  add(std::string("packetDeltaCount(2)<unsigned64>[8]"));
  add(std::string("protocolIdentifier(4)<unsigned8>[1]"));
  add(std::string("ipClassOfService(5)<unsigned8>[1]"));
  add(std::string("tcpControlBits(6)<unsigned8>[1]"));
  add(std::string("sourceTransportPort(7)<unsigned16>[2]"));
  add(std::string("sourceIPv4Address(8)<ipv4Address>[4]"));
  add(std::string("sourceIPv4PrefixLength(9)<unsigned8>[1]"));
  add(std::string("ingressInterface(10)<unsigned32>[4]"));
  add(std::string("destinationTransportPort(11)<unsigned16>[2]"));
  add(std::string("destinationIPv4Address(12)<ipv4Address>[4]"));
  add(std::string("destinationIPv4PrefixLength(13)<unsigned8>[1]"));
  add(std::string("egressInterface(14)<unsigned32>[4]"));
  add(std::string("ipNextHopIPv4Address(15)<ipv4Address>[4]"));
  add(std::string("bgpSourceAsNumber(16)<unsigned32>[4]"));
  add(std::string("bgpDestinationAsNumber(17)<unsigned32>[4]"));
  add(std::string("bgpNextHopIPv4Address(18)<ipv4Address>[4]"));
  add(std::string("postMCastPacketDeltaCount(19)<unsigned64>[8]"));
  add(std::string("postMCastOctetDeltaCount(20)<unsigned64>[8]"));
  add(std::string("flowEndSysUpTime(21)<unsigned32>[4]"));
  add(std::string("flowStartSysUpTime(22)<unsigned32>[4]"));
  add(std::string("postOctetDeltaCount(23)<unsigned64>[8]"));
  add(std::string("postPacketDeltaCount(24)<unsigned64>[8]"));
  add(std::string("minimumIpTotalLength(25)<unsigned64>[8]"));
  add(std::string("maximumIpTotalLength(26)<unsigned64>[8]"));
  add(std::string("sourceIPv6Address(27)<ipv6Address>[16]"));
  add(std::string("destinationIPv6Address(28)<ipv6Address>[16]"));
  add(std::string("sourceIPv6PrefixLength(29)<unsigned8>[1]"));
  add(std::string("destinationIPv6PrefixLength(30)<unsigned8>[1]"));
  add(std::string("flowLabelIPv6(31)<unsigned32>[4]"));
  add(std::string("icmpTypeCodeIPv4(32)<unsigned16>[2]"));
  add(std::string("igmpType(33)<unsigned8>[1]"));
  add(std::string("flowActiveTimeout(36)<unsigned16>[2]"));
  add(std::string("flowIdleTimeout(37)<unsigned16>[2]"));
  add(std::string("exportedOctetTotalCount(40)<unsigned64>[8]"));
  add(std::string("exportedMessageTotalCount(41)<unsigned64>[8]"));
  add(std::string("exportedFlowRecordTotalCount(42)<unsigned64>[8]"));
  add(std::string("sourceIPv4Prefix(44)<ipv4Address>[4]"));
  add(std::string("destinationIPv4Prefix(45)<ipv4Address>[4]"));
  add(std::string("mplsTopLabelType(46)<unsigned8>[1]"));
  add(std::string("mplsTopLabelIPv4Address(47)<ipv4Address>[4]"));
  add(std::string("minimumTTL(52)<unsigned8>[1]"));
  add(std::string("maximumTTL(53)<unsigned8>[1]"));
  add(std::string("fragmentIdentification(54)<unsigned32>[4]"));
  add(std::string("postIpClassOfService(55)<unsigned8>[1]"));
  add(std::string("sourceMacAddress(56)<macAddress>[6]"));
  add(std::string("postDestinationMacAddress(57)<macAddress>[6]"));
  add(std::string("vlanId(58)<unsigned16>[2]"));
  add(std::string("postVlanId(59)<unsigned16>[2]"));
  add(std::string("ipVersion(60)<unsigned8>[1]"));
  add(std::string("flowDirection(61)<unsigned8>[1]"));
  add(std::string("ipNextHopIPv6Address(62)<ipv6Address>[16]"));
  add(std::string("bgpNextHopIPv6Address(63)<ipv6Address>[16]"));
  add(std::string("ipv6ExtensionHeaders(64)<unsigned32>[4]"));
  add(std::string("mplsTopLabelStackSection(70)<octetArray>[3]"));
  add(std::string("mplsLabelStackSection2(71)<octetArray>[3]"));
  add(std::string("mplsLabelStackSection3(72)<octetArray>[3]"));
  add(std::string("mplsLabelStackSection4(73)<octetArray>[3]"));
  add(std::string("mplsLabelStackSection5(74)<octetArray>[3]"));
  add(std::string("mplsLabelStackSection6(75)<octetArray>[3]"));
  add(std::string("mplsLabelStackSection7(76)<octetArray>[3]"));
  add(std::string("mplsLabelStackSection8(77)<octetArray>[3]"));
  add(std::string("mplsLabelStackSection9(78)<octetArray>[3]"));
  add(std::string("mplsLabelStackSection10(79)<octetArray>[3]"));
  add(std::string("destinationMacAddress(80)<macAddress>[6]"));
  add(std::string("postSourceMacAddress(81)<macAddress>[6]"));
  add(std::string("interfaceName(82)<string>[65535]"));
  add(std::string("interfaceDescription(83)<string>[65535]"));
  add(std::string("octetTotalCount(85)<unsigned64>[8]"));
  add(std::string("packetTotalCount(86)<unsigned64>[8]"));
  add(std::string("fragmentOffset(88)<unsigned16>[2]"));
  add(std::string("mplsVpnRouteDistinguisher(90)<octetArray>[8]"));
  add(std::string("mplsTopLabelPrefixLength(91)<unsigned8>[1]"));
  add(std::string("postIpDiffServCodePoint(98)<unsigned8>[1]"));
}

void InfoModel::defaultIPFIX() {
  defaultV9();
  add(std::string("bgpNextAdjacentAsNumber(128)<unsigned32>[4]"));
  add(std::string("bgpPrevAdjacentAsNumber(129)<unsigned32>[4]"));
  add(std::string("exporterIPv4Address(130)<ipv4Address>[4]"));
  add(std::string("exporterIPv6Address(131)<ipv6Address>[16]"));
  add(std::string("droppedOctetDeltaCount(132)<unsigned64>[8]"));
  add(std::string("droppedPacketDeltaCount(133)<unsigned64>[8]"));
  add(std::string("droppedOctetTotalCount(134)<unsigned64>[8]"));
  add(std::string("droppedPacketTotalCount(135)<unsigned64>[8]"));
  add(std::string("flowEndReason(136)<unsigned8>[1]"));
  add(std::string("commonPropertiesId(137)<unsigned64>[8]"));
  add(std::string("observationPointId(138)<unsigned32>[4]"));
  add(std::string("icmpTypeCodeIPv6(139)<unsigned16>[2]"));
  add(std::string("mplsTopLabelIPv6Address(140)<ipv6Address>[16]"));
  add(std::string("lineCardId(141)<unsigned32>[4]"));
  add(std::string("portId(142)<unsigned32>[4]"));
  add(std::string("meteringProcessId(143)<unsigned32>[4]"));
  add(std::string("exportingProcessId(144)<unsigned32>[4]"));
  add(std::string("templateId(145)<unsigned16>[2]"));
  add(std::string("wlanChannelId(146)<unsigned8>[1]"));
  add(std::string("wlanSSID(147)<string>[32]"));
  add(std::string("flowId(148)<unsigned64>[8]"));
  add(std::string("observationDomainId(149)<unsigned32>[4]"));
  add(std::string("flowStartSeconds(150)<dateTimeSeconds>[4]"));
  add(std::string("flowEndSeconds(151)<dateTimeSeconds>[4]"));
  add(std::string("flowStartMilliseconds(152)<dateTimeMilliseconds>[8]"));
  add(std::string("flowEndMilliseconds(153)<dateTimeMilliseconds>[8]"));
  add(std::string("flowStartMicroseconds(154)<dateTimeMicroseconds>[8]"));
  add(std::string("flowEndMicroseconds(155)<dateTimeMicroseconds>[8]"));
  add(std::string("flowStartNanoseconds(156)<dateTimeNanoseconds>[8]"));
  add(std::string("flowEndNanoseconds(157)<dateTimeNanoseconds>[8]"));
  add(std::string("flowStartDeltaMicroseconds(158)<unsigned32>[4]"));
  add(std::string("flowEndDeltaMicroseconds(159)<unsigned32>[4]"));
  add(std::string("systemInitTimeMilliseconds(160)<dateTimeMilliseconds>[8]"));
  add(std::string("flowDurationMilliseconds(161)<unsigned32>[4]"));
  add(std::string("flowDurationMicroseconds(162)<unsigned32>[4]"));
  add(std::string("observedFlowTotalCount(163)<unsigned64>[8]"));
  add(std::string("ignoredPacketTotalCount(164)<unsigned64>[8]"));
  add(std::string("ignoredOctetTotalCount(165)<unsigned64>[8]"));
  add(std::string("notSentFlowTotalCount(166)<unsigned64>[8]"));
  add(std::string("notSentPacketTotalCount(167)<unsigned64>[8]"));
  add(std::string("notSentOctetTotalCount(168)<unsigned64>[8]"));
  add(std::string("destinationIPv6Prefix(169)<ipv6Address>[16]"));
  add(std::string("sourceIPv6Prefix(170)<ipv6Address>[16]"));
  add(std::string("postOctetTotalCount(171)<unsigned64>[8]"));
  add(std::string("postPacketTotalCount(172)<unsigned64>[8]"));
  add(std::string("flowKeyIndicator(173)<unsigned64>[8]"));
  add(std::string("postMCastPacketTotalCount(174)<unsigned64>[8]"));
  add(std::string("postMCastOctetTotalCount(175)<unsigned64>[8]"));
  add(std::string("icmpTypeIPv4(176)<unsigned8>[1]"));
  add(std::string("icmpCodeIPv4(177)<unsigned8>[1]"));
  add(std::string("icmpTypeIPv6(178)<unsigned8>[1]"));
  add(std::string("icmpCodeIPv6(179)<unsigned8>[1]"));
  add(std::string("udpSourcePort(180)<unsigned16>[2]"));
  add(std::string("udpDestinationPort(181)<unsigned16>[2]"));
  add(std::string("tcpSourcePort(182)<unsigned16>[2]"));
  add(std::string("tcpDestinationPort(183)<unsigned16>[2]"));
  add(std::string("tcpSequenceNumber(184)<unsigned32>[4]"));
  add(std::string("tcpAcknowledgementNumber(185)<unsigned32>[4]"));
  add(std::string("tcpWindowSize(186)<unsigned16>[2]"));
  add(std::string("tcpUrgentPointer(187)<unsigned16>[2]"));
  add(std::string("tcpHeaderLength(188)<unsigned8>[1]"));
  add(std::string("ipHeaderLength(189)<unsigned8>[1]"));
  add(std::string("totalLengthIPv4(190)<unsigned16>[2]"));
  add(std::string("payloadLengthIPv6(191)<unsigned16>[2]"));
  add(std::string("ipTTL(192)<unsigned8>[1]"));
  add(std::string("nextHeaderIPv6(193)<unsigned8>[1]"));
  add(std::string("mplsPayloadLength(194)<unsigned32>[4]"));
  add(std::string("ipDiffServCodePoint(195)<unsigned8>[1]"));
  add(std::string("ipPrecedence(196)<unsigned8>[1]"));
  add(std::string("fragmentFlags(197)<unsigned8>[1]"));
  add(std::string("octetDeltaSumOfSquares(198)<unsigned64>[8]"));
  add(std::string("octetTotalSumOfSquares(199)<unsigned64>[8]"));
  add(std::string("mplsTopLabelTTL(200)<unsigned8>[1]"));
  add(std::string("mplsLabelStackLength(201)<unsigned32>[4]"));
  add(std::string("mplsLabelStackDepth(202)<unsigned32>[4]"));
  add(std::string("mplsTopLabelExp(203)<unsigned8>[1]"));
  add(std::string("ipPayloadLength(204)<unsigned32>[4]"));
  add(std::string("udpMessageLength(205)<unsigned16>[2]"));
  add(std::string("isMulticast(206)<unsigned8>[1]"));
  add(std::string("ipv4IHL(207)<unsigned8>[1]"));
  add(std::string("ipv4Options(208)<unsigned32>[4]"));
  add(std::string("tcpOptions(209)<unsigned64>[8]"));
  add(std::string("paddingOctets(210)<octetArray>[1]"));
  add(std::string("collectorIPv4Address(211)<ipv4Address>[4]"));
  add(std::string("collectorIPv6Address(212)<ipv6Address>[16]"));
  add(std::string("exportInterface(213)<unsigned32>[4]"));
  add(std::string("exportProtocolVersion(214)<unsigned8>[1]"));
  add(std::string("exportTransportProtocol(215)<unsigned8>[1]"));
  add(std::string("collectorTransportPort(216)<unsigned16>[2]"));
  add(std::string("exporterTransportPort(217)<unsigned16>[2]"));
  add(std::string("tcpSynTotalCount(218)<unsigned64>[8]"));
  add(std::string("tcpFinTotalCount(219)<unsigned64>[8]"));
  add(std::string("tcpRstTotalCount(220)<unsigned64>[8]"));
  add(std::string("tcpPshTotalCount(221)<unsigned64>[8]"));
  add(std::string("tcpAckTotalCount(222)<unsigned64>[8]"));
  add(std::string("tcpUrgTotalCount(223)<unsigned64>[8]"));
  add(std::string("ipTotalLength(224)<unsigned64>[8]"));
  add(std::string("postNATSourceIPv4Address(225)<ipv4Address>[4]"));
  add(std::string("postNATDestinationIPv4Address(226)<ipv4Address>[4]"));
  add(std::string("postNAPTSourceTransportPort(227)<unsigned16>[2]"));
  add(std::string("postNAPTDestinationTransportPort(228)<unsigned16>[2]"));
  add(std::string("natOriginatingAddressRealm(229)<unsigned8>[1]"));
  add(std::string("natEvent(230)<unsigned8>[1]"));
  add(std::string("initiatorOctets(231)<unsigned64>[8]"));
  add(std::string("responderOctets(232)<unsigned64>[8]"));
  add(std::string("firewallEvent(233)<unsigned8>[1]"));
  add(std::string("ingressVRFID(234)<unsigned32>[4]"));
  add(std::string("egressVRFID(235)<unsigned32>[4]"));
  add(std::string("VRFname(236)<string>[v]"));
  add(std::string("postMplsTopLabelExp(237)<unsigned8>[1]"));
  add(std::string("tcpWindowScale(238)<unsigned16>[2]"));
  add(std::string("biflowDirection(239)<unsigned8>[1]"));
  add(std::string("ethernetHeaderLength(240)<unsigned8>[1]"));
  add(std::string("ethernetPayloadLength(241)<unsigned16>[2]"));
  add(std::string("ethernetTotalLength(242)<unsigned16>[2]"));
  add(std::string("dot1qVlanId(243)<unsigned16>[2]"));
  add(std::string("dot1qPriority(244)<unsigned8>[1]"));
  add(std::string("dot1qCustomerVlanId(245)<unsigned16>[2]"));
  add(std::string("dot1qCustomerPriority(246)<unsigned8>[1]"));
  add(std::string("metroEvcId(247)<string>[100]"));
  add(std::string("metroEvcType(248)<unsigned8>[1]"));
  add(std::string("pseudoWireId(249)<unsigned32>[4]"));
  add(std::string("pseudoWireType(250)<unsigned16>[2]"));
  add(std::string("pseudoWireControlWord(251)<unsigned32>[4]"));
  add(std::string("ingressPhysicalInterface(252)<unsigned32>[4]"));
  add(std::string("egressPhysicalInterface(253)<unsigned32>[4]"));
  add(std::string("postDot1qVlanId(254)<unsigned16>[2]"));
  add(std::string("postDot1qCustomerVlanId(255)<unsigned16>[2]"));
  add(std::string("ethernetType(256)<unsigned16>[2]"));
  add(std::string("postIpPrecedence(257)<unsigned8>[1]"));
  add(std::string("selectionSequenceId(301)<unsigned64>[8]"));
  add(std::string("selectorId(302)<unsigned16>[2]"));
  add(std::string("informationElementId(303)<unsigned16>[2]"));
  add(std::string("selectorAlgorithm(304)<unsigned16>[2]"));
  add(std::string("samplingPacketInterval(305)<unsigned32>[4]"));
  add(std::string("samplingPacketSpace(306)<unsigned32>[4]"));
  add(std::string("samplingTimeInterval(307)<unsigned32>[4]"));
  add(std::string("samplingTimeSpace(308)<unsigned32>[4]"));
  add(std::string("samplingSize(309)<unsigned32>[4]"));
  add(std::string("samplingPopulation(310)<unsigned32>[4]"));
  add(std::string("samplingProbability(311)<float64>[8]"));
  add(std::string("ipHeaderPacketSection(313)<octetArray>[v]"));
  add(std::string("ipPayloadPacketSection(314)<octetArray>[v]"));
  add(std::string("mplsLabelStackSection(316)<octetArray>[v]"));
  add(std::string("mplsPayloadPacketSection(317)<octetArray>[v]"));
  add(std::string("selectorIdTotalPktsObserved(318)<unsigned64>[8]"));
  add(std::string("selectorIdTotalPktsSelected(319)<unsigned64>[8]"));
  add(std::string("absoluteError(320)<float64>[8]"));
  add(std::string("relativeError(321)<float64>[8]"));
  add(std::string("observationTimeSeconds(322)<dateTimeSeconds>[4]"));
  add(std::string("observationTimeMilliseconds(323)<dateTimeMilliseconds>[8]"));
  add(std::string("observationTimeMicroseconds(324)<dateTimeMicroseconds>[8]"));
  add(std::string("observationTimeNanoseconds(325)<dateTimeNanoseconds>[8]"));
  add(std::string("digestHashValue(326)<unsigned64>[8]"));
  add(std::string("hashIPPayloadOffset(327)<unsigned64>[8]"));
  add(std::string("hashIPPayloadSize(328)<unsigned64>[8]"));
  add(std::string("hashOutputRangeMin(329)<unsigned64>[8]"));
  add(std::string("hashOutputRangeMax(330)<unsigned64>[8]"));
  add(std::string("hashSelectedRangeMin(331)<unsigned64>[8]"));
  add(std::string("hashSelectedRangeMax(332)<unsigned64>[8]"));
  add(std::string("hashDigestOutput(333)<boolean>[1]"));
  add(std::string("hashInitialiserValue(334)<unsigned64>[8]"));
  add(std::string("selectorName(335)<string>[v]"));
  add(std::string("upperCILimit(336)<float64>[8]"));
  add(std::string("lowerCILimit(337)<float64>[8]"));
  add(std::string("confidenceLevel(338)<float64>[8]"));
  add(std::string("informationElementDataType(339)<unsigned8>[1]"));
  add(std::string("informationElementDescription(340)<string>[v]"));
  add(std::string("informationElementName(341)<string>[v]"));
  add(std::string("informationElementRangeBegin(342)<unsigned64>[8]"));
  add(std::string("informationElementRangeEnd(343)<unsigned64>[8]"));
  add(std::string("informationElementSemantics(344)<unsigned8>[1]"));
  add(std::string("informationElementUnits(345)<unsigned16>[2]"));
  add(std::string("privateEnterpriseNumber(346)<unsigned32>[4]"));
}

void InfoModel::default5103() {
  defaultIPFIX();
}


}
