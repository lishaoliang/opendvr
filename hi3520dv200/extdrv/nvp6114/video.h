#ifndef _VDEC_VIDEO_HI_
#define _VDEC_VIDEO_HI_

void vdec_ntsc_common_init(void);
void vdec_pal_common_init(void);
void nvp6114_720p_30fps( void );
void nvp6114_720p_25fps( void );

void vdec_video_set_contrast(unsigned int ch, unsigned int value, unsigned int v_format);
void vdec_video_set_brightness(unsigned int ch, unsigned int value, unsigned int v_format);
void vdec_video_set_saturation(unsigned int ch, unsigned int value, unsigned int v_format);
void vdec_video_set_hue(unsigned int ch, unsigned int value, unsigned int v_format);
void nvp6114_960H_setting(unsigned char vformat, unsigned char channel_mode);

#endif
