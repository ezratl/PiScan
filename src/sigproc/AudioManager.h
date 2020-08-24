
#include "loguru.hpp"
#include "messages.h"
#ifdef USE_SYSTEM_RTAUDIO
#include <rtaudio/RtAudio.h>
#else
#include "RtAudio.h"
#endif

namespace piscan {

class AudioManager : public MessageReceiver {
public:
    AudioManager();
    ~AudioManager() {};

private:
    bool _outputLocally;
    RtAudio _audioDriver;

    void giveMessage(std::shared_ptr<Message> message);
    bool _startAudioOutput();
    bool _stopAudioOutput();
    void _muteAudio();
    void _unmuteAudio();
};
}
