
#include "loguru.hpp"
#ifdef USE_SYSTEM_RTAUDIO
#include <rtaudio/RtAudio.h>
#else
#include "RtAudio.h"
#endif

namespace piscan {

class AudioManager {
public:
    AudioManager();
    ~AudioManager() {};

private:
    bool _outputLocally;
    RtAudio _audioDriver;

    bool _startAudioOutput();
    bool _stopAudioOutput();
    void _muteAudio();
    void _unmuteAudio();
};
}
