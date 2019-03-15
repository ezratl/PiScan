

#ifndef DRIVERS_RTL_FM_H_
#define DRIVERS_RTL_FM_H_

#define RTL_MIN_FREQ	24000000
#define RTL_MAX_FREQ	1450000000

enum custom_atan {
    STD,
    FAST,
    LUT,
    ALE
};

enum mode_demod {
    FM,
    RAW,
    AM,
    USB,
    LSB,
    WBFM
};

#ifdef __cplusplus
extern "C" {
#endif
    int rtl_fm_init(void* audioBuffer, size_t bufferSize, int sampleRate, int gain);
    int rtl_fm_deinit();

    int rtl_fm_setfreq(uint32_t freq);
    int rtl_fm_setmode(enum mode_demod newMode);
    float rtl_fm_get_rssi();
    void rtl_fm_set_gain(int level);

    //TODO temporary
    void rtl_fm_mute(int mute);
#ifdef __cplusplus
}
#endif

#endif
