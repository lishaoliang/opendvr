#include "hi3516dv300_inc.h"
#include "klbutil/klb_log.h"
#include <assert.h>


int hisample_get_wh_by_sns(SAMPLE_SNS_TYPE_E sns_type, int* p_w, int* p_h)
{
    assert(NULL != p_w);
    assert(NULL != p_h);

    PIC_SIZE_E pic_size = (PIC_SIZE_E)KLB_RESOLUTION_1920x1080;
    *p_w = 1920;
    *p_h = 1080;

    CHECK_RET(SAMPLE_COMM_VI_GetSizeBySensor(sns_type, &pic_size), "SAMPLE_COMM_VI_GetSizeBySensor");

    SIZE_S sns_size = { 0 };
    CHECK_RET(SAMPLE_COMM_SYS_GetPicSize(pic_size, &sns_size), "SAMPLE_COMM_SYS_GetPicSize");

    *p_w = sns_size.u32Width;
    *p_h = sns_size.u32Height;

    return 0;
}

uint64_t hisample_calculate_block_size(uint32_t w, uint32_t h)
{
    return COMMON_GetPicBufferSize(w, h, PIXEL_FORMAT_YVU_SEMIPLANAR_422, DATA_BITWIDTH_8, COMPRESS_MODE_SEG, DEFAULT_ALIGN);
}


uint64_t hisample_calculate_block_size_by_sns(SAMPLE_SNS_TYPE_E sns_type)
{
    PIC_SIZE_E pic_size = (PIC_SIZE_E)KLB_RESOLUTION_1920x1080;
    CHECK_RET(SAMPLE_COMM_VI_GetSizeBySensor(sns_type, &pic_size), "SAMPLE_COMM_VI_GetSizeBySensor");

    SIZE_S sns_size = { 0 };
    CHECK_RET(SAMPLE_COMM_SYS_GetPicSize(pic_size, &sns_size), "SAMPLE_COMM_SYS_GetPicSize");

    return COMMON_GetPicBufferSize(sns_size.u32Width, sns_size.u32Height, PIXEL_FORMAT_YVU_SEMIPLANAR_422, DATA_BITWIDTH_8, COMPRESS_MODE_SEG, DEFAULT_ALIGN);
}

void hisample_print_vb_config(const VB_CONFIG_S* p_vb_cfg)
{
    for (int i = 0; i < p_vb_cfg->u32MaxPoolCnt; i++)
    {
        uint64_t total = (uint64_t)p_vb_cfg->astCommPool[i].u64BlkSize * p_vb_cfg->astCommPool[i].u32BlkCnt;
        total = total / 1048576; // M
        KLB_LOG("mmz pool idx:[%d],block count:[%u],block size:[%lluK],total:[%lluM]\n", i, p_vb_cfg->astCommPool[i].u32BlkCnt, p_vb_cfg->astCommPool[i].u64BlkSize / 1024, total);
    }
}

int hisample_mmz_alloc(HI_U64* pu64PhyAddr, HI_VOID** ppVirAddr, uint32_t size)
{
    HI_S32 ret = HI_MPI_SYS_MmzAlloc(pu64PhyAddr, ppVirAddr, "User", NULL, size);
    if (0 != ret)
    {
        KLB_LOG("HI_MPI_SYS_MmzAlloc error!ret:[%x]\n", ret);
    }

    return ret;
}

int hisample_mmz_free(HI_U64 u64PhyAddr, HI_VOID* pVirAddr)
{
    HI_S32 ret = HI_MPI_SYS_MmzFree(u64PhyAddr, pVirAddr);
    if (0 != ret)
    {
        KLB_LOG("HI_MPI_SYS_MmzFree error!ret:[%x]\n", ret);
    }

    return ret;
}
