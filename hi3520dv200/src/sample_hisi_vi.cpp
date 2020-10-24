#include "sample_hisi_inc.h"
#include "log/klb_log.h"


int sample_hisi_vi_init()
{
    int ret = SAMPLE_COMM_VI_Start(SAMPLE_VI_MODE_1_1080P, VIDEO_ENCODING_MODE_NTSC);
    if (HI_SUCCESS != ret)
    {
        KLB_LOG("start vi failed!\n");
        return ret;
    }

    return 0;
}

void sample_hisi_vi_quit()
{
    SAMPLE_COMM_VI_Stop(SAMPLE_VI_MODE_1_1080P);
}

int sample_hisi_vi_bind_vpss()
{
    int ret = SAMPLE_COMM_VI_BindVpss(SAMPLE_VI_MODE_1_1080P);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    return 0;
}

void sample_hisi_vi_unbind_vpss()
{
    SAMPLE_COMM_VI_UnBindVpss(SAMPLE_VI_MODE_1_1080P);
}
