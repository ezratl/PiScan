#pragma once

#include <memory>

namespace piscan::scan {
    class Entry;
    class RadioSystem;
    class SystemList;

    typedef std::shared_ptr<Entry> EntryPtr;
    typedef std::shared_ptr<RadioSystem> RadioSystemPtr;

    namespace database {
        constexpr char systems_key[] = "systems";

        namespace systems {
            constexpr char type_key[] = "systype";
            constexpr char type_analog[] = "analog";

            constexpr char channels_key[] = "channels";
            constexpr char tag_key[] = "tag";
            constexpr char lockout_key[] = "lockout";
        }

        namespace entry {
            namespace channel {
                constexpr char type_key[] = "chantype";
                constexpr char type_fm[] = "fmc";
                constexpr char type_am[] = "amc";
                constexpr char type_pl[] = "plc";
                constexpr char type_dc[] = "dcc";
            }

            constexpr char tag_key[] = "tag";
            constexpr char lockout_key[] = "lockout";
            constexpr char delay_key[] = "delay";
            constexpr char freq_key[] = "freq";
            constexpr char tone_key[] = "tone";
            constexpr char code_key[] = "code";
        }
    }
}
