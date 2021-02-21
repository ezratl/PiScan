import vlc
import time
import threading

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
    do_play = False
    monitor_thread = None
    volume = 50
    mute = False

    def __init__(self):
        self.vlc = vlc.Instance('--no-video', '--repeat')#, '-v')
        self.vlc_lock = threading.Lock()

    def close(self):
        if do_play:
            self.stopRtspAudioStream()

        if self.player:
            self.player.release()

        self.vlc.release()

    def startRtspAudioStream(self, host, port) -> bool :
        with self.vlc_lock:
            try:
                print('create media')
                self.media = self.vlc.media_new('rtsp://' + host + ':' + str(port) + '/audio', 'network-caching=50')
                #media.add_options('input-repeat=65535') # NOTE: repeat leads to the error: 'main input error: INPUT_CONTROL_SET_POSITION 0.0% failed'
                print('create new player')
                self.player = self.media.player_new_from_media()
                print('start player')
                self.player.play()

                self.player.audio_set_volume(self.volume)
                self.player.audio_set_mute(self.mute)

                self.do_play = True
                self.monitor_thread = threading.Thread(target=self.monitor, name='audio monitor')
                self.monitor_thread.start()
                return True
            except:
                print('error starting stream')
                return False

    def stopRtspAudioStream(self):
        print('stopping audio')

        with self.vlc_lock:
            if self.do_play and self.player:
                self.do_play = False
                self.player.stop()
                self.player.release()
                self.player = None
                self.media.release()
                self.media = None

        self.monitor_thread.join()
        self.monitor_thread = None
        print('stopped')

    def setAudioVolume(self, level : int):
        with self.vlc_lock:
            self.volume = level
            self.player.audio_set_volume(self.volume)

    def setAudioMute(self, mute : bool):
        with self.vlc_lock:
            self.mute = mute
            self.player.audio_set_mute(self.mute)

    # NOTE: this is a hacky way to maintain stream playback; for some reason using vlc's repeat parameter doesn't work
    def monitor(self):
        print('start stream monitor thread')
        while True:
            with self.vlc_lock:
                if not self.do_play:
                    break
                state = self.player.get_state()
                if state == vlc.State.Ended or state == vlc.State.Error:
                    print('attempting to restart stream')
                    self.player.stop()
                    self.player.release()
                    self.player = self.media.player_new_from_media()
                    self.player.play()
                    self.player.audio_set_volume(self.volume)
                    self.player.audio_set_mute(self.mute)
            time.sleep(0.01) # sleep 10ms
        print('exiting stream monitor')                    

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
