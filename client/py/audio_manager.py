import vlc
import time

#instance = vlc.Instance("-vvv", "--no-video", "--repeat")
#player = instance.media_player_new()
#media = instance.media_new("rtsp://piscan-arbor:8554/audio", "network-caching=25")
#player.set_media(media)
#player.play()
#while(True):
#    time.sleep(1)

class AudioManager:
    vlc = None
    player = None
    media = None

    def __init__(self):
        self.vlc = vlc.Instance('--no-video', '--repeat', '-v')

    def close(self):

        if self.player:
            self.player.stop()
            self.player.release()

        self.vlc.release()

    def startRtspAudioStream(self, host, port):
        print('create media')
        self.media = self.vlc.media_new('rtsp://' + host + ':' + port + '/audio', 'network-caching=50')
        print('create new player')
        self.player = self.media.player_new_from_media()
        print('start player')
        self.player.play()

    def stopRtspAudioStream(self):
        print('stopping audio')
        if self.player:
            self.player.stop()
            self.player.release()
            self.player = None
            self.media.release()
            self.media = None

    def setAudioVolume(self, level : int):
        self.player.audio_set_volume(level)

    def setAudioMute(self, mute : bool):
        self.player.audio_set_mute(mute)

if __name__ == '__main__':
    audio = AudioManager()

    audio.startRtspAudioStream("pibox-airglow", "8554")

    try:
        while(True):
            time.sleep(1)
    except KeyboardInterrupt:
        pass

    audio.stopRtspAudioStream()

    audio.close()
