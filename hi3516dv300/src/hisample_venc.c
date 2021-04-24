#include "hi3516dv300_inc.h"
#include "klbutil/klb_log.h"
#include "klbmem/klb_mem.h"
#include "klbbase/klb_mnp_dev.h"
#include "klbbase/klb_resolution.h"
#include <assert.h>


// 修改来源 : Hi3516CV500_SDK_V2.0.2.0\smp\a7_linux\mpp\sample\venc\sample_venc.c, line : 597
// 参考: SAMPLE_VENC_H265_H264
int hisample_venc_init(int venc_id, PAYLOAD_TYPE_E payload, klb_resolution_e resolution, bool b_start_recv)
{
    SAMPLE_RC_E rc_mode = SAMPLE_RC_CBR;
    
    VENC_GOP_ATTR_S stGopAttr;
    KLB_MEMSET(&stGopAttr, 0, sizeof(VENC_GOP_ATTR_S));

    CHECK_RET(SAMPLE_COMM_VENC_GetGopAttr(VENC_GOPMODE_NORMALP, &stGopAttr), "SAMPLE_COMM_VENC_GetGopAttr");
    CHECK_RET(SAMPLE_COMM_VENC_Creat(venc_id, payload, (PIC_SIZE_E)resolution, rc_mode, 0, HI_FALSE, &stGopAttr), "SAMPLE_COMM_VENC_Creat");

    if (b_start_recv)
    {
        hisample_venc_start_recv(venc_id);
    }

    return 0;
}

void hisample_venc_quit(int venc_id)
{
    SAMPLE_COMM_VENC_Stop(venc_id);
}

int hisample_venc_start_recv(int venc_id)
{
    VENC_RECV_PIC_PARAM_S  recv_param;
    recv_param.s32RecvPicNum = -1; // 持续接收

    CHECK_RET(HI_MPI_VENC_StartRecvFrame(venc_id, &recv_param), "HI_MPI_VENC_StartRecvFrame");
}

void hisample_venc_stop_recv(int venc_id)
{
    HI_MPI_VENC_StopRecvFrame(venc_id);
}

// 修改来源 : Hi3516CV500_SDK_V2.0.2.0\smp\a7_linux\mpp\sample\common\sample_comm_venc.c, line : 1980
// 参考 : SAMPLE_COMM_VENC_GetVencStreamProc
int hisample_venc_recv(int venc_id, VENC_STREAM_S* p_stream, VENC_PACK_S* p_pack/*max=KLB_MNP_PACK_MAX*/)
{
    VENC_CHN_STATUS_S stat = { 0 };
    HI_S32 ret = HI_MPI_VENC_QueryStatus(venc_id, &stat);

    if (HI_SUCCESS != ret)
    {
        return 1; // 暂无数据
    }

    if (0 == stat.u32CurPacks)
    {
        return 1; // 暂无数据
    }

    int packs = MIN((int)stat.u32CurPacks, KLB_MNP_PACK_MAX);

    p_stream->pstPack = p_pack;
    p_stream->u32PackCount = packs;

    ret = HI_MPI_VENC_GetStream(venc_id, p_stream, 0);  // 0.非阻塞方式

    return ret; // 0.有数据; 非0.错误码
}

void hisample_venc_free(int venc_id, VENC_STREAM_S* p_stream)
{
    CHECK_RET(HI_MPI_VENC_ReleaseStream(venc_id, p_stream), "HI_MPI_VENC_ReleaseStream");
}

void hisample_venc_request_i(int venc_id)
{
    HI_S32 ret = HI_MPI_VENC_RequestIDR(venc_id, HI_FALSE);
    if (0 != ret)
    {
        KLB_LOG("HI_MPI_VENC_RequestIDR error!code:0x%x,venc id:%d\n", ret, venc_id);
    }
}

int hisample_venc_set_stream(int venc_id, int fmt, int w, int h, int frame_rate, int bitrate, int rc_mode, int quality, int interval, bool b_has_start_recv)
{
    VENC_CHN_ATTR_S attr;
    KLB_MEMSET(&attr, 0, sizeof(VENC_CHN_ATTR_S));

    HI_S32 ret = HI_MPI_VENC_GetChnAttr(venc_id, &attr);
    if (0 != ret)
    {
        KLB_LOG("HI_MPI_VENC_GetChnAttr error!code:0x%x,venc id:%d\n", ret, venc_id);
        return 1;
    }

    PAYLOAD_TYPE_E payload_type = PT_H264;
    if (attr.stVencAttr.enType != payload_type)
    {
        KLB_LOG("hisample_venc_set_stream payload_type error!old type:%d,new type:%d,venc id:%d\n", attr.stVencAttr.enType, payload_type, venc_id);
        return -1; // 编码格式不正确
    }

    bool b_reset = false;
    if (attr.stVencAttr.u32PicWidth != w || attr.stVencAttr.u32PicHeight != h)
    {
        b_reset = true;
    }

    attr.stVencAttr.enType = payload_type;
    attr.stVencAttr.u32PicWidth = w;
    attr.stVencAttr.u32PicHeight = h;

    if (PT_H265 == attr.stVencAttr.enType)
    {
        // H265
        attr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
        if (VENC_RC_MODE_H265CBR == attr.stRcAttr.enRcMode)
        {
            attr.stRcAttr.stH265Cbr.u32Gop = interval;
            attr.stRcAttr.stH265Cbr.fr32DstFrameRate = frame_rate;
            attr.stRcAttr.stH265Cbr.u32BitRate = bitrate;
        }
        else
        {
            attr.stRcAttr.stH265Vbr.u32Gop = interval;
            attr.stRcAttr.stH265Vbr.fr32DstFrameRate = frame_rate;
            attr.stRcAttr.stH265Vbr.u32MaxBitRate = bitrate;
        }
    }
    else
    {
        // H264
        attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
        if (VENC_RC_MODE_H264CBR == attr.stRcAttr.enRcMode)
        {
            attr.stRcAttr.stH264Cbr.u32Gop = interval;
            attr.stRcAttr.stH264Cbr.fr32DstFrameRate = frame_rate;
            attr.stRcAttr.stH264Cbr.u32BitRate = bitrate;
        }
        else
        {
            attr.stRcAttr.stH264Vbr.u32Gop = interval;
            attr.stRcAttr.stH264Vbr.fr32DstFrameRate = frame_rate;
            attr.stRcAttr.stH264Vbr.u32MaxBitRate = bitrate;
        }
    }

    // 图像质量 [1,10] = >[-10, 30]
    attr.stGopAttr.stNormalP.s32IPQpDelta = -10 + 40 * quality / 10;

    if (b_reset)
    {
        // 修改了分辨率, 需要做如下处理
        if (b_has_start_recv)
        {
            // 已经开启的接收流, 则需要先关闭, 再重新打开
            HI_MPI_VENC_StopRecvFrame(venc_id);
            HI_MPI_VENC_ResetChn(venc_id);
        }

        ret = HI_MPI_VENC_SetChnAttr(venc_id, &attr);
        if (0 != ret)
        {
            KLB_LOG("HI_MPI_VENC_SetChnAttr error!code:0x%x,venc id:%d\n", ret, venc_id);
        }

        if (b_has_start_recv)
        {
            VENC_RECV_PIC_PARAM_S  recv_param;
            recv_param.s32RecvPicNum = -1;
            HI_MPI_VENC_StartRecvFrame(venc_id, &recv_param);
        }
    }
    else
    {
        ret = HI_MPI_VENC_SetChnAttr(venc_id, &attr);
        if (0 != ret)
        {
            KLB_LOG("HI_MPI_VENC_SetChnAttr error!code:0x%x,venc id:%d\n", ret, venc_id);
        }
    }

    HI_MPI_VENC_RequestIDR(venc_id, HI_FALSE);

    return 0;
}

int hisample_venc_set_stream_pic(int venc_id, int fmt, int w, int h, int quality)
{
    VENC_CHN_ATTR_S attr;
    KLB_MEMSET(&attr, 0, sizeof(VENC_CHN_ATTR_S));

    HI_S32 ret = HI_MPI_VENC_GetChnAttr(venc_id, &attr);
    if (0 != ret)
    {
        KLB_LOG("HI_MPI_VENC_GetChnAttr error!code:0x%x,venc id:%d\n", ret, venc_id);
        return ret;
    }

    bool b_reset = false;
    if (attr.stVencAttr.u32PicWidth != w || attr.stVencAttr.u32PicHeight != h)
    {
        b_reset = true;
    }

    attr.stVencAttr.u32PicWidth = w;
    attr.stVencAttr.u32PicHeight = h;

    // 图像质量 [1,10] = >[-10, 30]
    attr.stGopAttr.stNormalP.s32IPQpDelta = -10 + 40 * quality / 10;

    if (b_reset)
    {
        // 修改了分辨率, 需要做如下处理
        HI_MPI_VENC_StopRecvFrame(venc_id);
        HI_MPI_VENC_ResetChn(venc_id);

        ret = HI_MPI_VENC_SetChnAttr(venc_id, &attr);
        if (0 != ret)
        {
            KLB_LOG("HI_MPI_VENC_SetChnAttr error!code:0x%x,venc id:%d\n", ret, venc_id);
        }

        VENC_RECV_PIC_PARAM_S  recv_param;
        recv_param.s32RecvPicNum = -1;
        HI_MPI_VENC_StartRecvFrame(venc_id, &recv_param);
    }
    else
    {
        ret = HI_MPI_VENC_SetChnAttr(venc_id, &attr);
        if (0 != ret)
        {
            KLB_LOG("HI_MPI_VENC_SetChnAttr error!code:0x%x,venc id:%d\n", ret, venc_id);
        }
    }

    return ret;
}
