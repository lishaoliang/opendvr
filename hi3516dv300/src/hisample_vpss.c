#include "hi3516dv300_inc.h"
#include "klbutil/klb_log.h"
#include "klbmem/klb_mem.h"


// 修改来源 : Hi3516CV500_SDK_V2.0.2.0\smp\a7_linux\mpp\sample\venc\sample_venc.c, line : 363
// 修改函数 : SAMPLE_VENC_VPSS_Init
int hisample_vpss_init(hisample_vpss_config_t* p_vpss_cfg, DYNAMIC_RANGE_E enDynamicRange, PIXEL_FORMAT_E enPixelFormat)
{
    HI_BOOL pabChnEnable[VPSS_MAX_PHY_CHN_NUM];

    VPSS_GRP_ATTR_S stVpssGrpAttr = { 0 };
    VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];

    stVpssGrpAttr.enDynamicRange = enDynamicRange;      // DYNAMIC_RANGE_SDR8;
    stVpssGrpAttr.enPixelFormat = enPixelFormat;        // PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    stVpssGrpAttr.u32MaxW = p_vpss_cfg->grp_w;
    stVpssGrpAttr.u32MaxH = p_vpss_cfg->grp_h;
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stVpssGrpAttr.bNrEn = HI_TRUE;
    stVpssGrpAttr.stNrAttr.enNrType = VPSS_NR_TYPE_VIDEO;
    stVpssGrpAttr.stNrAttr.enNrMotionMode = NR_MOTION_MODE_NORMAL;
    stVpssGrpAttr.stNrAttr.enCompressMode = COMPRESS_MODE_FRAME;

    for (int i = 0; i < VPSS_MAX_PHY_CHN_NUM; i++)
    {
        if ((0 < p_vpss_cfg->vpss_chn[i].w) && (0 < p_vpss_cfg->vpss_chn[i].h))
        {
            pabChnEnable[i] = HI_TRUE;
        }
        else
        {
            pabChnEnable[i] = HI_FALSE;
        }

        if (HI_TRUE == pabChnEnable[i])
        {
            stVpssChnAttr[i].u32Width = p_vpss_cfg->vpss_chn[i].w;
            stVpssChnAttr[i].u32Height = p_vpss_cfg->vpss_chn[i].h;
            stVpssChnAttr[i].enChnMode = VPSS_CHN_MODE_USER;
            stVpssChnAttr[i].enCompressMode = COMPRESS_MODE_NONE;//COMPRESS_MODE_SEG;
            stVpssChnAttr[i].enDynamicRange = enDynamicRange;
            stVpssChnAttr[i].enPixelFormat = enPixelFormat;
            stVpssChnAttr[i].stFrameRate.s32SrcFrameRate = -1;
            stVpssChnAttr[i].stFrameRate.s32DstFrameRate = -1;
            stVpssChnAttr[i].u32Depth = 0;
            stVpssChnAttr[i].bMirror = HI_FALSE;
            stVpssChnAttr[i].bFlip = HI_FALSE;
            stVpssChnAttr[i].enVideoFormat = VIDEO_FORMAT_LINEAR;
            stVpssChnAttr[i].stAspectRatio.enMode = ASPECT_RATIO_NONE;
        }
    }

    HI_S32 s32Ret = SAMPLE_COMM_VPSS_Start(p_vpss_cfg->grp_id, pabChnEnable, &stVpssGrpAttr, stVpssChnAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("start VPSS fail for %#x!\n", s32Ret);
    }

    return s32Ret;
}

void hisample_vpss_quit(hisample_vpss_config_t* p_vpss_cfg)
{
    HI_BOOL pabChnEnable[VPSS_MAX_PHY_CHN_NUM];

    for (int i = 0; i < VPSS_MAX_PHY_CHN_NUM; i++)
    {
        if ((0 < p_vpss_cfg->vpss_chn[i].w) && (0 < p_vpss_cfg->vpss_chn[i].h))
        {
            pabChnEnable[i] = HI_TRUE;
        }
        else
        {
            pabChnEnable[i] = HI_FALSE;
        }
    }

    SAMPLE_COMM_VPSS_Stop(p_vpss_cfg->grp_id, pabChnEnable);
}

int hisample_vpss_set_depth(int vpss_grp, int vpss_chn, int depth)
{
    VPSS_CHN_ATTR_S attr;
    KLB_MEMSET(&attr, 0, sizeof(VPSS_CHN_ATTR_S));

    HI_S32 ret_get = HI_MPI_VPSS_GetChnAttr((VPSS_GRP)vpss_grp, (VPSS_CHN)vpss_chn, &attr);
    if (0 != ret_get)
    {
        KLB_LOG("HI_MPI_VPSS_GetChnAttr error!vpss grp:[%d],vpss chn:[%d],ret:[%#x]\n", vpss_grp, vpss_chn, ret_get);
        return ret_get;
    }

    attr.u32Depth = depth;

    HI_S32 ret_set = HI_MPI_VPSS_SetChnAttr((VPSS_GRP)vpss_grp, (VPSS_CHN)vpss_chn, &attr);
    if (0 != ret_set)
    {
        KLB_LOG("HI_MPI_VPSS_SetChnAttr error!vpss grp:[%d],vpss chn:[%d],ret:[%#x]\n", vpss_grp, vpss_chn, ret_set);
        return ret_set;
    }

    return 0;
}
