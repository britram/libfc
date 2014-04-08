libfc - IPFIX implementation in C++, with a binding for C
=====

(The following sections are taken from the doxygen comments in
`PlacementTemplate.h` and `ipfix.h`.)

A Brief Description of IPFIX
----------------------------

IPFIX is sometimes called a _self-describing format_.  In order to
self-describe, an IPFIX message contains _template records_ that
describe the format of the _data records_, which contain the content.
A template record basically says, "Hi, I'm a template record with the
identifying number 1234.  Later in this message, there may be
collection of data records, called a _data set_, also having the
identifying number 1234.  This means that the records in that data set
will have the following structure: [...]" That structure is then
described by giving a sequence of _information elements_ (and their
encoded lengths as they appear on the wire).  For example, a
simplified version of a flow template record could look like this:

<table>
  <tr><th>IE name</th><th>length</th></tr>
  <tr><td>flowStartMilliseconds</td><td>8</td></tr>
  <tr><td>flowEndMilliseconds</td><td>8</td></tr>
  <tr><td>sourceIPv4Address</td><td>4</td></tr>
  <tr><td>destinationIPv4Address</td><td>4</td></tr>
  <tr><td>sourceTransportPort</td><td>2</td></tr>
  <tr><td>destinationTransportPort</td><td>2</td></tr>
  <tr><td>protocolIdentifier</td><td>1</td></tr>
  <tr><td>octetDeltaCount</td><td>4</td></tr>
</table>

(This is a simplified template record because a real record would
contain not the information element names, but rather some identifying
numbers.)

Then this means that later data sets with this template ID will
contain the given information elements in this order, with this
length.  Now it would be nice if we could read in a data record and
map it to a struct, simply by casting a pointer to the data read from
the data source.  But that is not possible for a number of reasons:

 - The data might need to undergo _endianness conversion_.  For
   example, numbers will appear on the wire in network byte order (big
   endian), but the native format will be host byte order, which might
   be little endian.  This conversion affects some, but not all,
   information elements.  For example, the flowStartMilliseconds and
   sourceIPv4Address information elements are affected, whereas a
   sourceIPv6Address information element would not be affected.

 - Some information elements have variable length; this is known as
   _varlen encoding_.  Obviously, varlen-encoded information elements
   cannot be directly mapped to structs, because structs have a fixed
   length, available with the `sizeof` operator.

 - Some information element types cannot be directly mapped to native
   C++ data types.  Examples would be the `octetArray` and `string`
   data types.

 - Information elements can be encoded on the wire with a length that
   is less than the length that would be implied by the information
   element's type.  This is called _reduced-length encoding_.  For
   example, the octetDeltaCount is of type `unsigned64`, but if only
   32 of those 64 bits would be needed in a particular application,
   then the information element may appear on the wire with a length
   of just four octets.

 - In a data record, the information elements follow one another
   without any _padding_. This can lead to data that is not suitably
   aligned.  For example, the protocolIdentifier information element
   above is only 1 octet long, causing the following octetDeltaCount
   information element to be aligned on an odd address (assuming that
   the base address was aligned on an even address).

 - A collection process might not be interested in all information
   elements, but only in a _selection_, so forcing it to receive the
   entire data record would be unreasonable.

 - The collection process might, for its very own reasons, want to
   _scatter_ the information elements in its address space, not
   keeping them together as in a struct.

An IPFIX collection process will therefore ultimately need to decide
where an information element as it appears in an IPFIX data record
will have to be stored in memory, just as an IPFIX exporter will have
to decide where an information element that is to be exported will
take its data from. This class provides the association between a set
of information elements and corresponding memory locations.


Placement Templates
-------------------

Here is how it works.  Let's say you're interested in flow records
like the one shown in the above table, but you're only interested in
the source and destination IP addresses, and not the other information
elements.  We encapsulate this in a data structure that we call a
_placement template_, and this code would let you express that
interest:

    uint32_t sip;
    uint32_t dip;
    IPFIX::InfoModel& model = IPFIX::InfoModel::instance();
  
    PlacementTemplate* my_flow_template = new PlacementTemplate();
   
    my_flow_template->register_placement(
       model.lookupIE("sourceIPv4Address"),
       &sip);
    my_flow_template->register_placement(
       model.lookupIE("destinationIPv4Address"),
       &dip);


### Collection

Now before you can start collecting records with these information
elements, you need two more things: first, you need to tell someone
that you want this placement template to go into effect, and second,
you need a way for that someone to tell you that it's just read a
record that matched this placement template and that it has placed the
values form the data record into the pointers that you have provided
in that placement template.  All this is provided by a custom class
derived from PlacementCollector (we will be using the IPFIX namespace
for simplicity):

    class MyCollector : public PlacementCollector {
    public:
      MyCollector() {
         // Create and fill my_flow_template as above
    
         register_placement_template(my_flow_template);
      }
    
      void end_placement(const PlacementTemplate* tmpl) {
        // At this point, there are fresh values in this->sip
        // and this->dip
      }
    
    private:
     uint32_t sip;
     uint32_t dip;
    };
     
    MyCollector cb;
    
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd >= 0) {
      FileInputSource is(fd);
      try {
        cb.collect(is);
      } catch (FormatError e) {
        std::cerr << "Format error: " << e.what() << std::endl;
      }
      (void) close(fd);
    }

And that's all, folks!

It should be noted that it is possible to register more than one
placement template in the MyCollector constructor:

    MyCollector() {
      PlacementTemplate* my_flow_template = new PlacementTemplate();
      // Fill my_flow_template as above
       register_placement_template(my_flow_template);
  
      PlacementTemplate* my_observation_template = new PlacementTemplate();
      // Fill my_obsrevation_template similarly
       register_placement_template(my_observation_template);
    }
  
This is the reason why the end_placement() member function has a
PlacementTemplate pointer parameter: so that you can distinguish
which of your templates has just been matched and hence which of
your data members now have fresh content.  Obviously, for this,
the template pointers should be data members of MyCollector.


### Export

Placement templates can also be used for export.  In fact, export
using the placement interface is slightly simpler than
collection, since no custom derived class is needed. (The reason
for this is that export needs no callback function, since the
information flow is now from the program to the IPFIX messages.)

Let's say you want to write IPFIX records to a file, and that you
have a file descriptor for that file in the variable
`fd`.  Here is how you could export flow records:

    FileExportDestination d(fd);
    PlacementExporter exporter(d, 0x12344321);
    
    uint64_t flow_start_milliseconds = ...;
    uint64_t flow_end_milliseconds = ...;
    uint32_t source_ip_v4_address = ...;
    uint32_t destination_ip_v4_address = ...;
    uint16_t source_transport_port = ...;
    uint16_t destination_transport_port = ...;
    uint8_t  protocol_identifier = ...;
    uint64_t octet_delta_count = ...;
    
    PlacementTemplate* my_flow_template = new PlacementTemplate();
    // Fill template as above
    
    exporter.place_values(my_flow_template);
    
    // more calls to place_values()
    
    exporter.flush();
    
    delete my_flow_template;

And that's all!

### C Binding

Let's say that you hate C++, and yet have a file called "test.ipfix",
containing IPFIX messages, and that you want to extract IP source and
destination addresses from it using C. Here is how you could do that
(with apologies to only rudimentary error handling):

    #include <stdint.h>
    #include <stdio.h>
    
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    
    #include "ipfix.h"

    static uint32_t sip;
    static uint32_t dip;
    
    static void callback(const struct ipfix_template_t* p) {
      printf("Got new values %08x and %08x\n", sip, dip);
    }
    
    int main() {
      int fd = open("test.ipfix", O_RDONLY);
      if (fd < 0)
        exit(1);
    
      struct ipfix_template_t* t = ipfix_template_new();
      ipfix_register_placement(t, "sourceIPv4Address", &sip, 0);
      ipfix_register_placement(t, "destinationIPv4Address", &dip, 0);
      ipfix_register_callback(t, callback);
      ipfix_collect_from_file(fd, t);
    
      if (close(fd) < 0)
        exit(1);
    
      ipfix_template_delete(t);
    
      return 0;
    }


Developed by
------------

Communication Systems Group
ETH Zurich
Contact: Brian Trammell <trammell@tik.ee.ethz.ch>
         Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>


License
------

BSD-Style license.

Copyright (c) 2011-2014 ETH Zürich. All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:
 Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
 Neither the names of ETH Zürich nor the names of other contributors 
     may be used to endorse or promote products derived from this software 
     without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH ZURICH BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
