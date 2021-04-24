#ifndef __HI3516DV300_INC_H__
#define __HI3516DV300_INC_H__


#include "klb_type.h"
#include "sample_comm.h"
#include "klbbase/klb_mnp_stream.h"
#include "klbbase/klb_resolution.h"


//////////////////////////////////////////////////////////////////////////
// 

int hisample_get_wh_by_sns(SAMPLE_SNS_TYPE_E sns_type, int* p_w, int* p_h);
uint64_t hisample_calculate_block_size(uint32_t w, uint32_t h);
uint64_t hisample_calculate_block_size_by_sns(SAMPLE_SNS_TYPE_E sns_type);


/// @brief 打印vb配置
/// @param [in] *p_vb_cfg       vb配置
/// @return 无
void hisample_print_vb_config(const VB_CONFIG_S* p_vb_cfg);

int hisample_mmz_alloc(HI_U64* pu64PhyAddr, HI_VOID** ppVirAddr, uint32_t size);
int hisample_mmz_free(HI_U64 u64PhyAddr, HI_VOID* pVirAddr);

//////////////////////////////////////////////////////////////////////////
// isp

/// @brief 色彩: 亮度, 对比度, 饱和度, 色度
int hisample_isp_set_csc(int vi_pipe, int bright, int contrast, int saturation, int hue);


//////////////////////////////////////////////////////////////////////////
// vi

/// @brief 初始化vi
/// @param [in]  *pstViConfig       vi配置
/// @param [in]  bLowDelay          HI_TRUE.VPSS在线模式; HI_FALSE.VPSS离线模式
/// @return int 0.成功; 非0.失败
///  vi配置参数参考: Hi3516CV500_SDK_V2.0.2.0\smp\a7_linux\mpp\sample\venc\sample_venc.c, line 546
///  初始化SAMPLE_VENC_VI_Init函数的参数
int hisample_vi_init(SAMPLE_VI_CONFIG_S* pstViConfig, HI_BOOL bLowDelay);

/// @brief 退出vi
/// @param [in]  *pstViConfig       vi配置
/// @return 无
void hisample_vi_quit(SAMPLE_VI_CONFIG_S* pstViConfig);

// 镜像,翻转
int hisample_vi_chn_set_mirror_flip(int vi_pipe, int vi_chn, bool mirror, bool flip);


//////////////////////////////////////////////////////////////////////////
// VPSS

typedef struct hisample_vpss_config_t_
{
    int         grp_id;         ///< vpss组
    int         grp_w;          ///< vpss组,宽
    int         grp_h;          ///< vpss组,高

    struct
    {
        int     w;              ///< vpss通道,宽
        int     h;              ///< vpss通道,高
    }vpss_chn[VPSS_MAX_PHY_CHN_NUM];
}hisample_vpss_config_t;

int hisample_vpss_init(hisample_vpss_config_t* p_vpss_cfg, DYNAMIC_RANGE_E enDynamicRange, PIXEL_FORMAT_E enPixelFormat);
void hisample_vpss_quit(hisample_vpss_config_t* p_vpss_cfg);

// 深度
int hisample_vpss_set_depth(int vpss_grp, int vpss_chn, int depth/*[0,8]*/);


//////////////////////////////////////////////////////////////////////////
// VENC

int hisample_venc_init(int venc_id, PAYLOAD_TYPE_E payload, klb_resolution_e resolution, bool b_start_recv);
void hisample_venc_quit(int venc_id);

int hisample_venc_start_recv(int venc_id);
void hisample_venc_stop_recv(int venc_id);

int hisample_venc_recv(int venc_id, VENC_STREAM_S* p_stream, VENC_PACK_S* p_pack/*max=KLB_MNP_PACK_MAX*/);
void hisample_venc_free(int venc_id, VENC_STREAM_S* p_stream);

// 立即关键帧
void hisample_venc_request_i(int venc_id);
int hisample_venc_set_stream(int venc_id, int fmt, int w, int h, int frame_rate, int bitrate, int rc_mode, int quality, int interval, bool b_has_start_recv);
int hisample_venc_set_stream_pic(int venc_id, int fmt, int w, int h, int quality);


#endif // __HI3516DV300_INC_H__
//end
