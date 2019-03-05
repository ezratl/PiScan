
#include "loguru.hpp"
#include "messages.h"
#include "RtAudio.h"

class AudioManager : public MessageReceiver {
public:
    AudioManager();
    ~AudioManager() {};

private:
    bool _outputLocally;
    RtAudio _audioDriver;

    void giveMessage(Message& message);
    bool _startAudioOutput();
    bool _stopAudioOutput();
    void _muteAudio();
    void _unmuteAudio();
};
