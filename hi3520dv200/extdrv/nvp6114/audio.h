

typedef enum _nvp6114_audio_bites_en
{
    NVP6114_BITES_WIDTH_16 = 0,
    NVP6114_BITES_WIDTH_8  = 1,
    NVP6114_BITES_WIDTH_BUTT
} nvp6114_audio_bites_en;




typedef enum _nvp6114_audio_samplerate_en
{
    NVP6114_SAMPLE_RATE_8000  = 0,
    NVP6114_SAMPLE_RATE_16000 = 1,
    NVP6114_SAMPLE_RATE_BUTT
} nvp6114_audio_samplerate_en;




void audio_init(unsigned char dec, unsigned char ch_num, unsigned char samplerate, unsigned char bits);

unsigned int audio_set_chnnum(unsigned int ch_num);

unsigned int audio_set_samplerate(nvp6114_audio_samplerate_en e_samplerate);

unsigned int audio_set_bites_width(nvp6114_audio_bites_en e_bites);





