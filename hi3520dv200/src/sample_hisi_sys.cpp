#include "sample_hisi_inc.h"
#include "log/klb_log.h"


uint64_t sample_hisi_calculate_block_size(uint32_t w, uint32_t h)
{
    HI_S32 s32Ret = HI_FAILURE;
    SIZE_S stSize;
    stSize.u32Width = w;
    stSize.u32Height = h;

    PIXEL_FORMAT_E enPixFmt = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    if (PIXEL_FORMAT_YUV_SEMIPLANAR_422 != enPixFmt && PIXEL_FORMAT_YUV_SEMIPLANAR_420 != enPixFmt)
    {
        KLB_LOG("pixel format[%d] input failed!\n", enPixFmt);
        return HI_FAILURE;
    }

    int u32AlignWidth = 64;

    if (16 != u32AlignWidth && 32 != u32AlignWidth && 64 != u32AlignWidth)
    {
        KLB_LOG("system align width[%d] input failed!\n", \
            u32AlignWidth);
        return HI_FAILURE;
    }
    if (704 == stSize.u32Width)
    {
        stSize.u32Width = 720;
    }

    KLB_LOG("w:%d, u32AlignWidth:%d\n", CEILING_2_POWER(stSize.u32Width, u32AlignWidth), u32AlignWidth);
    return (CEILING_2_POWER(stSize.u32Width, u32AlignWidth) * \
        CEILING_2_POWER(stSize.u32Height, u32AlignWidth) * \
        ((PIXEL_FORMAT_YUV_SEMIPLANAR_422 == enPixFmt) ? 2 : 1.5));
}

void sample_hisi_report_vb_config(const VB_CONF_S* p_vb_config)
{
    for (int i = 0; i < p_vb_config->u32MaxPoolCnt; i++)
    {
        uint64_t total = (uint64_t)p_vb_config->astCommPool[i].u32BlkSize * p_vb_config->astCommPool[i].u32BlkCnt;
        total = total / 1048576; // M
        KLB_LOG("pool index:[%u],block size:[%u],block count:[%u],total:[%lluM]\n", i, p_vb_config->astCommPool[i].u32BlkSize, p_vb_config->astCommPool[i].u32BlkCnt, total);
    }
}
