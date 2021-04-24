#include "dopi_3516dv3_main.h"
#include "klbmem/klb_mem.h"
#include "klbutil/klb_log.h"
#include <assert.h>


/* extern */dopi_3516dv3_main_t* g_dopi_3516dv3_main = NULL;


/// @brief 初始化
/// @param [in] *p_json_param    json格式的参数
/// @return int 0.成功; 非0.错误码
int dopi_3516dv3_main_init(const char* p_json_param)
{
    KLB_LOG("dopi_3516dv3_main_init...\n");

    assert(NULL == g_dopi_3516dv3_main);
    g_dopi_3516dv3_main = KLB_MALLOC(dopi_3516dv3_main_t, 1, 0);
    KLB_MEMSET(g_dopi_3516dv3_main, 0, sizeof(dopi_3516dv3_main_t));

    // sns w,h
    hisample_get_wh_by_sns(SENSOR0_TYPE, &g_dopi_3516dv3_main->sns_w, &g_dopi_3516dv3_main->sns_h);
    KLB_LOG("hisample_get_wh_by_sns,SENSOR0_TYPE:[%d],wh:[%d,%d]\n", SENSOR0_TYPE, g_dopi_3516dv3_main->sns_w, g_dopi_3516dv3_main->sns_h);


    // mmz max = 256M
    // 初始化 vb 池
    VB_CONFIG_S vb_config = { 0 };
    vb_config.u32MaxPoolCnt = 2;

    vb_config.astCommPool[0].u64BlkSize = hisample_calculate_block_size_by_sns(SENSOR0_TYPE); //
    vb_config.astCommPool[0].u32BlkCnt = 16;

    vb_config.astCommPool[1].u64BlkSize = hisample_calculate_block_size(960, 540); //
    vb_config.astCommPool[1].u32BlkCnt = 16;

    HI_S32 ret = SAMPLE_COMM_SYS_Init(&vb_config);
    KLB_LOG("SAMPLE_COMM_SYS_Init ret=%d\n", ret);

    hisample_print_vb_config(&vb_config);


    // 初始化 vi 
    SAMPLE_VI_CONFIG_S* p_vi_cfg = &g_dopi_3516dv3_main->vi_cfg;
    SAMPLE_COMM_VI_GetSensorInfo(p_vi_cfg);

    p_vi_cfg->s32WorkingViNum = 1;
    p_vi_cfg->astViInfo[0].stDevInfo.ViDev = 0; // 0 号vi
    p_vi_cfg->astViInfo[0].stPipeInfo.aPipe[0] = DOPI_3516DV3_VI_PIPE_0;
    p_vi_cfg->astViInfo[0].stChnInfo.ViChn = DOPI_3516DV3_VI_CHN_0;
    p_vi_cfg->astViInfo[0].stChnInfo.enDynamicRange = DYNAMIC_RANGE_SDR8;
    p_vi_cfg->astViInfo[0].stChnInfo.enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;

    ret = hisample_vi_init(p_vi_cfg, HI_FALSE);
    KLB_LOG("hisample_vi_init ret=%d\n", ret);

    // 初始化 vpss
    hisample_vpss_config_t* p_vpss_cfg = &g_dopi_3516dv3_main->vpss_cfg;
    p_vpss_cfg->grp_id = DOPI_3516DV3_VPSS_GRP_0;
    p_vpss_cfg->grp_w = g_dopi_3516dv3_main->sns_w;
    p_vpss_cfg->grp_h = g_dopi_3516dv3_main->sns_h;

    p_vpss_cfg->vpss_chn[DOPI_3516DV3_VPSS_CHN_0].w = g_dopi_3516dv3_main->sns_w; // 1920;
    p_vpss_cfg->vpss_chn[DOPI_3516DV3_VPSS_CHN_0].h = g_dopi_3516dv3_main->sns_h; // 1080;

    ret = hisample_vpss_init(p_vpss_cfg, DYNAMIC_RANGE_SDR8, PIXEL_FORMAT_YVU_SEMIPLANAR_420);
    KLB_LOG("hisample_vpss_init ret=%d\n", ret);

    ret = SAMPLE_COMM_VI_Bind_VPSS(DOPI_3516DV3_VI_PIPE_0, DOPI_3516DV3_VI_CHN_0, DOPI_3516DV3_VPSS_GRP_0);
    KLB_LOG("SAMPLE_COMM_VI_Bind_VPSS ret=%d\n", ret);


    // 初始化 venc
    ret = hisample_venc_init(DOPI_3516DV3_VENC_0, PT_H264, KLB_RESOLUTION_1920x1080, true);
    KLB_LOG("hisample_venc_init ret=%d\n", ret);

    ret = SAMPLE_COMM_VPSS_Bind_VENC(DOPI_3516DV3_VPSS_GRP_0, DOPI_3516DV3_VPSS_CHN_0, DOPI_3516DV3_VENC_0);
    KLB_LOG("SAMPLE_COMM_VPSS_Bind_VENC ret=%d\n", ret);



    KLB_LOG("dopi_3516dv3_main_init over...\n");
    return 0;
}

/// @brief 启动线程, 开始运行
/// @param [in] *p_json_param    json格式的参数
/// @return int 0.成功; 非0.错误码
int dopi_3516dv3_main_start(const char* p_json_param)
{
    assert(NULL != g_dopi_3516dv3_main);


    return 0;
}


/// @brief 关闭线程
/// @return 无
void dopi_3516dv3_main_stop()
{
    assert(NULL != g_dopi_3516dv3_main);
}


/// @brief 退出
/// @return 无
void dopi_3516dv3_main_quit()
{
    KLB_LOG("dopi_3516dv3_main_quit...\n");

    // 退出

    // 解绑定
    SAMPLE_COMM_VPSS_UnBind_VENC(DOPI_3516DV3_VPSS_GRP_0, DOPI_3516DV3_VPSS_CHN_0, DOPI_3516DV3_VENC_0);
    SAMPLE_COMM_VI_UnBind_VPSS(DOPI_3516DV3_VI_PIPE_0, DOPI_3516DV3_VI_CHN_0, DOPI_3516DV3_VPSS_GRP_0);

    hisample_venc_quit(DOPI_3516DV3_VENC_0);                // venc 0
    hisample_vpss_quit(&g_dopi_3516dv3_main->vpss_cfg);     // vpss
    hisample_vi_quit(&g_dopi_3516dv3_main->vi_cfg);         // vi 

    SAMPLE_COMM_SYS_Exit();

    KLB_FREE(g_dopi_3516dv3_main);
    KLB_LOG("dopi_3516dv3_main_quit over...\n");
}

//////////////////////////////////////////////////////////////////////////

void dopi_3516dv3_main_set_stream_receiver(int index, klb_mnp_stream_cb cb_stream)
{

}
