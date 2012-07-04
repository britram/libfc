using namespace IPFIX;

#define BOOST_TEST_DYN_LINK
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>


public static uint64_t kTimeSeqStart = 1141344000000UL; // March 3, 2006
public static uint64_t kTimeSeqStep  = 1000;            // one second
public static uint32_t kIPSeqStart   = 0x0A000000;      // 10.0.0.1
public static uint32_t kIPSeqEnd     = 0x0AFFFFFF;      // 10.255.255.255
public static uint32_t kIPSeqStep    = 1;
public static uint16_t kPortSeqStart = 1;
public static uint16_t kPortSeqEnd   = 65535;
public static uint32_t kPortSeqStep  = 1;
public static uint64_t kOctetsSeqStart = 44;
public static uint64_t kOctetsSeqEnd   = 1000000000;
public static uint64_t kOctetsSeqStep  = 44;

public static uint16_t kFlowTemplateId = 256;
public static uint16_t kObsTemplateId = 257;

public static int kTestRecordCount = 100000;
public static int kTestFlowPerSetCount = 22;
public static int kTestObsPerSetCount = 11;

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

public:
    
    TestFlow():
        stime(kTimeSeqStart),
        etime(kTimeSeqStart + kFlowDuration),
        sip(kIPSeqStart),
        dip(kIPSeqEnd),
        sp(kPortSeqStart),
        dp(kPortSeqEnd),
        proto(6),
        octets(kOctetsSeqStart) {}
    
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
    
    bool operator== (const TestFlow& rhs) const {
        if (stime == rhs.stime &&
            etime == rhs.etime &&
            sip == rhs.sip &&
            dip == rhs.dip &&
            sp == rhs.sp &&
            dp == rhs.dp && 
            proto == rhs.proto &&
            octets == rhs.octets)
        {
            return true;
        } else {
            return false;
        }
    }

    void prepareExport(Exporter& e) {
        InfoModel* m = InfoModel::instance();

        ie_stime_ = m->lookupIE("flowStartMilliseconds");
        ie_etime_ = m->lookupIE("flowEndMilliseconds");
        ie_sip_ = m->lookupIE("sourceIPv4Address");
        ie_dip_ = m->lookupIE("destinationIPv4Address");
        ie_sp_ = m->lookupIE("sourceTransportPort");
        ie_dp_ = m->lookupIE("destinationTransportPort");
        ie_proto_ = m->lookupIE("protocolIdentifier");
        ie_octets_ = m->lookupIE("octetDeltaCount[4]");

        WireTemplate* t = e.getTemplate(kFlowTemplateId);
        t->clear();
        t->add(ie_stime_);
        t->add(ie_etime_);
        t->add(ie_sip_);
        t->add(ie_dip_);
        t->add(ie_sp_);
        t->add(ie_dp_);
        t->add(ie_proto_);
        t->add(ie_octets_);
        t->activate();
    }

    void export(Exporter& e) {
        e.setTemplate(kObsTemplateId);
        e.beginRecord();
        e.putValue(ie_stime_, stime_);
        e.putValue(ie_etime_, etime_);
        e.putValue(ie_sip_, sip_);
        e.putValue(ie_dip_, dip_);
        e.putValue(ie_sp_, sp_);
        e.putValue(ie_dp_, dp_);
        e.putValue(ie_proto_, proto_);
        e.putValue(ie_octets_, octets_);
        e.exportRecord();

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
        InfoModel* m = InfoModel::instance();

        m->add("observationValue(35566/804)<unsigned64>[8]");
        m->add("observationLabel(35566/805)<string>[v]");
        
        ie_otime_ = m->lookupIE("observationTimeMilliseconds");
        ie_value_ = m->lookupIE("observationValue");
        ie_label_ = m->lookupIE("observationLabel");
        
        WireTemplate* t = e.getTemplate(kObsTemplateId);
        t->clear();
        t->add(ie_otime_);
        t->add(ie_value_);
        t->add(ie_label_);
        t->activate();
    }

    void export(Exporter& e) {
        e.setTemplate(kObsTemplateId);
        e.beginRecord();
        e.putValue(ie_otime_, otime_);
        e.putValue(ie_value_, value_);
        e.putValue(ie_label_, label_);
        e.exportRecord();
    }
};

