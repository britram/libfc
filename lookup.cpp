#include <iostream>

#include "InfoModel.h"

using namespace IPFIX;

int main(void) {
  InfoModel m = InfoModel::instance();

  m.defaultIPFIX();

  const InfoElement* e = m.lookupIE("octetDeltaCount");
  if (e == 0) 
    std::cerr << "is null!" << std::endl;

  return 0;
}
