

#ifndef DRIVERS_RTL_FM_H_
#define DRIVERS_RTL_FM_H_

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
    int rtl_fm_init(void* audioBuffer, size_t bufferSize, int sampleRate);
    int rtl_fm_deinit();

    int rtl_fm_setfreq(uint32_t freq);
    int rtl_fm_setmode(enum mode_demod newMode);
    int rtl_fm_get_rssi();
#ifdef __cplusplus
}
#endif

#endif
