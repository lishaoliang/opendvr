#include "hi3516dv300_inc.h"
#include "klbutil/klb_log.h"
#include "klbmem/klb_mem.h"
#include <assert.h>


int hisample_isp_set_csc(int vi_pipe, int bright, int contrast, int saturation, int hue)
{
    ISP_CSC_ATTR_S attr;
    KLB_MEMSET(&attr, 0, sizeof(ISP_CSC_ATTR_S));

    int ret = HI_MPI_ISP_GetCSCAttr(vi_pipe, &attr);
    if (0 != ret)
    {
        KLB_LOG("HI_MPI_ISP_GetCSCAttr error!code:0x%x,vi pipe:%d\n", ret, vi_pipe);
        return ret;
    }

    attr.u8Luma = (HI_U8)bright;        // 亮度 [0, 100]
    attr.u8Contr = (HI_U8)contrast;     // 对比度 [0, 100]
    attr.u8Satu = (HI_U8)saturation;    // 饱和度 [0, 100]
    attr.u8Hue = (HI_U8)hue;            // 色度 [0, 100]

    ret = HI_MPI_ISP_SetCSCAttr(vi_pipe, &attr);
    if (0 != ret)
    {
        KLB_LOG("HI_MPI_ISP_SetCSCAttr error!code:0x%x,vi pipe:%d\n", ret, vi_pipe);
    }

    return ret;
}
