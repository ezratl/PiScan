#include "messages/context.h"

#include "Entry.h"

namespace piscan {
namespace server {
namespace context {

ScannerContext::ScannerContext(const ScannerContext &copy) : state(copy.state), systemTag(copy.systemTag), entryTag(
                                                                                                               copy.entryTag),
                                                             frequency(copy.frequency), modulation(
                                                                                            copy.modulation),
                                                             entryIndex(copy.entryIndex){};

ScannerContext::ScannerContext(ScannerState state, std::string sysTag, std::string entryTag,
                               long long freq, std::string mod, std::string index) : state(state), systemTag(sysTag), entryTag(entryTag), frequency(freq), modulation(mod), entryIndex(index){};

ScannerContext::ScannerContext(ScannerState state, piscan::scan::Entry &entry) : state(state), systemTag(entry.tag()){

}                                                                                 };
}
}
