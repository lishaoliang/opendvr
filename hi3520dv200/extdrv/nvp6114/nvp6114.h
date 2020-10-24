#ifndef _NVP6114_H_
#define _NVP6114_H_

#include <linux/ioctl.h>
#include "audio.h"

//#define ONLY_720H

#define NVP6114	0x60
#define NVP6114_R0_ID 	0x83

//FIXME HI3520 Register
#define VIU_CH_CTRL					0x08
#define VIU_ANC0_START				0x9c
#define VIU_ANC0_SIZE				0xa0
#define VIU_ANC1_START				0xa4
#define VIU_ANC1_SIZE				0xa8
#define VIU_BLANK_DATA_ADDR			0xac

#define VDEC_NTSC	        1
#define VDEC_PAL	        2
#define VDEC_AUTO	        3
#define VDEC_INVALID_MODE	4

#define NVP1114A_IOC_MAGIC            't'
#define NVP1114ACMD_READ_REG		    0x01
#define NVP1114ACMD_WRITE_REG	        0x02

#define NVP1114A_GET_VIDEO_LOSS			0x04
#define NVP1114A_SET_IMAGE_ADJUST	    0x05
#define NVP1114A_GET_IMAGE_ADJUST	    0x06
#define NVP1114A_SET_VIDEO_NORM	    	0x07
#define NVP1114A_GET_VIDEO_NORM	    	0x08
#define NVP1114A_SET_WORK_MODE	   		0x09

#define NVP1114A_SET_SAMPLE_RATE	    0x0a
#define NVP1114A_SET_AUDIO_PLAYBACK   	0x0b
#define NVP1114A_SET_AUDIO_DA_MUTE    	0x0c
#define NVP1114A_SET_AUDIO_DA_VOLUME  	0x0d

#define NVP1114A_SET_BRIGHTNESS	    	0x0e
#define NVP1114A_SET_CONTRAST   		0x0f
#define NVP1114A_SET_HUE    			0x10
#define NVP1114A_SET_SATURATION  		0x11
#define NVP1114A_SET_SHARPNESS  		0x12
#define NVP1114A_SET_AUDIO_MUTE	        0x13

#define IOC_AUDIO_SET_LIVE_CH			0x14
#define IOC_AUDIO_SET_PB_CH				0x15
#define NVP1108_GET_MOTION				0x16
#define NVP1108_SET_MOTION				0x17
#define NVP1108_SET_MOTION_SENS			0x18
#define NVP1108_SET_MOTION_EN			0x19
#define NVP1108_SET_MOTION_TRACE		0x1A


//Hwarang
#define IOC_VDEC_SET_VIDEO_MODE			0x07
#define IOC_VDEC_GET_VIDEO_LOSS     	0x09
#define IOC_VDEC_SET_BRIGHTNESS	    	0x0e
#define IOC_VDEC_SET_CONTRAST   		0x0f
#define IOC_VDEC_SET_HUE    			0x10
#define IOC_VDEC_SET_SATURATION  		0x11
#define IOC_VDEC_SET_SHARPNESS  		0x12
#define IOC_VDEC_PTZ_CHANNEL_SEL		0x20
#define IOC_VDEC_PTZ_PELCO_INIT			0x21
#define IOC_VDEC_PTZ_PELCO_RESET		0x22
#define IOC_VDEC_PTZ_PELCO_SET			0x23
#define IOC_VDEC_PTZ_PELCO_UP			0x24
#define IOC_VDEC_PTZ_PELCO_DOWN			0x25
#define IOC_VDEC_PTZ_PELCO_LEFT			0x26
#define IOC_VDEC_PTZ_PELCO_RIGHT		0x27
#define IOC_VDEC_PTZ_PELCO_OSD			0x28
#define IOC_VDEC_PTZ_PELCO_IRIS_OPEN	0x29
#define IOC_VDEC_PTZ_PELCO_IRIS_CLOSE	0x2a
#define IOC_VDEC_PTZ_PELCO_FOCUS_NEAR	0x2b
#define IOC_VDEC_PTZ_PELCO_FOCUS_FAR	0x2c
#define IOC_VDEC_PTZ_PELCO_ZOOM_WIDE	0x2d
#define IOC_VDEC_PTZ_PELCO_ZOOM_TELE	0x2e

#define IOC_VDEC_INIT_MOTION			0x40
#define IOC_VDEC_ENABLE_MOTION			0x41
#define IOC_VDEC_DISABLE_MOTION			0x42
#define IOC_VDEC_SET_MOTION_AREA		0x43
#define IOC_VDEC_GET_MOTION_INFO		0x44
#define IOC_VDEC_SET_MOTION_DISPLAY		0x45
#define IOC_VDEC_SET_MOTION_SENS		0x46

#define IOC_VDEC_ENABLE_LOW_RES			0x50
#define IOC_VDEC_DISABLE_LOW_RES		0x51

#define IOC_VDEC_ENABLE_BW				0x60
#define IOC_VDEC_DISABLE_BW				0x61
#define IOC_VDEC_READ_BALCK_COUNT		0x62
#define IOC_VDEC_READ_WHITE_COUNT		0x63
#define IOC_VDEC_4CH_VIDEO_SEQUENCE		0x64


#define IOC_AUDIO_SET_CHNNUM            0x80
#define IOC_AUDIO_SET_SAMPLE_RATE       0x81
#define IOC_AUDIO_SET_BITWIDTH          0x82




typedef struct _vdec_video_mode
{
    unsigned int chip;
    unsigned int mode;
}vdec_video_mode;

typedef struct _vdec_video_loss
{
    unsigned int chip;
    unsigned int ch;
    unsigned int is_lost;
}vdec_video_loss;

typedef struct _vdec_video_adjust
{
    unsigned int ch;
	unsigned int value;
}vdec_video_adjust;

typedef struct _NVP1114A_audio_output
{
	unsigned char PlaybackOrLoop;   /*0:playback; 1:loop*/
	unsigned char channel;          /*[0, 15]*/
	unsigned char reserve[6];
} nvp1114a_audio_output;

typedef struct _vdec_clock_mode
{
    unsigned int chip;
    unsigned int mode;
}vdec_clock_mode;


typedef struct _vdec_motion_area
{
    unsigned char ch;
    int m_info[12];
}vdec_motion_area;

typedef struct _vdec_audio_playback
{
    unsigned int chip;
    unsigned int ch;
}vdec_audio_playback;

typedef struct _vdec_audio_da_mute
{
    unsigned int chip;
}vdec_audio_da_mute;

typedef struct _vdec_audio_da_volume
{
    unsigned int chip;
    unsigned int volume;
}vdec_audio_da_volume;

enum{
	NVC1700_OUTMODE_2D1_8CIF = 0,
	NVC1700_OUTMODE_1D1_16CIF,	
	NVC1700_OUTMODE_4D1_16CIF,
	NVC1700_OUTMODE_BUTT
	};

enum{
	NVC1700_VM_NTSC = 0,
		NVC1700_VM_PAL,
		NVC1700_VM_BUTT
	};

enum{
	NVP6114_OUT_2X960_2X720P = 0,
	NVP6114_OUT_2X720P_2X960H,
	NVP6114_OUT_ALL_960H,
	NVP6114_OUT_ALL_720P,
	NVP6114_OUT_BUTT
};

typedef struct _vdec_input_videofmt
{
    //unsigned int ch;
    unsigned int inputvideofmt[16];
	unsigned int getvideofmt[16];
}vdec_input_videofmt;

enum{
	NVP6114_INPUT_VIDEOFMT_SD_NTSC = 0,
	NVP6114_INPUT_VIDEOFMT_SD_PAL,
	NVP6114_INPUT_VIDEOFMT_AHD_30FPS,
	NVP6114_INPUT_VIDEOFMT_AHD_25FPS,
	NVP6114_INPUT_VIDEOFMT_BUTT
};

#endif
