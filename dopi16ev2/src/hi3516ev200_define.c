#include "hi3516ev200_define.h"


int hi3516ev200_resolution_bit_rate(int resolution/*klb_resolution_e*/, int frame_rate)
{
    switch (resolution)
    {
    // 4:3
    case KLB_RESOLUTION_256x190:
    {
        return 64 * 1 + 64 * frame_rate / 30;
    }
    case KLB_RESOLUTION_320x240:
    {
        return 64 * 2 + 64 * frame_rate / 30;
    }
    case KLB_RESOLUTION_480x360:
    {
        return 512 * 1 + 512 * frame_rate / 30;
    }
    case KLB_RESOLUTION_640x480:
    {
        return 512 * 2 + 512 * frame_rate / 30;
    }
    case KLB_RESOLUTION_768x576:
    {
        return 512 * 2 + 512 * frame_rate / 30;
    }
    case KLB_RESOLUTION_1024x768:
    {
        return 512 * 3 + 512 * frame_rate / 30;
    }
    case KLB_RESOLUTION_1400x1050:
    {
        return 1024 * 1 + 1024 * frame_rate / 30;
    }
    case KLB_RESOLUTION_1600x1200:
    {
        return 1024 * 2 + 1024 * frame_rate / 30;
    }
    case KLB_RESOLUTION_2048x1536:
    {
        return 1024 * 4 + 1024 * frame_rate / 30;
    }

    // 16:9
    case KLB_RESOLUTION_256x144:
    {
        return 64 * 1 + 64 * frame_rate / 30;
    }
    case KLB_RESOLUTION_384x216:
    {
        return 64 * 2 + 64 * frame_rate / 30;
    }
    case KLB_RESOLUTION_480x272:
    {
        return 512 * 1 + 512 * frame_rate / 30;
    }
    case KLB_RESOLUTION_640x360:
    {
        return 512 * 2 + 512 * frame_rate / 30;
    }
    case KLB_RESOLUTION_960x540:
    {
        return 512 * 3 + 512 * frame_rate / 30;
    }
    case KLB_RESOLUTION_1280x720:
    {
        return 1024 * 1 + 1024 * frame_rate / 30;
    }
    case KLB_RESOLUTION_1600x900:
    {
        return 1024 * 2 + 1024 * frame_rate / 30;;
    }
    case KLB_RESOLUTION_1920x1080:
    {
        return 1024 * 3 + 1024 * frame_rate / 30;
    }
    case KLB_RESOLUTION_2560x1440:
    {
        return 1024 * 4 + 1024 * frame_rate / 30;
    }
    case KLB_RESOLUTION_3200x1800:
    {
        return 1024 * 5 + 1024 * frame_rate / 30;
    }
    case KLB_RESOLUTION_3840x2160:
    {
        return 1024 * 7 + 1024 * frame_rate / 30;
    }
    case KLB_RESOLUTION_5120x2880:
    {
        return 1024 * 9 + 1024 * frame_rate / 30;
    }
    case KLB_RESOLUTION_7680x4320:
    {
        return 1024 * 11 + 1024 * frame_rate / 30;
    }
    case KLB_RESOLUTION_9600x5400:
    {
        return 1024 * 13 + 1024 * frame_rate / 30;
    }
    case KLB_RESOLUTION_10240x5760:
    {
        return 1024 * 15 + 1024 * frame_rate / 30;
    }
    default:
        break;
    }

    return 1024;
}
