#include "sample_hisi_inc.h"
#include "log/klb_log.h"
#include "mem/klb_mem.h"


int sample_hisi_venc_init(int venc_grp, int venc_id, PAYLOAD_TYPE_E payload, int resolution/*klb_resolution_e*/)
{
    int ret = SAMPLE_COMM_VENC_Start(venc_grp, venc_id, payload, VIDEO_ENCODING_MODE_NTSC, (PIC_SIZE_E)resolution, SAMPLE_RC_CBR);
    if (0 != ret)
    {
        KLB_LOG("SAMPLE_COMM_VENC_Start error!ret=0x%x\n", ret);
    }

    return 0;
}

void sample_hisi_venc_quit(int venc_grp, int venc_id)
{
    SAMPLE_COMM_VENC_Stop(venc_grp, venc_id);
}

int sample_hisi_venc_recv(int venc_id, VENC_STREAM_S* p_stream, VENC_PACK_S* p_pack/*max=KLB_MNP_PACK_MAX*/)
{
    VENC_CHN_STAT_S status;
    KLB_MEMSET(&status, 0, sizeof(VENC_CHN_STAT_S));

    int ret = HI_MPI_VENC_Query(venc_id, &status);
    if (HI_SUCCESS != ret)
    {
        KLB_LOG("HI_MPI_VENC_Query error!ret=0x%x\n", ret);
        return 1;
    }

    if (status.u32CurPacks <= 0)
    {
        return 2; // 暂无数据
    }

    p_stream->u32PackCount = MIN(status.u32CurPacks, KLB_MNP_PACK_MAX);
    ret = HI_MPI_VENC_GetStream(venc_id, p_stream, HI_FALSE);
    if (0 != ret)
    {
        KLB_LOG("HI_MPI_VENC_GetStream error!ret=0x%x\n", ret);
        return 3;
    }

    return 0;
}

int sample_hisi_venc_free(int venc_id, VENC_STREAM_S* p_stream)
{
    int ret = HI_MPI_VENC_ReleaseStream(venc_id, p_stream);
    if (HI_SUCCESS != ret)
    {
        KLB_LOG("HI_MPI_VENC_ReleaseStream error!ret=0x%x\n", ret);
        return ret;
    }

    return 0;
}
