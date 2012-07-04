#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "RecordReceiver.h"

#include "TestCommon.h"

using namespace IPFIX;

static uint64_t kTimeSeqStart = 1141344000000UL; // March 3, 2006
static uint64_t kTimeSeqStep  = 1000;            // one second
static uint32_t kIPSeqStart   = 0x0A000000;      // 10.0.0.1
static uint32_t kIPSeqEnd     = 0x0AFFFFFF;      // 10.255.255.255
static uint32_t kIPSeqStep    = 1;
static uint16_t kPortSeqStart = 1;
static uint16_t kPortSeqEnd   = 65535;
static uint32_t kPortSeqStep  = 1;
static uint64_t kOctetsSeqStart = 44;
static uint64_t kOctetsSeqEnd   = 1000000000;
static uint64_t kOctetsSeqStep  = 44;

static uint16_t kFlowTemplateId = 256;
static uint16_t kObsTemplateId = 257;

static int kTestRecordCount = 100000;
static int kTestFlowPerSetCount = 22;
static int kTestObsPerSetCount = 11;

class TestFlow {
    
private:
    
    uint64_t        stime;
    uint64_t        etime;
    uint32_t        sip;
    uint32_t        dip;
    uint16_t        sp;
    uint16_t        dp;
    uint8_t         proto;
    uint64_t        octets;

    const InfoElement*    ie_stime;
    const InfoElement*    ie_etime;
    const InfoElement*    ie_sip;
    const InfoElement*    ie_dip;
    const InfoElement*    ie_sp;
    const InfoElement*    ie_dp;
    const InfoElement*    ie_proto;
    const InfoElement*    ie_octets;

public:
    
    TestFlow():
        stime(kTimeSeqStart),
        etime(kTimeSeqStart + kTimeSeqStep),
        sip(kIPSeqStart),
        dip(kIPSeqEnd),
        sp(kPortSeqStart),
        dp(kPortSeqEnd),
        proto(6),
        octets(kOctetsSeqStart) {}

  TestFlow(uint64_t stime,
           uint64_t etime,
           uint32_t sip,
           uint32_t dip,
           uint16_t sp,
           uint16_t dp,
           uint8_t  proto,
           uint64_t octets)
    : stime(stime),
      etime(etime),
      sip(sip),
      dip(dip),
      sp(sp),
      dp(dp),
      proto(proto),
      octets(octets) {
  }

    void incrementPattern() {
        stime += kTimeSeqStep;
        etime += kTimeSeqStep;
        sip += kIPSeqStep; if (sip > kIPSeqEnd) sip = kIPSeqStart;
        dip -= kIPSeqStep; if (dip < kIPSeqStart) sip = kIPSeqEnd;
        sp += kPortSeqStep; if (sp > kPortSeqEnd) sp = kPortSeqStart;
        dp -= kPortSeqStep; if (sp < kPortSeqStart) sp = kPortSeqEnd;
        octets += kOctetsSeqStep; 
        if (octets > kOctetsSeqEnd) octets = kOctetsSeqStart;
    }
    
    bool operator==(const TestFlow& rhs) const {
        return stime == rhs.stime &&
          etime == rhs.etime &&
          sip == rhs.sip &&
          dip == rhs.dip &&
          sp == rhs.sp &&
          dp == rhs.dp && 
          proto == rhs.proto &&
          octets == rhs.octets;
    }

    bool operator!=(const TestFlow& rhs) const {
      return !(*this == rhs);
    }

    void prepareExport(Exporter& e) {
        InfoModel& m = InfoModel::instance();

        ie_stime = m.lookupIE("flowStartMilliseconds");
        ie_etime = m.lookupIE("flowEndMilliseconds");
        ie_sip = m.lookupIE("sourceIPv4Address");
        ie_dip = m.lookupIE("destinationIPv4Address");
        ie_sp = m.lookupIE("sourceTransportPort");
        ie_dp = m.lookupIE("destinationTransportPort");
        ie_proto = m.lookupIE("protocolIdentifier");
        ie_octets = m.lookupIE("octetDeltaCount[4]");

        WireTemplate* t = e.getTemplate(kFlowTemplateId);
        t->clear();
        t->add(ie_stime);
        t->add(ie_etime);
        t->add(ie_sip);
        t->add(ie_dip);
        t->add(ie_sp);
        t->add(ie_dp);
        t->add(ie_proto);
        t->add(ie_octets);
        t->activate();
    }

    void do_export(Exporter& e) {
        e.setTemplate(kObsTemplateId);
        e.beginRecord();
        e.putValue(ie_stime, stime);
        e.putValue(ie_etime, etime);
        e.putValue(ie_sip, sip);
        e.putValue(ie_dip, dip);
        e.putValue(ie_sp, sp);
        e.putValue(ie_dp, dp);
        e.putValue(ie_proto, proto);
        e.putValue(ie_octets, octets);
        e.exportRecord();
    }

};

std::string& obsLabelFor(uint64_t value) {
    
    static std::vector<std::string> labels;
    static bool labels_valid = false;
    
    if (!labels_valid) {
        labels_valid = true;
        labels.clear();
        labels.push_back("To-morrow,"); 
        labels.push_back("and"); 
        labels.push_back("to-morrow,"); 
        labels.push_back("and"); 
        labels.push_back("to-morrow,");
        labels.push_back("Creeps"); 
        labels.push_back("in"); 
        labels.push_back("this"); 
        labels.push_back("petty"); 
        labels.push_back("pace"); 
        labels.push_back("from"); 
        labels.push_back("day"); 
        labels.push_back("to"); 
        labels.push_back("day,");
        labels.push_back("To"); 
        labels.push_back("the"); 
        labels.push_back("last"); 
        labels.push_back("syllable"); 
        labels.push_back("of"); 
        labels.push_back("recorded"); 
        labels.push_back("time;");
    }
    
    return labels[value % labels.size()];
}

class TestObs {

private:
    
    uint64_t                otime_;
    uint64_t                value_;
    std::string             label_;
    const InfoElement*      ie_otime_;
    const InfoElement*      ie_value_;
    const InfoElement*      ie_label_;

public:

    TestObs():
        otime_(kTimeSeqStart),
        value_(0),
        label_(obsLabelFor(0))
        {}

    void incrementPattern() {
        otime_ += kTimeSeqStep;
        value_ += 1;
        label_ = obsLabelFor(value_);
    }

    bool operator== (const TestObs& rhs) const {
        if (otime_ == rhs.otime_ &&
            label_ == rhs.label_ &&
            value_ == rhs.value_)
        {
            return true;
        } else {
            return false;
        }
    }

    void prepareExport(Exporter& e) {
        InfoModel& m = InfoModel::instance();

        m.add("observationValue(35566/804)<unsigned64>[8]");
        m.add("observationLabel(35566/805)<string>[v]");
        
        ie_otime_ = m.lookupIE("observationTimeMilliseconds");
        ie_value_ = m.lookupIE("observationValue");
        ie_label_ = m.lookupIE("observationLabel");
        
        WireTemplate* t = e.getTemplate(kObsTemplateId);
        t->clear();
        t->add(ie_otime_);
        t->add(ie_value_);
        t->add(ie_label_);
        t->activate();
    }

    void do_export(Exporter& e) {
        e.setTemplate(kObsTemplateId);
        e.beginRecord();
        e.putValue(ie_otime_, otime_);
        e.putValue(ie_value_, value_);
        e.putValue(ie_label_, label_);
        e.exportRecord();
    }
};

class TestFlowReceiver : public RecordReceiver {
private:
  TestFlow f_;
  bool pass_;
  unsigned int rec_count;

    const InfoElement*    ie_stime;
    const InfoElement*    ie_etime;
    const InfoElement*    ie_sip;
    const InfoElement*    ie_dip;
    const InfoElement*    ie_sp;
    const InfoElement*    ie_dp;
    const InfoElement*    ie_proto;
    const InfoElement*    ie_octets;

public:
  TestFlowReceiver() 
    : pass_(true), 
      rec_count(0),
      ie_stime(InfoModel::instance().lookupIE("flowStartMilliseconds")),
      ie_etime(InfoModel::instance().lookupIE("flowEndMilliseconds")),
      ie_sip(InfoModel::instance().lookupIE("sourceIPv4Address")),
      ie_dip(InfoModel::instance().lookupIE("destinationIPv4Address")),
      ie_sp(InfoModel::instance().lookupIE("sourceTransportPort")),
      ie_dp(InfoModel::instance().lookupIE("destinationTransportPort")),
      ie_proto(InfoModel::instance().lookupIE("protocolIdentifier")),
      ie_octets(InfoModel::instance().lookupIE("octetDeltaCount[4]")) {
  }

  bool is_passing() const { 
    return pass_;
  }

  unsigned int get_rec_count() const {
    return rec_count;
  }

  void receiveRecord()  {
    rec_count++;

    uint64_t stime;
    uint64_t etime;
    uint32_t sip;
    uint32_t dip;
    uint16_t sp;
    uint16_t dp;
    uint8_t  proto;
    uint64_t octets;

    if (getValue(ie_stime, stime)
        && getValue(ie_etime, etime)
        && getValue(ie_sip, sip)
        && getValue(ie_dip, dip)
        && getValue(ie_sp, sp)
        && getValue(ie_dp, dp)
        && getValue(ie_proto, proto)
        && getValue(ie_octets, octets)) {
      TestFlow f(stime, etime, sip, dip, sp, dp, proto, octets);
      if (f_ != f)
        pass_ = false;
    } else
      pass_ = false;

    f_.incrementPattern();
  }
};

BOOST_AUTO_TEST_SUITE(ImportExport)

BOOST_AUTO_TEST_CASE(LoopFile) {
  TestFlow flow;
  TestObs obs;
  Exporter *e = new FileWriter("loopfile", kTestDomain);

  flow.prepareExport(*e);
  obs.prepareExport(*e);

  for (unsigned int i = 0 ; i < kTestRecordCount; i++) {
    for (unsigned int k = 0; k < kTestFlowPerSetCount; k++) {
      flow.do_export(*e);
      flow.incrementPattern();
    }

    for (unsigned int k = 0 ; k < kTestObsPerSetCount; k++) {
      obs.do_export(*e);
      obs.incrementPattern();
    }
  }

  e->flush();
  delete e;
}

BOOST_AUTO_TEST_SUITE_END()
