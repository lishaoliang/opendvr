#include "hi3516dv300_inc.h"
#include "klbutil/klb_log.h"
#include "klbmem/klb_mem.h"
#include <assert.h>


// 修改来源 : Hi3516CV500_SDK_V2.0.2.0\smp\a7_linux\mpp\sample\venc\sample_venc.c, line : 284
// 修改函数 : SAMPLE_VENC_VI_Init
int hisample_vi_init(SAMPLE_VI_CONFIG_S* pstViConfig, HI_BOOL bLowDelay)
{
    HI_S32              s32Ret;
    SAMPLE_SNS_TYPE_E   enSnsType;
    ISP_CTRL_PARAM_S    stIspCtrlParam;
    HI_U32              u32FrameRate;

    enSnsType = pstViConfig->astViInfo[0].stSnsInfo.enSnsType;

    pstViConfig->as32WorkingViId[0] = 0;
    //pstViConfig->s32WorkingViNum                              = 1;

    pstViConfig->astViInfo[0].stSnsInfo.MipiDev = SAMPLE_COMM_VI_GetComboDevBySensor(pstViConfig->astViInfo[0].stSnsInfo.enSnsType, 0);
    pstViConfig->astViInfo[0].stSnsInfo.s32BusId = 0;

    //pstViConfig->astViInfo[0].stDevInfo.ViDev              = ViDev0;
    pstViConfig->astViInfo[0].stDevInfo.enWDRMode = WDR_MODE_NONE;

    if (HI_TRUE == bLowDelay)
    {
        pstViConfig->astViInfo[0].stPipeInfo.enMastPipeMode = VI_ONLINE_VPSS_ONLINE;
    }
    else
    {
        pstViConfig->astViInfo[0].stPipeInfo.enMastPipeMode = VI_OFFLINE_VPSS_OFFLINE;
    }

#if 0
    // mmz内存提前分配
    s32Ret = SAMPLE_VENC_SYS_Init(u32SupplementConfig, enSnsType);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("Init SYS err for %#x!\n", s32Ret);
        return s32Ret;
    }
#endif

    //pstViConfig->astViInfo[0].stPipeInfo.aPipe[0]          = ViPipe0;
    pstViConfig->astViInfo[0].stPipeInfo.aPipe[1] = -1;
    pstViConfig->astViInfo[0].stPipeInfo.aPipe[2] = -1;
    pstViConfig->astViInfo[0].stPipeInfo.aPipe[3] = -1;

    //pstViConfig->astViInfo[0].stChnInfo.ViChn              = ViChn;
    //pstViConfig->astViInfo[0].stChnInfo.enPixFormat        = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    //pstViConfig->astViInfo[0].stChnInfo.enDynamicRange     = enDynamicRange;
    pstViConfig->astViInfo[0].stChnInfo.enVideoFormat = VIDEO_FORMAT_LINEAR;
    pstViConfig->astViInfo[0].stChnInfo.enCompressMode = COMPRESS_MODE_SEG;//COMPRESS_MODE_SEG;
    s32Ret = SAMPLE_COMM_VI_SetParam(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_SetParam failed with %d!\n", s32Ret);
        return s32Ret;
    }

    SAMPLE_COMM_VI_GetFrameRateBySensor(enSnsType, &u32FrameRate);

    s32Ret = HI_MPI_ISP_GetCtrlParam(pstViConfig->astViInfo[0].stPipeInfo.aPipe[0], &stIspCtrlParam);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_ISP_GetCtrlParam failed with %d!\n", s32Ret);
        return s32Ret;
    }
    stIspCtrlParam.u32StatIntvl = u32FrameRate / 30;

    s32Ret = HI_MPI_ISP_SetCtrlParam(pstViConfig->astViInfo[0].stPipeInfo.aPipe[0], &stIspCtrlParam);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_ISP_SetCtrlParam failed with %d!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_COMM_SYS_Exit();
        SAMPLE_PRT("SAMPLE_COMM_VI_StartVi failed with %d!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

void hisample_vi_quit(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    SAMPLE_COMM_VI_StopVi(pstViConfig);
}

int hisample_vi_chn_set_mirror_flip(int vi_pipe, int vi_chn, bool mirror, bool flip)
{
    VI_CHN_ATTR_S attr;
    KLB_MEMSET(&attr, 0, sizeof(VI_CHN_ATTR_S));

    HI_S32 ret = HI_MPI_VI_GetChnAttr(vi_pipe, vi_chn, &attr);
    if (0 != ret)
    {
        KLB_LOG("HI_MPI_VI_GetChnAttr error!code:0x%x,vi pipe:%d,vi chn:%d\n", ret, vi_pipe, vi_chn);
        return ret;
    }

    attr.bMirror = mirror ? HI_TRUE : HI_FALSE;
    attr.bFlip = flip ? HI_TRUE : HI_FALSE;

    ret = HI_MPI_VI_SetChnAttr(vi_pipe, vi_chn, &attr);
    if (0 != ret)
    {
        KLB_LOG("HI_MPI_VI_SetChnAttr error!code:0x%x,vi pipe:%d,vi chn:%d\n", ret, vi_pipe, vi_chn);
    }

    return ret;
}
