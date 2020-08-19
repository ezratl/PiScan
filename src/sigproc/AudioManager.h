
#include "loguru.hpp"
#include "messages.h"
#include "RtAudio.h"

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