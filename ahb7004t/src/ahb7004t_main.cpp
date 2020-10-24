#include "ahb7004t_main.h"
#include "mem/klb_mem.h"
#include "log/klb_log.h"


/* extern */ahb7004t_main_t* g_ahb7004t_main = NULL;


/// @brief 初始化
/// @param [in] *p_json_param    json格式的参数
/// @return int 0.成功; 非0.错误码
int ahb7004t_main_init(const char* p_json_param)
{
    KLB_LOG("ahb7004t_main_init...\n");

    assert(NULL == g_ahb7004t_main);
    g_ahb7004t_main = KLB_MALLOC(ahb7004t_main_t, 1, 0);
    KLB_MEMSET(g_ahb7004t_main, 0, sizeof(ahb7004t_main_t));

    // vb
    VB_CONF_S vb_conf = { 0 };
    vb_conf.u32MaxPoolCnt = 2;

    vb_conf.astCommPool[0].u32BlkSize = sample_hisi_calculate_block_size(960, 540);  //
    vb_conf.astCommPool[0].u32BlkCnt = 15;

    vb_conf.astCommPool[1].u32BlkSize = sample_hisi_calculate_block_size(480, 272);  //
    vb_conf.astCommPool[1].u32BlkCnt = 15;

    SAMPLE_COMM_SYS_Init(&vb_conf);
    sample_hisi_report_vb_config(&vb_conf);

    // vi 
    sample_hisi_vi_init();

    // vpss
    sample_hisi_vpss_init(960, 540);

    // vi bind vpss

    // venc
    sample_hisi_venc_init(0, AHB7004T_VENC_0_V1, PT_H264, KLB_RESOLUTION_960x540);

    // venc bind vpss


    KLB_LOG("ahb7004t_main_init over...\n");
    return 0;
}


/// @brief 启动线程, 开始运行
/// @param [in] *p_json_param    json格式的参数
/// @return int 0.成功; 非0.错误码
int ahb7004t_main_start(const char* p_json_param)
{

    return 0;
}


/// @brief 关闭线程
/// @return 无
void ahb7004t_main_stop()
{

}


/// @brief 退出
/// @return 无
void ahb7004t_main_quit()
{
    KLB_LOG("ahb7004t_main_quit...\n");

    sample_hisi_venc_quit(0, AHB7004T_VENC_0_V1);

    sample_hisi_vpss_quit();
    sample_hisi_vi_quit();

    SAMPLE_COMM_SYS_Exit();
    KLB_FREE(g_ahb7004t_main);

    KLB_LOG("ahb7004t_main_quit over...\n");
}

//////////////////////////////////////////////////////////////////////////

void ahb7004t_main_set_stream_receiver(int index, klb_mnp_stream_cb cb_stream)
{

}
