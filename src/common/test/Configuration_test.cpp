#include "gtest/gtest.h"
#include "Configuration.h"

namespace piscan {
namespace config {

class ConfigurationTest : public testing::Test {
protected:
    Configuration& config = Configuration::getConfig();
};

TEST_F(ConfigurationTest, TestSetDirectory) {
    
}

}
}
