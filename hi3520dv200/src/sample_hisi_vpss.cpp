#include "sample_hisi_inc.h"
#include "log/klb_log.h"


int sample_hisi_vpss_init(int w, int h)
{
    SIZE_S stSize = { 0 };
    stSize.u32Width = w;
    stSize.u32Height = h;

    VPSS_GRP_ATTR_S stGrpAttr = { 0 };
    stGrpAttr.u32MaxW = stSize.u32Width;
    stGrpAttr.u32MaxH = stSize.u32Height;
    stGrpAttr.bDrEn = HI_FALSE;
    stGrpAttr.bDbEn = HI_FALSE;
    stGrpAttr.bIeEn = HI_TRUE;
    stGrpAttr.bNrEn = HI_TRUE;
    stGrpAttr.bHistEn = HI_TRUE;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_AUTO;
    stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

    int ret = SAMPLE_COMM_VPSS_Start(1, &stSize, VPSS_MAX_CHN_NUM, NULL);

    return ret;
}

void sample_hisi_vpss_quit()
{
    SAMPLE_COMM_VPSS_Stop(1, VPSS_MAX_CHN_NUM);
}
