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

namespace IPFIX {

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

    std::string name = "_unknown_" + std::to_string(pen) + 
      "_" + std::to_string(number);

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
    add("mplsTopLabelStackSection(70)<octetArray>[3]");
    add("mplsLabelStackSection2(71)<octetArray>[3]");
    add("mplsLabelStackSection3(72)<octetArray>[3]");
    add("mplsLabelStackSection4(73)<octetArray>[3]");
    add("mplsLabelStackSection5(74)<octetArray>[3]");
    add("mplsLabelStackSection6(75)<octetArray>[3]");
    add("mplsLabelStackSection7(76)<octetArray>[3]");
    add("mplsLabelStackSection8(77)<octetArray>[3]");
    add("mplsLabelStackSection9(78)<octetArray>[3]");
    add("mplsLabelStackSection10(79)<octetArray>[3]");
    add("destinationMacAddress(80)<macAddress>[6]");
    add("postSourceMacAddress(81)<macAddress>[6]");
    add("interfaceName(82)<string>[65535]");
    add("interfaceDescription(83)<string>[65535]");
    add("octetTotalCount(85)<unsigned64>[8]");
    add("packetTotalCount(86)<unsigned64>[8]");
    add("fragmentOffset(88)<unsigned16>[2]");
    add("mplsVpnRouteDistinguisher(90)<octetArray>[8]");
    add("mplsTopLabelPrefixLength(91)<unsigned8>[1]");
    add("postIpDiffServCodePoint(98)<unsigned8>[1]");
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
    add("observationPointId(138)<unsigned32>[4]");
    add("icmpTypeCodeIPv6(139)<unsigned16>[2]");
    add("mplsTopLabelIPv6Address(140)<ipv6Address>[16]");
    add("lineCardId(141)<unsigned32>[4]");
    add("portId(142)<unsigned32>[4]");
    add("meteringProcessId(143)<unsigned32>[4]");
    add("exportingProcessId(144)<unsigned32>[4]");
    add("templateId(145)<unsigned16>[2]");
    add("wlanChannelId(146)<unsigned8>[1]");
    add("wlanSSID(147)<string>[32]");
    add("flowId(148)<unsigned64>[8]");
    add("observationDomainId(149)<unsigned32>[4]");
    add("flowStartSeconds(150)<dateTimeSeconds>[4]");
    add("flowEndSeconds(151)<dateTimeSeconds>[4]");
    add("flowStartMilliseconds(152)<dateTimeMilliseconds>[8]");
    add("flowEndMilliseconds(153)<dateTimeMilliseconds>[8]");
    add("flowStartMicroseconds(154)<dateTimeMicroseconds>[8]");
    add("flowEndMicroseconds(155)<dateTimeMicroseconds>[8]");
    add("flowStartNanoseconds(156)<dateTimeNanoseconds>[8]");
    add("flowEndNanoseconds(157)<dateTimeNanoseconds>[8]");
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
    add("paddingOctets(210)<octetArray>[1]");
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
    add("VRFname(236)<string>[v]");
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
    add("metroEvcId(247)<string>[100]");
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
    add("initiatorPackets(298)<unsigned64>[8]");
    add("responderPackets(299)<unsigned64>[8]");
    add("selectionSequenceId(301)<unsigned64>[8]");
    add("selectorId(302)<unsigned16>[2]");
    add("informationElementId(303)<unsigned16>[2]");
    add("selectorAlgorithm(304)<unsigned16>[2]");
    add("samplingPacketInterval(305)<unsigned32>[4]");
    add("samplingPacketSpace(306)<unsigned32>[4]");
    add("samplingTimeInterval(307)<unsigned32>[4]");
    add("samplingTimeSpace(308)<unsigned32>[4]");
    add("samplingSize(309)<unsigned32>[4]");
    add("samplingPopulation(310)<unsigned32>[4]");
    add("samplingProbability(311)<float64>[8]");
    add("ipHeaderPacketSection(313)<octetArray>[v]");
    add("ipPayloadPacketSection(314)<octetArray>[v]");
    add("mplsLabelStackSection(316)<octetArray>[v]");
    add("mplsPayloadPacketSection(317)<octetArray>[v]");
    add("selectorIdTotalPktsObserved(318)<unsigned64>[8]");
    add("selectorIdTotalPktsSelected(319)<unsigned64>[8]");
    add("absoluteError(320)<float64>[8]");
    add("relativeError(321)<float64>[8]");
    add("observationTimeSeconds(322)<dateTimeSeconds>[4]");
    add("observationTimeMilliseconds(323)<dateTimeMilliseconds>[8]");
    add("observationTimeMicroseconds(324)<dateTimeMicroseconds>[8]");
    add("observationTimeNanoseconds(325)<dateTimeNanoseconds>[8]");
    add("digestHashValue(326)<unsigned64>[8]");
    add("hashIPPayloadOffset(327)<unsigned64>[8]");
    add("hashIPPayloadSize(328)<unsigned64>[8]");
    add("hashOutputRangeMin(329)<unsigned64>[8]");
    add("hashOutputRangeMax(330)<unsigned64>[8]");
    add("hashSelectedRangeMin(331)<unsigned64>[8]");
    add("hashSelectedRangeMax(332)<unsigned64>[8]");
    add("hashDigestOutput(333)<boolean>[1]");
    add("hashInitialiserValue(334)<unsigned64>[8]");
    add("selectorName(335)<string>[v]");
    add("upperCILimit(336)<float64>[8]");
    add("lowerCILimit(337)<float64>[8]");
    add("confidenceLevel(338)<float64>[8]");
    add("informationElementDataType(339)<unsigned8>[1]");
    add("informationElementDescription(340)<string>[v]");
    add("informationElementName(341)<string>[v]");
    add("informationElementRangeBegin(342)<unsigned64>[8]");
    add("informationElementRangeEnd(343)<unsigned64>[8]");
    add("informationElementSemantics(344)<unsigned8>[1]");
    add("informationElementUnits(345)<unsigned16>[2]");
    add("privateEnterpriseNumber(346)<unsigned32>[4]");
  }

  void InfoModel::default5103() {
    std::lock_guard<std::recursive_mutex> locker(lock);

    // FIXME: Add reverses
    defaultIPFIX();
    // FIXME: Partial list -- add rest
    add("reverseOctetDeltaCount(29305/1)<unsigned64>[8]");
    add("reversePacketDeltaCount(29305/2)<unsigned64>[8]");
    add("reverseMinimumTTL(29305/52)<unsigned8>[1]");
    add("reverseMaximumTTL(29305/53)<unsigned8>[1]");
    add("reverseTcpSequenceNumber(29305/184)<unsigned32>[4]");
  }

} /* namespace IPFIX */
