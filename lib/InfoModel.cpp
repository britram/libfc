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

 
#include <cassert>
#include <climits>
#include <sstream>
#include <string>

#include "InfoElement.h"
#include "InfoModel.h"
#include "Constants.h"

#include "exceptions/IESpecError.h"

namespace LIBFC {

  InfoModel& InfoModel::instance() {
    static InfoModel instance_;
    return instance_;
  }

  const IEType* InfoModel::lookupIEType(const std::string &name) const {
    std::lock_guard<std::recursive_mutex> locker(lock);

    std::map<std::string, const IEType*>::const_iterator iter;
    
    if ((iter = ietypes_byname_.find(name)) == ietypes_byname_.end()) {
      return NULL;
    } else { 
      return iter->second;
    }
  }

  const IEType* InfoModel::lookupIEType(const unsigned int number) const { 
    std::lock_guard<std::recursive_mutex> locker(lock);

    return ietypes_bynum_.at(number); 
  }

  InfoModel::InfoModel() {
    initTypes();
  }

  static void parseIESpec_NumPen(std::istringstream& iestream, 
				 unsigned int& number,
				 unsigned int& pen) {
    // get the first number
    iestream >> number;
    if (iestream.fail())
      throw IESpecError("badly formatted IE number / PEN");
    
    // see how we're terminated
    char c = iestream.get();
    if (c == '/') {
      // first number was a pen. get the next.
      pen = number;
      iestream >> number;
      if (iestream.fail())
	throw IESpecError("badly formatted IE number");
    } else {
      pen = 0;
      iestream.unget();
    }
  }

  static void parseIESpec_Length(std::istringstream& iestream,
				 unsigned int& len) {
    std::stringbuf lenbuf;
    
    iestream.get(lenbuf, ']');
    
    if (lenbuf.str()[0] == 'v') {
      len = kIpfixVarlen;
    } else {  // parse size
      unsigned long ullen;
      try {
	ullen = std::stoul(lenbuf.str());
      } catch (std::invalid_argument) {
	throw IESpecError("bad size " + lenbuf.str() + " (invalid format)");
      } catch (std::out_of_range) {
	throw IESpecError("bad size " + lenbuf.str() + " (out of range)");
      }

      if (ullen != kIpfixVarlen &&
	  ullen > kIpfixVarlen - kIpfixMessageHeaderLen - kIpfixSetHeaderLen) {
	  throw IESpecError("bad size " + std::to_string(ullen)
			    + " (too large)");
      }
      
      len = static_cast<unsigned int>(ullen);
    }
  }

  static void match(std::istringstream& iestream, char x) {
    char c = iestream.get();
    if (iestream.fail()) {
      std::ostringstream b;
      b << "expected character '" << x << "', but read failed";
      throw IESpecError(b.str());
    } else if (c != x) {
      std::ostringstream b;
      b << "expected character '" << x << "', but got '" << c << "'";
      throw IESpecError(b.str());
    }
  }
    
  static void parseIESpec_Initial(std::istringstream& iestream, 
				  std::stringbuf& namebuf, bool& name_set,
				  std::stringbuf& typebuf, bool& type_set,
				  std::stringbuf& ctxbuf,  bool& ctx_set,
				  unsigned int& number,    bool& number_set,
				  unsigned int& pen,       bool& pen_set,
				  unsigned int& len,       bool& len_set) {
    char c = iestream.get();
    if (iestream.eof()) return;
    
    switch (c) {
    case '(':
      assert(!pen_set || number_set);
      if (number_set)
	throw IESpecError("IESpec contains number / pen more than once");
              
      parseIESpec_NumPen(iestream, number, pen);
      match(iestream, ')');
      number_set = true;
      pen_set = pen != 0;
      break;
    case '[':
      if (len_set)
	throw IESpecError("IESpec contains length more than once");
      parseIESpec_Length(iestream, len);
      match(iestream, ']');
      len_set = true;
      break;
    case '<':
      if (type_set)
	throw IESpecError("IESpec contains type more than once");
      iestream.get(typebuf, '>');
      match(iestream, '>');
      type_set = true;
      break;
    case '{':
      if (ctx_set)
	throw IESpecError("IESpec contains contextf more than once");
      iestream.get(ctxbuf, '}');
      match(iestream, '}');
      ctx_set = true;
      break;
    default:
      if (name_set)
	throw IESpecError("IESpec contains name more than once");
      do {
	namebuf.sputc(c);
	c = iestream.get();
      } while (!iestream.eof() 
	       && c != '(' && c != '{' && c != '<' && c != '[');
      if (!iestream.eof())
	iestream.unget();
      name_set = true;
      break;
    }
  }

  const InfoElement InfoModel::parseIESpec(const std::string& iespec) const {
    std::lock_guard<std::recursive_mutex> locker(lock);
    
    // check for name-only IE
    // WORKAROUND for broken libc++ on Mac OS X Lion
    if ((iespec.find('(') == std::string::npos) &&
        (iespec.find('[') == std::string::npos) &&
        (iespec.find('<') == std::string::npos) &&
        (iespec.find('{') == std::string::npos))
      {
	InfoElement ie(iespec, 0, 0, 0, 0);
	return ie;
      }
    
    std::istringstream iestream(iespec);
    std::stringbuf namebuf, typebuf, ctxbuf;
    unsigned int number = 0, pen = 0, len = 0;
    bool name_set = false;
    bool type_set = false;
    bool ctx_set = false;
    bool number_set = false;
    bool pen_set = false;
    bool len_set = false;
    
    while (!iestream.eof()) {
      parseIESpec_Initial(iestream, 
                          namebuf, name_set,
                          typebuf, type_set,
                          ctxbuf, ctx_set,
                          number, number_set,
                          pen, pen_set,
                          len, len_set);
    }
    
    const IEType *ietype = lookupIEType(typebuf.str());
    InfoElement ie(namebuf.str(), pen, number, ietype, len);
    return ie;
  }

  const InfoElement* InfoModel::add(const InfoElement& ie) {
    std::lock_guard<std::recursive_mutex> locker(lock);

    // Short circuit unless we have a record valid for insertion:
    // at least a name, number, and valid, known type
    if (!ie.name().size() || !ie.number() || ie.ietype() == IEType::unknown()) {
      throw IESpecError("incomplete IESpec for InfoModel addition");
    }
  
    const InfoElement* ret = 0;

    // Only add if we don't have an existing IE for the given name and pen
    if ((ret = lookupIE(ie.pen(), ie.number(), ie.len())) != 0) return ret;


    if (ie.pen()) {
      name_registry_[ie.name()] = 
	pen_registry_[ie.pen()][ie.number()] = 
        std::shared_ptr<InfoElement>(new InfoElement(ie));
      // std::cerr << "add  PEN IE " << ie.pen() << "/" << ie.number() << " " << ie.name() << std::endl;
    } else {
      name_registry_[ie.name()] = 
	iana_registry_[ie.number()] = 
        std::shared_ptr<InfoElement>(new InfoElement(ie));
      // std::cerr << "add IANA IE " << ie.number() << " " << ie.name() << std::endl;
    }

    return name_registry_[ie.name()].get();
  }

  void InfoModel::add(const std::string& iespec) {
    std::lock_guard<std::recursive_mutex> locker(lock);

    add(parseIESpec(iespec));
  }

  const InfoElement* InfoModel::add_unknown(uint32_t pen, uint16_t number, uint16_t len) {
    std::lock_guard<std::recursive_mutex> locker(lock);

    /* Naming convention from Brian's Python code. */
    std::string name = "__ipfix_(" + std::to_string(pen) + 
      ")_(" + std::to_string(number) + ")";

    InfoElement ie(name, pen, number, lookupIEType("octetArray"), len);

    return add(ie);
  }
  
  const InfoElement* InfoModel::lookupIE(uint32_t pen, uint16_t number, uint16_t len) const {  
    std::lock_guard<std::recursive_mutex> locker(lock);

    std::map<uint16_t, std::shared_ptr<InfoElement> >::const_iterator iter;

    //std::cerr << "lookupIE (" << pen << "/" << number << ")[" << len << "]" << std::endl;
    if (pen) {
      std::map<uint32_t, std::map<uint16_t, std::shared_ptr<InfoElement> > >::const_iterator peniter;

      if ((peniter = pen_registry_.find(pen)) == pen_registry_.end()) {
	//std::cerr << "    no such pen" << std::endl;
	return NULL;
      } else {
	if ((iter = peniter->second.find(number)) == peniter->second.end()) {
	  //std::cerr << "    not in pen registry" << std::endl;
	  return NULL;
	}
      }
    } else {
      if ((iter = iana_registry_.find(number)) == iana_registry_.end()) {
	//std::cerr << "    not in iana registry" << std::endl;
	return NULL;
      }
    }
    
    return iter->second->forLen(len);
  }

  const InfoElement *InfoModel::lookupIE(const InfoElement& specie) const {
    std::lock_guard<std::recursive_mutex> locker(lock);

    if (specie.number()) {
      return lookupIE(specie.pen(), specie.number(), specie.len());
    } else if (specie.name().empty()) {
      // Nothing to look up.
      throw IESpecError("incomplete IESpec for InfoModel lookup.");
    } else {
      // std::cerr << "lookupIE " << specie.name() << std::endl;
      std::map<std::string, std::shared_ptr<InfoElement> >::const_iterator iter = name_registry_.find(specie.name());
      if (iter == name_registry_.end()) {
	// std::cerr << "    not in name registry" << std::endl;
	return NULL;
      } else {
	return iter->second->forLen(specie.len());
      }
    }
  }

  const InfoElement *InfoModel::lookupIE(const std::string& iespec) const {
    std::lock_guard<std::recursive_mutex> locker(lock);

    // Parse the Information Element and look it up
    // std::cerr << "lookup " << iespec << " by std::string" << std::endl;
    return lookupIE(parseIESpec(iespec));
  }    

  void InfoModel::dump(std::ostream &os) const {
    std::lock_guard<std::recursive_mutex> locker(lock);

    std::map<uint16_t, std::shared_ptr<InfoElement> >::const_iterator   iana_keyiter;

    for (iana_keyiter = iana_registry_.begin();
	 iana_keyiter != iana_registry_.end();
	 iana_keyiter++) {
      os << iana_keyiter->second->toIESpec() << std::endl;
    }
  }

  void InfoModel::registerIEType(const IEType *iet) {
    std::lock_guard<std::recursive_mutex> locker(lock);

    ietypes_bynum_[iet->number()] = iet;
    ietypes_byname_[iet->name()] = iet;
  }

  void InfoModel::initTypes() {
    std::lock_guard<std::recursive_mutex> locker(lock);

    ietypes_bynum_.resize(IEType::ieTypeCount());
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
    std::lock_guard<std::recursive_mutex> locker(lock);

      add("octetDeltaCount(1)<unsigned64>[8]");
      add("packetDeltaCount(2)<unsigned64>[8]");
      add("deltaFlowCount(3)<unsigned64>[8]");
      add("protocolIdentifier(4)<unsigned8>[1]");
      add("ipClassOfService(5)<unsigned8>[1]");
      add("tcpControlBits(6)<unsigned8>[1]");
      add("sourceTransportPort(7)<unsigned16>[2]");
      add("sourceIPv4Address(8)<ipv4Address>[4]");
      add("sourceIPv4PrefixLength(9)<unsigned8>[1]");
      add("ingressInterface(10)<unsigned32>[4]");
      add("destinationTransportPort(11)<unsigned16>[2]");
      add("destinationIPv4Address(12)<ipv4Address>[4]");
      add("destinationIPv4PrefixLength(13)<unsigned8>[1]");
      add("egressInterface(14)<unsigned32>[4]");
      add("ipNextHopIPv4Address(15)<ipv4Address>[4]");
      add("bgpSourceAsNumber(16)<unsigned32>[4]");
      add("bgpDestinationAsNumber(17)<unsigned32>[4]");
      add("bgpNextHopIPv4Address(18)<ipv4Address>[4]");
      add("postMCastPacketDeltaCount(19)<unsigned64>[8]");
      add("postMCastOctetDeltaCount(20)<unsigned64>[8]");
      add("flowEndSysUpTime(21)<unsigned32>[4]");
      add("flowStartSysUpTime(22)<unsigned32>[4]");
      add("postOctetDeltaCount(23)<unsigned64>[8]");
      add("postPacketDeltaCount(24)<unsigned64>[8]");
      add("minimumIpTotalLength(25)<unsigned64>[8]");
      add("maximumIpTotalLength(26)<unsigned64>[8]");
      add("sourceIPv6Address(27)<ipv6Address>[16]");
      add("destinationIPv6Address(28)<ipv6Address>[16]");
      add("sourceIPv6PrefixLength(29)<unsigned8>[1]");
      add("destinationIPv6PrefixLength(30)<unsigned8>[1]");
      add("flowLabelIPv6(31)<unsigned32>[4]");
      add("icmpTypeCodeIPv4(32)<unsigned16>[2]");
      add("igmpType(33)<unsigned8>[1]");
      add("flowActiveTimeout(36)<unsigned16>[2]");
      add("flowIdleTimeout(37)<unsigned16>[2]");
      add("exportedOctetTotalCount(40)<unsigned64>[8]");
      add("exportedMessageTotalCount(41)<unsigned64>[8]");
      add("exportedFlowRecordTotalCount(42)<unsigned64>[8]");
      add("sourceIPv4Prefix(44)<ipv4Address>[4]");
      add("destinationIPv4Prefix(45)<ipv4Address>[4]");
      add("mplsTopLabelType(46)<unsigned8>[1]");
      add("mplsTopLabelIPv4Address(47)<ipv4Address>[4]");
      add("minimumTTL(52)<unsigned8>[1]");
      add("maximumTTL(53)<unsigned8>[1]");
      add("fragmentIdentification(54)<unsigned32>[4]");
      add("postIpClassOfService(55)<unsigned8>[1]");
      add("sourceMacAddress(56)<macAddress>[6]");
      add("postDestinationMacAddress(57)<macAddress>[6]");
      add("vlanId(58)<unsigned16>[2]");
      add("postVlanId(59)<unsigned16>[2]");
      add("ipVersion(60)<unsigned8>[1]");
      add("flowDirection(61)<unsigned8>[1]");
      add("ipNextHopIPv6Address(62)<ipv6Address>[16]");
      add("bgpNextHopIPv6Address(63)<ipv6Address>[16]");
      add("ipv6ExtensionHeaders(64)<unsigned32>[4]");
      add("mplsTopLabelStackSection(70)<octetArray>[65535]");
      add("mplsLabelStackSection2(71)<octetArray>[65535]");
      add("mplsLabelStackSection3(72)<octetArray>[65535]");
      add("mplsLabelStackSection4(73)<octetArray>[65535]");
      add("mplsLabelStackSection5(74)<octetArray>[65535]");
      add("mplsLabelStackSection6(75)<octetArray>[65535]");
      add("mplsLabelStackSection7(76)<octetArray>[65535]");
      add("mplsLabelStackSection8(77)<octetArray>[65535]");
      add("mplsLabelStackSection9(78)<octetArray>[65535]");
      add("mplsLabelStackSection10(79)<octetArray>[65535]");
      add("destinationMacAddress(80)<macAddress>[6]");
      add("postSourceMacAddress(81)<macAddress>[6]");
      add("interfaceName(82)<string>[65535]");
      add("interfaceDescription(83)<string>[65535]");
      add("octetTotalCount(85)<unsigned64>[8]");
      add("packetTotalCount(86)<unsigned64>[8]");
      add("fragmentOffset(88)<unsigned16>[2]");
      add("mplsVpnRouteDistinguisher(90)<octetArray>[65535]");
      add("mplsTopLabelPrefixLength(91)<unsigned8>[1]");
      add("applicationDescription(94)<string>[65535]");
      add("applicationId(95)<octetArray>[65535]");
      add("applicationName(96)<string>[65535]");
      add("postIpDiffServCodePoint(98)<unsigned8>[1]");
      add("multicastReplicationFactor(99)<unsigned32>[4]");
      add("classificationEngineId(101)<unsigned8>[1]");
  }

  void InfoModel::defaultIPFIX() {
    std::lock_guard<std::recursive_mutex> locker(lock);

    defaultV9();
      add("bgpNextAdjacentAsNumber(128)<unsigned32>[4]");
      add("bgpPrevAdjacentAsNumber(129)<unsigned32>[4]");
      add("exporterIPv4Address(130)<ipv4Address>[4]");
      add("exporterIPv6Address(131)<ipv6Address>[16]");
      add("droppedOctetDeltaCount(132)<unsigned64>[8]");
      add("droppedPacketDeltaCount(133)<unsigned64>[8]");
      add("droppedOctetTotalCount(134)<unsigned64>[8]");
      add("droppedPacketTotalCount(135)<unsigned64>[8]");
      add("flowEndReason(136)<unsigned8>[1]");
      add("commonPropertiesId(137)<unsigned64>[8]");
      add("observationPointId(138)<unsigned64>[8]");
      add("icmpTypeCodeIPv6(139)<unsigned16>[2]");
      add("mplsTopLabelIPv6Address(140)<ipv6Address>[16]");
      add("lineCardId(141)<unsigned32>[4]");
      add("portId(142)<unsigned32>[4]");
      add("meteringProcessId(143)<unsigned32>[4]");
      add("exportingProcessId(144)<unsigned32>[4]");
      add("templateId(145)<unsigned16>[2]");
      add("wlanChannelId(146)<unsigned8>[1]");
      add("wlanSSID(147)<string>[65535]");
      add("flowId(148)<unsigned64>[8]");
      add("observationDomainId(149)<unsigned32>[4]");
      add("flowStartSeconds(150)<dateTimeSeconds>[4]");
      add("flowEndSeconds(151)<dateTimeSeconds>[4]");
      add("flowStartMilliseconds(152)<dateTimeMilliseconds>[8]");
      add("flowEndMilliseconds(153)<dateTimeMilliseconds>[8]");
      add("flowStartMicroseconds(154)<dateTimeMicroseconds>");
      add("flowEndMicroseconds(155)<dateTimeMicroseconds>");
      add("flowStartNanoseconds(156)<dateTimeNanoseconds>");
      add("flowEndNanoseconds(157)<dateTimeNanoseconds>");
      add("flowStartDeltaMicroseconds(158)<unsigned32>[4]");
      add("flowEndDeltaMicroseconds(159)<unsigned32>[4]");
      add("systemInitTimeMilliseconds(160)<dateTimeMilliseconds>[8]");
      add("flowDurationMilliseconds(161)<unsigned32>[4]");
      add("flowDurationMicroseconds(162)<unsigned32>[4]");
      add("observedFlowTotalCount(163)<unsigned64>[8]");
      add("ignoredPacketTotalCount(164)<unsigned64>[8]");
      add("ignoredOctetTotalCount(165)<unsigned64>[8]");
      add("notSentFlowTotalCount(166)<unsigned64>[8]");
      add("notSentPacketTotalCount(167)<unsigned64>[8]");
      add("notSentOctetTotalCount(168)<unsigned64>[8]");
      add("destinationIPv6Prefix(169)<ipv6Address>[16]");
      add("sourceIPv6Prefix(170)<ipv6Address>[16]");
      add("postOctetTotalCount(171)<unsigned64>[8]");
      add("postPacketTotalCount(172)<unsigned64>[8]");
      add("flowKeyIndicator(173)<unsigned64>[8]");
      add("postMCastPacketTotalCount(174)<unsigned64>[8]");
      add("postMCastOctetTotalCount(175)<unsigned64>[8]");
      add("icmpTypeIPv4(176)<unsigned8>[1]");
      add("icmpCodeIPv4(177)<unsigned8>[1]");
      add("icmpTypeIPv6(178)<unsigned8>[1]");
      add("icmpCodeIPv6(179)<unsigned8>[1]");
      add("udpSourcePort(180)<unsigned16>[2]");
      add("udpDestinationPort(181)<unsigned16>[2]");
      add("tcpSourcePort(182)<unsigned16>[2]");
      add("tcpDestinationPort(183)<unsigned16>[2]");
      add("tcpSequenceNumber(184)<unsigned32>[4]");
      add("tcpAcknowledgementNumber(185)<unsigned32>[4]");
      add("tcpWindowSize(186)<unsigned16>[2]");
      add("tcpUrgentPointer(187)<unsigned16>[2]");
      add("tcpHeaderLength(188)<unsigned8>[1]");
      add("ipHeaderLength(189)<unsigned8>[1]");
      add("totalLengthIPv4(190)<unsigned16>[2]");
      add("payloadLengthIPv6(191)<unsigned16>[2]");
      add("ipTTL(192)<unsigned8>[1]");
      add("nextHeaderIPv6(193)<unsigned8>[1]");
      add("mplsPayloadLength(194)<unsigned32>[4]");
      add("ipDiffServCodePoint(195)<unsigned8>[1]");
      add("ipPrecedence(196)<unsigned8>[1]");
      add("fragmentFlags(197)<unsigned8>[1]");
      add("octetDeltaSumOfSquares(198)<unsigned64>[8]");
      add("octetTotalSumOfSquares(199)<unsigned64>[8]");
      add("mplsTopLabelTTL(200)<unsigned8>[1]");
      add("mplsLabelStackLength(201)<unsigned32>[4]");
      add("mplsLabelStackDepth(202)<unsigned32>[4]");
      add("mplsTopLabelExp(203)<unsigned8>[1]");
      add("ipPayloadLength(204)<unsigned32>[4]");
      add("udpMessageLength(205)<unsigned16>[2]");
      add("isMulticast(206)<unsigned8>[1]");
      add("ipv4IHL(207)<unsigned8>[1]");
      add("ipv4Options(208)<unsigned32>[4]");
      add("tcpOptions(209)<unsigned64>[8]");
      add("paddingOctets(210)<octetArray>[65535]");
      add("collectorIPv4Address(211)<ipv4Address>[4]");
      add("collectorIPv6Address(212)<ipv6Address>[16]");
      add("exportInterface(213)<unsigned32>[4]");
      add("exportProtocolVersion(214)<unsigned8>[1]");
      add("exportTransportProtocol(215)<unsigned8>[1]");
      add("collectorTransportPort(216)<unsigned16>[2]");
      add("exporterTransportPort(217)<unsigned16>[2]");
      add("tcpSynTotalCount(218)<unsigned64>[8]");
      add("tcpFinTotalCount(219)<unsigned64>[8]");
      add("tcpRstTotalCount(220)<unsigned64>[8]");
      add("tcpPshTotalCount(221)<unsigned64>[8]");
      add("tcpAckTotalCount(222)<unsigned64>[8]");
      add("tcpUrgTotalCount(223)<unsigned64>[8]");
      add("ipTotalLength(224)<unsigned64>[8]");
      add("postNATSourceIPv4Address(225)<ipv4Address>[4]");
      add("postNATDestinationIPv4Address(226)<ipv4Address>[4]");
      add("postNAPTSourceTransportPort(227)<unsigned16>[2]");
      add("postNAPTDestinationTransportPort(228)<unsigned16>[2]");
      add("natOriginatingAddressRealm(229)<unsigned8>[1]");
      add("natEvent(230)<unsigned8>[1]");
      add("initiatorOctets(231)<unsigned64>[8]");
      add("responderOctets(232)<unsigned64>[8]");
      add("firewallEvent(233)<unsigned8>[1]");
      add("ingressVRFID(234)<unsigned32>[4]");
      add("egressVRFID(235)<unsigned32>[4]");
      add("VRFname(236)<string>[65535]");
      add("postMplsTopLabelExp(237)<unsigned8>[1]");
      add("tcpWindowScale(238)<unsigned16>[2]");
      add("biflowDirection(239)<unsigned8>[1]");
      add("ethernetHeaderLength(240)<unsigned8>[1]");
      add("ethernetPayloadLength(241)<unsigned16>[2]");
      add("ethernetTotalLength(242)<unsigned16>[2]");
      add("dot1qVlanId(243)<unsigned16>[2]");
      add("dot1qPriority(244)<unsigned8>[1]");
      add("dot1qCustomerVlanId(245)<unsigned16>[2]");
      add("dot1qCustomerPriority(246)<unsigned8>[1]");
      add("metroEvcId(247)<string>[65535]");
      add("metroEvcType(248)<unsigned8>[1]");
      add("pseudoWireId(249)<unsigned32>[4]");
      add("pseudoWireType(250)<unsigned16>[2]");
      add("pseudoWireControlWord(251)<unsigned32>[4]");
      add("ingressPhysicalInterface(252)<unsigned32>[4]");
      add("egressPhysicalInterface(253)<unsigned32>[4]");
      add("postDot1qVlanId(254)<unsigned16>[2]");
      add("postDot1qCustomerVlanId(255)<unsigned16>[2]");
      add("ethernetType(256)<unsigned16>[2]");
      add("postIpPrecedence(257)<unsigned8>[1]");
      add("collectionTimeMilliseconds(258)<dateTimeMilliseconds>[8]");
      add("exportSctpStreamId(259)<unsigned16>[2]");
      add("maxExportSeconds(260)<dateTimeSeconds>[4]");
      add("maxFlowEndSeconds(261)<dateTimeSeconds>[4]");
      add("messageMD5Checksum(262)<octetArray>[65535]");
      add("messageScope(263)<unsigned8>[1]");
      add("minExportSeconds(264)<dateTimeSeconds>[4]");
      add("minFlowStartSeconds(265)<dateTimeSeconds>[4]");
      add("opaqueOctets(266)<octetArray>[65535]");
      add("sessionScope(267)<unsigned8>[1]");
      add("maxFlowEndMicroseconds(268)<dateTimeMicroseconds>");
      add("maxFlowEndMilliseconds(269)<dateTimeMilliseconds>[8]");
      add("maxFlowEndNanoseconds(270)<dateTimeNanoseconds>");
      add("minFlowStartMicroseconds(271)<dateTimeMicroseconds>");
      add("minFlowStartMilliseconds(272)<dateTimeMilliseconds>");
      add("minFlowStartNanoseconds(273)<dateTimeNanoseconds>");
      add("collectorCertificate(274)<octetArray>[65535]");
      add("exporterCertificate(275)<octetArray>[65535]");
      add("dataRecordsReliability(276)<boolean>[1]");
      add("observationPointType(277)<unsigned8>[1]");
      add("connectionCountNew(278)<unsigned32>[4]");
      add("connectionSumDuration(279)<unsigned64>[8]");
      add("connectionTransactionId(280)<unsigned64>[8]");
      add("postNATSourceIPv6Address(281)<ipv6Address>[16]");
      add("postNATDestinationIPv6Address(282)<ipv6Address>[16]");
      add("natPoolId(283)<unsigned32>[4]");
      add("natPoolName(284)<string>[65535]");
      add("anonymizationFlags(285)<unsigned16>[2]");
      add("anonymizationTechnique(286)<unsigned16>[2]");
      add("informationElementIndex(287)<unsigned16>[2]");
      add("p2pTechnology(288)<string>[65535]");
      add("tunnelTechnology(289)<string>[65535]");
      add("encryptedTechnology(290)<string>[65535]");
      add("bgpValidityState(294)<unsigned8>[1]");
      add("IPSecSPI(295)<unsigned32>[4]");
      add("greKey(296)<unsigned32>[4]");
      add("natType(297)<unsigned8>[1]");
      add("initiatorPackets(298)<unsigned64>[8]");
      add("responderPackets(299)<unsigned64>[8]");
      add("observationDomainName(300)<string>[65535]");
      add("selectionSequenceId(301)<unsigned64>[8]");
      add("selectorId(302)<unsigned64>[8]");
      add("informationElementId(303)<unsigned16>[2]");
      add("selectorAlgorithm(304)<unsigned16>[2]");
      add("samplingPacketInterval(305)<unsigned32>[4]");
      add("samplingPacketSpace(306)<unsigned32>[4]");
      add("samplingTimeInterval(307)<unsigned32>[4]");
      add("samplingTimeSpace(308)<unsigned32>[4]");
      add("samplingSize(309)<unsigned32>[4]");
      add("samplingPopulation(310)<unsigned32>[4]");
      add("samplingProbability(311)<float64>[8]");
      add("dataLinkFrameSize(312)<unsigned16>[2]");
      add("ipHeaderPacketSection(313)<octetArray>[65535]");
      add("ipPayloadPacketSection(314)<octetArray>[65535]");
      add("dataLinkFrameSection(315)<octetArray>[65535]");
      add("mplsLabelStackSection(316)<octetArray>[65535]");
      add("mplsPayloadPacketSection(317)<octetArray>[65535]");
      add("selectorIdTotalPktsObserved(318)<unsigned64>[8]");
      add("selectorIdTotalPktsSelected(319)<unsigned64>[8]");
      add("absoluteError(320)<float64>[8]");
      add("relativeError(321)<float64>[8]");
      add("observationTimeSeconds(322)<dateTimeSeconds>[4]");
      add("observationTimeMilliseconds(323)<dateTimeMilliseconds>");
      add("observationTimeMicroseconds(324)<dateTimeMicroseconds>");
      add("observationTimeNanoseconds(325)<dateTimeNanoseconds>");
      add("digestHashValue(326)<unsigned64>[8]");
      add("hashIPPayloadOffset(327)<unsigned64>[8]");
      add("hashIPPayloadSize(328)<unsigned64>[8]");
      add("hashOutputRangeMin(329)<unsigned64>[8]");
      add("hashOutputRangeMax(330)<unsigned64>[8]");
      add("hashSelectedRangeMin(331)<unsigned64>[8]");
      add("hashSelectedRangeMax(332)<unsigned64>[8]");
      add("hashDigestOutput(333)<boolean>[1]");
      add("hashInitialiserValue(334)<unsigned64>[8]");
      add("selectorName(335)<string>[65535]");
      add("upperCILimit(336)<float64>[8]");
      add("lowerCILimit(337)<float64>[8]");
      add("confidenceLevel(338)<float64>[8]");
      add("informationElementDataType(339)<unsigned8>[1]");
      add("informationElementDescription(340)<string>[65535]");
      add("informationElementName(341)<string>[65535]");
      add("informationElementRangeBegin(342)<unsigned64>[8]");
      add("informationElementRangeEnd(343)<unsigned64>[8]");
      add("informationElementSemantics(344)<unsigned8>[1]");
      add("informationElementUnits(345)<unsigned16>[2]");
      add("privateEnterpriseNumber(346)<unsigned32>[4]");
      add("virtualStationInterfaceId(347)<octetArray>[65535]");
      add("virtualStationInterfaceName(348)<string>[65535]");
      add("virtualStationUUID(349)<octetArray>[65535]");
      add("virtualStationName(350)<string>[65535]");
      add("layer2SegmentId(351)<unsigned64>[8]");
      add("layer2OctetDeltaCount(352)<unsigned64>[8]");
      add("layer2OctetTotalCount(353)<unsigned64>[8]");
      add("ingressUnicastPacketTotalCount(354)<unsigned64>[8]");
      add("ingressMulticastPacketTotalCount(355)<unsigned64>[8]");
      add("ingressBroadcastPacketTotalCount(356)<unsigned64>[8]");
      add("egressUnicastPacketTotalCount(357)<unsigned64>[8]");
      add("egressBroadcastPacketTotalCount(358)<unsigned64>[8]");
      add("monitoringIntervalStartMilliSeconds(359)<dateTimeMilliseconds>[8]");
      add("monitoringIntervalEndMilliSeconds(360)<dateTimeMilliseconds>[8]");
      add("portRangeStart(361)<unsigned16>[2]");
      add("portRangeEnd(362)<unsigned16>[2]");
      add("portRangeStepSize(363)<unsigned16>[2]");
      add("portRangeNumPorts(364)<unsigned16>[2]");
      add("staMacAddress(365)<macAddress>[6]");
      add("staIPv4Address(366)<ipv4Address>[4]");
      add("wtpMacAddress(367)<macAddress>[6]");
      add("ingressInterfaceType(368)<unsigned32>[4]");
      add("egressInterfaceType(369)<unsigned32>[4]");
      add("rtpSequenceNumber(370)<unsigned16>[2]");
      add("userName(371)<string>[65535]");
      add("applicationCategoryName(372)<string>[65535]");
      add("applicationSubCategoryName(373)<string>[65535]");
      add("applicationGroupName(374)<string>[65535]");
      add("originalFlowsPresent(375)<unsigned64>[8]");
      add("originalFlowsInitiated(376)<unsigned64>[8]");
      add("originalFlowsCompleted(377)<unsigned64>[8]");
      add("distinctCountOfSourceIPAddress(378)<unsigned64>[8]");
      add("distinctCountOfDestinationIPAddress(379)<unsigned64>[8]");
      add("distinctCountOfSourceIPv4Address(380)<unsigned32>[4]");
      add("distinctCountOfDestinationIPv4Address(381)<unsigned32>[4]");
      add("distinctCountOfSourceIPv6Address(382)<unsigned64>[8]");
      add("distinctCountOfDestinationIPv6Address(383)<unsigned64>[8]");
      add("valueDistributionMethod(384)<unsigned8>[1]");
      add("rfc3550JitterMilliseconds(385)<unsigned32>[4]");
      add("rfc3550JitterMicroseconds(386)<unsigned32>[4]");
      add("rfc3550JitterNanoseconds(387)<unsigned32>[4]");
      add("dot1qDEI(388)<boolean>[1]");
      add("dot1qCustomerDEI(389)<boolean>[1]");
      add("flowSelectorAlgorithm(390)<unsigned16>");
      add("flowSelectedOctetDeltaCount(391)<unsigned64>");
      add("flowSelectedPacketDeltaCount(392)<unsigned64>");
      add("flowSelectedFlowDeltaCount(393)<unsigned64>");
      add("selectorIDTotalFlowsObserved(394)<unsigned64>");
      add("selectorIDTotalFlowsSelected(395)<unsigned64>");
      add("samplingFlowInterval(396)<unsigned64>");
      add("samplingFlowSpacing(397)<unsigned64>");
      add("flowSamplingTimeInterval(398)<unsigned64>");
      add("flowSamplingTimeSpacing(399)<unsigned64>");
      add("hashFlowDomain(400)<unsigned16>");
      add("transportOctetDeltaCount(401)<unsigned64>");
      add("transportPacketDeltaCount(402)<unsigned64>");
      add("originalExporterIPv4Address(403)<ipv4Address>");
      add("originalExporterIPv6Address(404)<ipv6Address>");
      add("originalObservationDomainId(405)<unsigned32>");
      add("intermediateProcessId(406)<unsigned32>");
      add("ignoredDataRecordTotalCount(407)<unsigned64>");
      add("dataLinkFrameType(408)<unsigned16>");
      add("sectionOffset(409)<unsigned16>");
      add("sectionExportedOctets(410)<unsigned16>");
      add("dot1qServiceInstanceTag(411)<octetArray>");
      add("dot1qServiceInstanceId(412)<unsigned32>");
      add("dot1qServiceInstancePriority(413)<unsigned8>");
      add("dot1qCustomerSourceMacAddress(414)<macAddress>");
      add("dot1qCustomerDestinationMacAddress(415)<macAddress>");
      add("l2OctetDeltaCount(416)<unsigned64>");
      add("postL2OctetDeltaCount(417)<unsigned64>");
      add("postMCastL2OctetDeltaCount(418)<unsigned64>");
      add("l2OctetTotalCount(419)<unsigned64>");
      add("postL2OctetTotalCount(420)<unsigned64>");
      add("postMCastL2OctetTotalCount(421)<unsigned64>");
      add("minimumL2TotalLength(422)<unsigned64>");
      add("maximumL2TotalLength(423)<unsigned64>");
      add("droppedL2OctetDeltaCount(424)<unsigned64>");
      add("droppedL2OctetTotalCount(425)<unsigned64>");
      add("ignoredL2OctetTotalCount(426)<unsigned64>");
      add("notSentL2OctetTotalCount(427)<unsigned64>");
      add("l2OctetDeltaSumOfSquares(428)<unsigned64>");
      add("l2OctetTotalSumOfSquares(429)<unsigned64>");
}

  void InfoModel::default5103() {
    std::lock_guard<std::recursive_mutex> locker(lock);

    defaultIPFIX();
    add("reverseOctetDeltaCount(29305/1)<unsigned64>[8]");
    add("reversePacketDeltaCount(29305/2)<unsigned64>[8]");
    add("reverseDeltaFlowCount(29305/3)<unsigned64>[8]");
    add("reverseProtocolIdentifier(29305/4)<unsigned8>[1]");
    add("reverseIpClassOfService(29305/5)<unsigned8>[1]");
    add("reverseTcpControlBits(29305/6)<unsigned8>[1]");
    add("reverseSourceTransportPort(29305/7)<unsigned16>[2]");
    add("reverseSourceIPv4Address(29305/8)<ipv4Address>[4]");
    add("reverseSourceIPv4PrefixLength(29305/9)<unsigned8>[1]");
    add("reverseIngressInterface(29305/10)<unsigned32>[4]");
    add("reverseDestinationTransportPort(29305/11)<unsigned16>[2]");
    add("reverseDestinationIPv4Address(29305/12)<ipv4Address>[4]");
    add("reverseDestinationIPv4PrefixLength(29305/13)<unsigned8>[1]");
    add("reverseEgressInterface(29305/14)<unsigned32>[4]");
    add("reverseIpNextHopIPv4Address(29305/15)<ipv4Address>[4]");
    add("reverseBgpSourceAsNumber(29305/16)<unsigned32>[4]");
    add("reverseBgpDestinationAsNumber(29305/17)<unsigned32>[4]");
    add("reverseBgpNextHopIPv4Address(29305/18)<ipv4Address>[4]");
    add("reversePostMCastPacketDeltaCount(29305/19)<unsigned64>[8]");
    add("reversePostMCastOctetDeltaCount(29305/20)<unsigned64>[8]");
    add("reverseFlowEndSysUpTime(29305/21)<unsigned32>[4]");
    add("reverseFlowStartSysUpTime(29305/22)<unsigned32>[4]");
    add("reversePostOctetDeltaCount(29305/23)<unsigned64>[8]");
    add("reversePostPacketDeltaCount(29305/24)<unsigned64>[8]");
    add("reverseMinimumIpTotalLength(29305/25)<unsigned64>[8]");
    add("reverseMaximumIpTotalLength(29305/26)<unsigned64>[8]");
    add("reverseSourceIPv6Address(29305/27)<ipv6Address>[16]");
    add("reverseDestinationIPv6Address(29305/28)<ipv6Address>[16]");
    add("reverseSourceIPv6PrefixLength(29305/29)<unsigned8>[1]");
    add("reverseDestinationIPv6PrefixLength(29305/30)<unsigned8>[1]");
    add("reverseFlowLabelIPv6(29305/31)<unsigned32>[4]");
    add("reverseIcmpTypeCodeIPv4(29305/32)<unsigned16>[2]");
    add("reverseIgmpType(29305/33)<unsigned8>[1]");
    add("reverseFlowActiveTimeout(29305/36)<unsigned16>[2]");
    add("reverseFlowIdleTimeout(29305/37)<unsigned16>[2]");
    add("reverseExportedOctetTotalCount(29305/40)<unsigned64>[8]");
    add("reverseExportedMessageTotalCount(29305/41)<unsigned64>[8]");
    add("reverseExportedFlowRecordTotalCount(29305/42)<unsigned64>[8]");
    add("reverseSourceIPv4Prefix(29305/44)<ipv4Address>[4]");
    add("reverseDestinationIPv4Prefix(29305/45)<ipv4Address>[4]");
    add("reverseMplsTopLabelType(29305/46)<unsigned8>[1]");
    add("reverseMplsTopLabelIPv4Address(29305/47)<ipv4Address>[4]");
    add("reverseMinimumTTL(29305/52)<unsigned8>[1]");
    add("reverseMaximumTTL(29305/53)<unsigned8>[1]");
    add("reverseFragmentIdentification(29305/54)<unsigned32>[4]");
    add("reversePostIpClassOfService(29305/55)<unsigned8>[1]");
    add("reverseSourceMacAddress(29305/56)<macAddress>[6]");
    add("reversePostDestinationMacAddress(29305/57)<macAddress>[6]");
    add("reverseVlanId(29305/58)<unsigned16>[2]");
    add("reversePostVlanId(29305/59)<unsigned16>[2]");
    add("reverseIpVersion(29305/60)<unsigned8>[1]");
    add("reverseFlowDirection(29305/61)<unsigned8>[1]");
    add("reverseIpNextHopIPv6Address(29305/62)<ipv6Address>[16]");
    add("reverseBgpNextHopIPv6Address(29305/63)<ipv6Address>[16]");
    add("reverseIpv6ExtensionHeaders(29305/64)<unsigned32>[4]");
    add("reverseMplsTopLabelStackSection(29305/70)<octetArray>[65535]");
    add("reverseMplsLabelStackSection2(29305/71)<octetArray>[65535]");
    add("reverseMplsLabelStackSection3(29305/72)<octetArray>[65535]");
    add("reverseMplsLabelStackSection4(29305/73)<octetArray>[65535]");
    add("reverseMplsLabelStackSection5(29305/74)<octetArray>[65535]");
    add("reverseMplsLabelStackSection6(29305/75)<octetArray>[65535]");
    add("reverseMplsLabelStackSection7(29305/76)<octetArray>[65535]");
    add("reverseMplsLabelStackSection8(29305/77)<octetArray>[65535]");
    add("reverseMplsLabelStackSection9(29305/78)<octetArray>[65535]");
    add("reverseMplsLabelStackSection10(29305/79)<octetArray>[65535]");
    add("reverseDestinationMacAddress(29305/80)<macAddress>[6]");
    add("reversePostSourceMacAddress(29305/81)<macAddress>[6]");
    add("reverseInterfaceName(29305/82)<string>[65535]");
    add("reverseInterfaceDescription(29305/83)<string>[65535]");
    add("reverseOctetTotalCount(29305/85)<unsigned64>[8]");
    add("reversePacketTotalCount(29305/86)<unsigned64>[8]");
    add("reverseFragmentOffset(29305/88)<unsigned16>[2]");
    add("reverseMplsVpnRouteDistinguisher(29305/90)<octetArray>[65535]");
    add("reverseMplsTopLabelPrefixLength(29305/91)<unsigned8>[1]");
    add("reverseApplicationDescription(29305/94)<string>[65535]");
    add("reverseApplicationId(29305/95)<octetArray>[65535]");
    add("reverseApplicationName(29305/96)<string>[65535]");
    add("reversePostIpDiffServCodePoint(29305/98)<unsigned8>[1]");
    add("reverseMulticastReplicationFactor(29305/99)<unsigned32>[4]");
    add("reverseClassificationEngineId(29305/101)<unsigned8>[1]");
    add("reverseBgpNextAdjacentAsNumber(29305/128)<unsigned32>[4]");
    add("reverseBgpPrevAdjacentAsNumber(29305/129)<unsigned32>[4]");
    add("reverseExporterIPv4Address(29305/130)<ipv4Address>[4]");
    add("reverseExporterIPv6Address(29305/131)<ipv6Address>[16]");
    add("reverseDroppedOctetDeltaCount(29305/132)<unsigned64>[8]");
    add("reverseDroppedPacketDeltaCount(29305/133)<unsigned64>[8]");
    add("reverseDroppedOctetTotalCount(29305/134)<unsigned64>[8]");
    add("reverseDroppedPacketTotalCount(29305/135)<unsigned64>[8]");
    add("reverseFlowEndReason(29305/136)<unsigned8>[1]");
    add("reverseCommonPropertiesId(29305/137)<unsigned64>[8]");
    add("reverseObservationPointId(29305/138)<unsigned64>[8]");
    add("reverseIcmpTypeCodeIPv6(29305/139)<unsigned16>[2]");
    add("reverseMplsTopLabelIPv6Address(29305/140)<ipv6Address>[16]");
    add("reverseLineCardId(29305/141)<unsigned32>[4]");
    add("reversePortId(29305/142)<unsigned32>[4]");
    add("reverseMeteringProcessId(29305/143)<unsigned32>[4]");
    add("reverseExportingProcessId(29305/144)<unsigned32>[4]");
    add("reverseTemplateId(29305/145)<unsigned16>[2]");
    add("reverseWlanChannelId(29305/146)<unsigned8>[1]");
    add("reverseWlanSSID(29305/147)<string>[65535]");
    add("reverseFlowId(29305/148)<unsigned64>[8]");
    add("reverseObservationDomainId(29305/149)<unsigned32>[4]");
    add("reverseFlowStartSeconds(29305/150)<dateTimeSeconds>[4]");
    add("reverseFlowEndSeconds(29305/151)<dateTimeSeconds>[4]");
    add("reverseFlowStartMilliseconds(29305/152)<dateTimeMilliseconds>[8]");
    add("reverseFlowEndMilliseconds(29305/153)<dateTimeMilliseconds>[8]");
    add("reverseFlowStartMicroseconds(29305/154)<dateTimeMicroseconds>");
    add("reverseFlowEndMicroseconds(29305/155)<dateTimeMicroseconds>");
    add("reverseFlowStartNanoseconds(29305/156)<dateTimeNanoseconds>");
    add("reverseFlowEndNanoseconds(29305/157)<dateTimeNanoseconds>");
    add("reverseFlowStartDeltaMicroseconds(29305/158)<unsigned32>[4]");
    add("reverseFlowEndDeltaMicroseconds(29305/159)<unsigned32>[4]");
    add("reverseSystemInitTimeMilliseconds(29305/160)<dateTimeMilliseconds>[8]");
    add("reverseFlowDurationMilliseconds(29305/161)<unsigned32>[4]");
    add("reverseFlowDurationMicroseconds(29305/162)<unsigned32>[4]");
    add("reverseObservedFlowTotalCount(29305/163)<unsigned64>[8]");
    add("reverseIgnoredPacketTotalCount(29305/164)<unsigned64>[8]");
    add("reverseIgnoredOctetTotalCount(29305/165)<unsigned64>[8]");
    add("reverseNotSentFlowTotalCount(29305/166)<unsigned64>[8]");
    add("reverseNotSentPacketTotalCount(29305/167)<unsigned64>[8]");
    add("reverseNotSentOctetTotalCount(29305/168)<unsigned64>[8]");
    add("reverseDestinationIPv6Prefix(29305/169)<ipv6Address>[16]");
    add("reverseSourceIPv6Prefix(29305/170)<ipv6Address>[16]");
    add("reversePostOctetTotalCount(29305/171)<unsigned64>[8]");
    add("reversePostPacketTotalCount(29305/172)<unsigned64>[8]");
    add("reverseFlowKeyIndicator(29305/173)<unsigned64>[8]");
    add("reversePostMCastPacketTotalCount(29305/174)<unsigned64>[8]");
    add("reversePostMCastOctetTotalCount(29305/175)<unsigned64>[8]");
    add("reverseIcmpTypeIPv4(29305/176)<unsigned8>[1]");
    add("reverseIcmpCodeIPv4(29305/177)<unsigned8>[1]");
    add("reverseIcmpTypeIPv6(29305/178)<unsigned8>[1]");
    add("reverseIcmpCodeIPv6(29305/179)<unsigned8>[1]");
    add("reverseUdpSourcePort(29305/180)<unsigned16>[2]");
    add("reverseUdpDestinationPort(29305/181)<unsigned16>[2]");
    add("reverseTcpSourcePort(29305/182)<unsigned16>[2]");
    add("reverseTcpDestinationPort(29305/183)<unsigned16>[2]");
    add("reverseTcpSequenceNumber(29305/184)<unsigned32>[4]");
    add("reverseTcpAcknowledgementNumber(29305/185)<unsigned32>[4]");
    add("reverseTcpWindowSize(29305/186)<unsigned16>[2]");
    add("reverseTcpUrgentPointer(29305/187)<unsigned16>[2]");
    add("reverseTcpHeaderLength(29305/188)<unsigned8>[1]");
    add("reverseIpHeaderLength(29305/189)<unsigned8>[1]");
    add("reverseTotalLengthIPv4(29305/190)<unsigned16>[2]");
    add("reversePayloadLengthIPv6(29305/191)<unsigned16>[2]");
    add("reverseIpTTL(29305/192)<unsigned8>[1]");
    add("reverseNextHeaderIPv6(29305/193)<unsigned8>[1]");
    add("reverseMplsPayloadLength(29305/194)<unsigned32>[4]");
    add("reverseIpDiffServCodePoint(29305/195)<unsigned8>[1]");
    add("reverseIpPrecedence(29305/196)<unsigned8>[1]");
    add("reverseFragmentFlags(29305/197)<unsigned8>[1]");
    add("reverseOctetDeltaSumOfSquares(29305/198)<unsigned64>[8]");
    add("reverseOctetTotalSumOfSquares(29305/199)<unsigned64>[8]");
    add("reverseMplsTopLabelTTL(29305/200)<unsigned8>[1]");
    add("reverseMplsLabelStackLength(29305/201)<unsigned32>[4]");
    add("reverseMplsLabelStackDepth(29305/202)<unsigned32>[4]");
    add("reverseMplsTopLabelExp(29305/203)<unsigned8>[1]");
    add("reverseIpPayloadLength(29305/204)<unsigned32>[4]");
    add("reverseUdpMessageLength(29305/205)<unsigned16>[2]");
    add("reverseIsMulticast(29305/206)<unsigned8>[1]");
    add("reverseIpv4IHL(29305/207)<unsigned8>[1]");
    add("reverseIpv4Options(29305/208)<unsigned32>[4]");
    add("reverseTcpOptions(29305/209)<unsigned64>[8]");
    add("reversePaddingOctets(29305/210)<octetArray>[65535]");
    add("reverseCollectorIPv4Address(29305/211)<ipv4Address>[4]");
    add("reverseCollectorIPv6Address(29305/212)<ipv6Address>[16]");
    add("reverseExportInterface(29305/213)<unsigned32>[4]");
    add("reverseExportProtocolVersion(29305/214)<unsigned8>[1]");
    add("reverseExportTransportProtocol(29305/215)<unsigned8>[1]");
    add("reverseCollectorTransportPort(29305/216)<unsigned16>[2]");
    add("reverseExporterTransportPort(29305/217)<unsigned16>[2]");
    add("reverseTcpSynTotalCount(29305/218)<unsigned64>[8]");
    add("reverseTcpFinTotalCount(29305/219)<unsigned64>[8]");
    add("reverseTcpRstTotalCount(29305/220)<unsigned64>[8]");
    add("reverseTcpPshTotalCount(29305/221)<unsigned64>[8]");
    add("reverseTcpAckTotalCount(29305/222)<unsigned64>[8]");
    add("reverseTcpUrgTotalCount(29305/223)<unsigned64>[8]");
    add("reverseIpTotalLength(29305/224)<unsigned64>[8]");
    add("reversePostNATSourceIPv4Address(29305/225)<ipv4Address>[4]");
    add("reversePostNATDestinationIPv4Address(29305/226)<ipv4Address>[4]");
    add("reversePostNAPTSourceTransportPort(29305/227)<unsigned16>[2]");
    add("reversePostNAPTDestinationTransportPort(29305/228)<unsigned16>[2]");
    add("reverseNatOriginatingAddressRealm(29305/229)<unsigned8>[1]");
    add("reverseNatEvent(29305/230)<unsigned8>[1]");
    add("reverseInitiatorOctets(29305/231)<unsigned64>[8]");
    add("reverseResponderOctets(29305/232)<unsigned64>[8]");
    add("reverseFirewallEvent(29305/233)<unsigned8>[1]");
    add("reverseIngressVRFID(29305/234)<unsigned32>[4]");
    add("reverseEgressVRFID(29305/235)<unsigned32>[4]");
    add("reverseVRFname(29305/236)<string>[65535]");
    add("reversePostMplsTopLabelExp(29305/237)<unsigned8>[1]");
    add("reverseTcpWindowScale(29305/238)<unsigned16>[2]");
    add("reverseBiflowDirection(29305/239)<unsigned8>[1]");
    add("reverseEthernetHeaderLength(29305/240)<unsigned8>[1]");
    add("reverseEthernetPayloadLength(29305/241)<unsigned16>[2]");
    add("reverseEthernetTotalLength(29305/242)<unsigned16>[2]");
    add("reverseDot1qVlanId(29305/243)<unsigned16>[2]");
    add("reverseDot1qPriority(29305/244)<unsigned8>[1]");
    add("reverseDot1qCustomerVlanId(29305/245)<unsigned16>[2]");
    add("reverseDot1qCustomerPriority(29305/246)<unsigned8>[1]");
    add("reverseMetroEvcId(29305/247)<string>[65535]");
    add("reverseMetroEvcType(29305/248)<unsigned8>[1]");
    add("reversePseudoWireId(29305/249)<unsigned32>[4]");
    add("reversePseudoWireType(29305/250)<unsigned16>[2]");
    add("reversePseudoWireControlWord(29305/251)<unsigned32>[4]");
    add("reverseIngressPhysicalInterface(29305/252)<unsigned32>[4]");
    add("reverseEgressPhysicalInterface(29305/253)<unsigned32>[4]");
    add("reversePostDot1qVlanId(29305/254)<unsigned16>[2]");
    add("reversePostDot1qCustomerVlanId(29305/255)<unsigned16>[2]");
    add("reverseEthernetType(29305/256)<unsigned16>[2]");
    add("reversePostIpPrecedence(29305/257)<unsigned8>[1]");
    add("reverseCollectionTimeMilliseconds(29305/258)<dateTimeMilliseconds>[8]");
    add("reverseExportSctpStreamId(29305/259)<unsigned16>[2]");
    add("reverseMaxExportSeconds(29305/260)<dateTimeSeconds>[4]");
    add("reverseMaxFlowEndSeconds(29305/261)<dateTimeSeconds>[4]");
    add("reverseMessageMD5Checksum(29305/262)<octetArray>[65535]");
    add("reverseMessageScope(29305/263)<unsigned8>[1]");
    add("reverseMinExportSeconds(29305/264)<dateTimeSeconds>[4]");
    add("reverseMinFlowStartSeconds(29305/265)<dateTimeSeconds>[4]");
    add("reverseOpaqueOctets(29305/266)<octetArray>[65535]");
    add("reverseSessionScope(29305/267)<unsigned8>[1]");
    add("reverseMaxFlowEndMicroseconds(29305/268)<dateTimeMicroseconds>");
    add("reverseMaxFlowEndMilliseconds(29305/269)<dateTimeMilliseconds>");
    add("reverseMaxFlowEndNanoseconds(29305/270)<dateTimeNanoseconds>");
    add("reverseMinFlowStartMicroseconds(29305/271)<dateTimeMicroseconds>");
    add("reverseMinFlowStartMilliseconds(29305/272)<dateTimeMilliseconds>");
    add("reverseMinFlowStartNanoseconds(29305/273)<dateTimeNanoseconds>");
    add("reverseCollectorCertificate(29305/274)<octetArray>[65535]");
    add("reverseExporterCertificate(29305/275)<octetArray>[65535]");
    add("reverseDataRecordsReliability(29305/276)<boolean>[1]");
    add("reverseObservationPointType(29305/277)<unsigned8>[1]");
    add("reverseConnectionCountNew(29305/278)<unsigned32>[4]");
    add("reverseConnectionSumDuration(29305/279)<unsigned64>[8]");
    add("reverseConnectionTransactionId(29305/280)<unsigned64>[8]");
    add("reversePostNATSourceIPv6Address(29305/281)<ipv6Address>[16]");
    add("reversePostNATDestinationIPv6Address(29305/282)<ipv6Address>[16]");
    add("reverseNatPoolId(29305/283)<unsigned32>[4]");
    add("reverseNatPoolName(29305/284)<string>[65535]");
    add("reverseAnonymizationFlags(29305/285)<unsigned16>[2]");
    add("reverseAnonymizationTechnique(29305/286)<unsigned16>[2]");
    add("reverseInformationElementIndex(29305/287)<unsigned16>[2]");
    add("reverseP2PTechnology(29305/288)<string>[65535]");
    add("reverseTunnelTechnology(29305/289)<string>[65535]");
    add("reverseEncryptedTechnology(29305/290)<string>[65535]");
    add("reverseBgpValidityState(29305/294)<unsigned8>[1]");
    add("reverseIPSecSPI(29305/295)<unsigned32>[4]");
    add("reverseGreKey(29305/296)<unsigned32>[4]");
    add("reverseNatType(29305/297)<unsigned8>[1]");
    add("reverseInitiatorPackets(29305/298)<unsigned64>[8]");
    add("reverseResponderPackets(29305/299)<unsigned64>[8]");
    add("reverseObservationDomainName(29305/300)<string>[65535]");
    add("reverseSelectionSequenceId(29305/301)<unsigned64>[8]");
    add("reverseSelectorId(29305/302)<unsigned64>[8]");
    add("reverseInformationElementId(29305/303)<unsigned16>[2]");
    add("reverseSelectorAlgorithm(29305/304)<unsigned16>[2]");
    add("reverseSamplingPacketInterval(29305/305)<unsigned32>[4]");
    add("reverseSamplingPacketSpace(29305/306)<unsigned32>[4]");
    add("reverseSamplingTimeInterval(29305/307)<unsigned32>[4]");
    add("reverseSamplingTimeSpace(29305/308)<unsigned32>[4]");
    add("reverseSamplingSize(29305/309)<unsigned32>[4]");
    add("reverseSamplingPopulation(29305/310)<unsigned32>[4]");
    add("reverseSamplingProbability(29305/311)<float64>[8]");
    add("reverseDataLinkFrameSize(29305/312)<unsigned16>[2]");
    add("reverseIpHeaderPacketSection(29305/313)<octetArray>[65535]");
    add("reverseIpPayloadPacketSection(29305/314)<octetArray>[65535]");
    add("reverseDataLinkFrameSection(29305/315)<octetArray>[65535]");
    add("reverseMplsLabelStackSection(29305/316)<octetArray>[65535]");
    add("reverseMplsPayloadPacketSection(29305/317)<octetArray>[65535]");
    add("reverseSelectorIdTotalPktsObserved(29305/318)<unsigned64>[8]");
    add("reverseSelectorIdTotalPktsSelected(29305/319)<unsigned64>[8]");
    add("reverseAbsoluteError(29305/320)<float64>[8]");
    add("reverseRelativeError(29305/321)<float64>[8]");
    add("reverseObservationTimeSeconds(29305/322)<dateTimeSeconds>");
    add("reverseObservationTimeMilliseconds(29305/323)<dateTimeMilliseconds>");
    add("reverseObservationTimeMicroseconds(29305/324)<dateTimeMicroseconds>");
    add("reverseObservationTimeNanoseconds(29305/325)<dateTimeNanoseconds>");
    add("reverseDigestHashValue(29305/326)<unsigned64>[8]");
    add("reverseHashIPPayloadOffset(29305/327)<unsigned64>[8]");
    add("reverseHashIPPayloadSize(29305/328)<unsigned64>[8]");
    add("reverseHashOutputRangeMin(29305/329)<unsigned64>[8]");
    add("reverseHashOutputRangeMax(29305/330)<unsigned64>[8]");
    add("reverseHashSelectedRangeMin(29305/331)<unsigned64>[8]");
    add("reverseHashSelectedRangeMax(29305/332)<unsigned64>[8]");
    add("reverseHashDigestOutput(29305/333)<boolean>[1]");
    add("reverseHashInitialiserValue(29305/334)<unsigned64>[8]");
    add("reverseSelectorName(29305/335)<string>[65535]");
    add("reverseUpperCILimit(29305/336)<float64>[8]");
    add("reverseLowerCILimit(29305/337)<float64>[8]");
    add("reverseConfidenceLevel(29305/338)<float64>[8]");
    add("reverseInformationElementDataType(29305/339)<unsigned8>[1]");
    add("reverseInformationElementDescription(29305/340)<string>[65535]");
    add("reverseInformationElementName(29305/341)<string>[65535]");
    add("reverseInformationElementRangeBegin(29305/342)<unsigned64>[8]");
    add("reverseInformationElementRangeEnd(29305/343)<unsigned64>[8]");
    add("reverseInformationElementSemantics(29305/344)<unsigned8>[1]");
    add("reverseInformationElementUnits(29305/345)<unsigned16>[2]");
    add("reversePrivateEnterpriseNumber(29305/346)<unsigned32>[4]");
    add("reverseVirtualStationInterfaceId(29305/347)<octetArray>[65535]");
    add("reverseVirtualStationInterfaceName(29305/348)<string>[65535]");
    add("reverseVirtualStationUUID(29305/349)<octetArray>[65535]");
    add("reverseVirtualStationName(29305/350)<string>[65535]");
    add("reverseLayer2SegmentId(29305/351)<unsigned64>[8]");
    add("reverseLayer2OctetDeltaCount(29305/352)<unsigned64>[8]");
    add("reverseLayer2OctetTotalCount(29305/353)<unsigned64>[8]");
    add("reverseIngressUnicastPacketTotalCount(29305/354)<unsigned64>[8]");
    add("reverseIngressMulticastPacketTotalCount(29305/355)<unsigned64>[8]");
    add("reverseIngressBroadcastPacketTotalCount(29305/356)<unsigned64>[8]");
    add("reverseEgressUnicastPacketTotalCount(29305/357)<unsigned64>[8]");
    add("reverseEgressBroadcastPacketTotalCount(29305/358)<unsigned64>[8]");
    add("reverseMonitoringIntervalStartMilliSeconds(29305/359)<dateTimeMilliseconds>[8]");
    add("reverseMonitoringIntervalEndMilliSeconds(29305/360)<dateTimeMilliseconds>[8]");
    add("reversePortRangeStart(29305/361)<unsigned16>[2]");
    add("reversePortRangeEnd(29305/362)<unsigned16>[2]");
    add("reversePortRangeStepSize(29305/363)<unsigned16>[2]");
    add("reversePortRangeNumPorts(29305/364)<unsigned16>[2]");
    add("reverseStaMacAddress(29305/365)<macAddress>[6]");
    add("reverseStaIPv4Address(29305/366)<ipv4Address>[4]");
    add("reverseWtpMacAddress(29305/367)<macAddress>[6]");
    add("reverseIngressInterfaceType(29305/368)<unsigned32>[4]");
    add("reverseEgressInterfaceType(29305/369)<unsigned32>[4]");
    add("reverseRtpSequenceNumber(29305/370)<unsigned16>[2]");
    add("reverseUserName(29305/371)<string>[65535]");
    add("reverseApplicationCategoryName(29305/372)<string>[65535]");
    add("reverseApplicationSubCategoryName(29305/373)<string>[65535]");
    add("reverseApplicationGroupName(29305/374)<string>[65535]");
    add("reverseOriginalFlowsPresent(29305/375)<unsigned64>[8]");
    add("reverseOriginalFlowsInitiated(29305/376)<unsigned64>[8]");
    add("reverseOriginalFlowsCompleted(29305/377)<unsigned64>[8]");
    add("reverseDistinctCountOfSourceIPAddress(29305/378)<unsigned64>[8]");
    add("reverseDistinctCountOfDestinationIPAddress(29305/379)<unsigned64>[8]");
    add("reverseDistinctCountOfSourceIPv4Address(29305/380)<unsigned32>[4]");
    add("reverseDistinctCountOfDestinationIPv4Address(29305/381)<unsigned32>[4]");
    add("reverseDistinctCountOfSourceIPv6Address(29305/382)<unsigned64>[8]");
    add("reverseDistinctCountOfDestinationIPv6Address(29305/383)<unsigned64>[8]");
    add("reverseValueDistributionMethod(29305/384)<unsigned8>[1]");
    add("reverseRfc3550JitterMilliseconds(29305/385)<unsigned32>[4]");
    add("reverseRfc3550JitterMicroseconds(29305/386)<unsigned32>[4]");
    add("reverseRfc3550JitterNanoseconds(29305/387)<unsigned32>[4]");
    add("reverseDot1qDEI(29305/388)<boolean>[1]");
    add("reverseDot1qCustomerDEI(29305/389)<boolean>[1]");
    add("reverseFlowSelectorAlgorithm(29305/390)<unsigned16>");
    add("reverseFlowSelectedOctetDeltaCount(29305/391)<unsigned64>");
    add("reverseFlowSelectedPacketDeltaCount(29305/392)<unsigned64>");
    add("reverseFlowSelectedFlowDeltaCount(29305/393)<unsigned64>");
    add("reverseSelectorIDTotalFlowsObserved(29305/394)<unsigned64>");
    add("reverseSelectorIDTotalFlowsSelected(29305/395)<unsigned64>");
    add("reverseSamplingFlowInterval(29305/396)<unsigned64>");
    add("reverseSamplingFlowSpacing(29305/397)<unsigned64>");
    add("reverseFlowSamplingTimeInterval(29305/398)<unsigned64>");
    add("reverseFlowSamplingTimeSpacing(29305/399)<unsigned64>");
    add("reverseHashFlowDomain(29305/400)<unsigned16>");
    add("reverseTransportOctetDeltaCount(29305/401)<unsigned64>");
    add("reverseTransportPacketDeltaCount(29305/402)<unsigned64>");
    add("reverseOriginalExporterIPv4Address(29305/403)<ipv4Address>");
    add("reverseOriginalExporterIPv6Address(29305/404)<ipv6Address>");
    add("reverseOriginalObservationDomainId(29305/405)<unsigned32>");
    add("reverseIntermediateProcessId(29305/406)<unsigned32>");
    add("reverseIgnoredDataRecordTotalCount(29305/407)<unsigned64>");
    add("reverseDataLinkFrameType(29305/408)<unsigned16>");
    add("reverseSectionOffset(29305/409)<unsigned16>");
    add("reverseSectionExportedOctets(29305/410)<unsigned16>");
    add("reverseDot1qServiceInstanceTag(29305/411)<octetArray>");
    add("reverseDot1qServiceInstanceId(29305/412)<unsigned32>");
    add("reverseDot1qServiceInstancePriority(29305/413)<unsigned8>");
    add("reverseDot1qCustomerSourceMacAddress(29305/414)<macAddress>");
    add("reverseDot1qCustomerDestinationMacAddress(29305/415)<macAddress>");
    add("reverseL2OctetDeltaCount(29305/416)<unsigned64>");
    add("reversePostL2OctetDeltaCount(29305/417)<unsigned64>");
    add("reversePostMCastL2OctetDeltaCount(29305/418)<unsigned64>");
    add("reverseL2OctetTotalCount(29305/419)<unsigned64>");
    add("reversePostL2OctetTotalCount(29305/420)<unsigned64>");
    add("reversePostMCastL2OctetTotalCount(29305/421)<unsigned64>");
    add("reverseMinimumL2TotalLength(29305/422)<unsigned64>");
    add("reverseMaximumL2TotalLength(29305/423)<unsigned64>");
    add("reverseDroppedL2OctetDeltaCount(29305/424)<unsigned64>");
    add("reverseDroppedL2OctetTotalCount(29305/425)<unsigned64>");
    add("reverseIgnoredL2OctetTotalCount(29305/426)<unsigned64>");
    add("reverseNotSentL2OctetTotalCount(29305/427)<unsigned64>");
    add("reverseL2OctetDeltaSumOfSquares(29305/428)<unsigned64>");
    add("reverseL2OctetTotalSumOfSquares(29305/429)<unsigned64>");
}

} /* namespace LIBFC */
