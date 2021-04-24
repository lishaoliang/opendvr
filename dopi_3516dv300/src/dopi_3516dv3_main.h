#ifndef __DOPI_3516DV3_MAIN_H__
#define __DOPI_3516DV3_MAIN_H__

#include "klb_type.h"
#include "klbbase/klb_mnp.h"
#include "klbplatform/klb_thread.h"
#include "hi3516dv300_inc.h"


#define DOPI_3516DV3_VI_PIPE_0          0
#define DOPI_3516DV3_VI_CHN_0           0


#define DOPI_3516DV3_VPSS_GRP_0         0


#define DOPI_3516DV3_VPSS_CHN_0         0

#define DOPI_3516DV3_VENC_0             0


typedef struct dopi_3516dv3_main_t_
{
    int     sns_type;
    int     sns_w;
    int     sns_h;

    SAMPLE_VI_CONFIG_S      vi_cfg;
    hisample_vpss_config_t  vpss_cfg;
}dopi_3516dv3_main_t;

extern dopi_3516dv3_main_t* g_dopi_3516dv3_main;


/// @brief 初始化
/// @param [in] *p_json_param    json格式的参数
/// @return int 0.成功; 非0.错误码
int dopi_3516dv3_main_init(const char* p_json_param);


/// @brief 启动线程, 开始运行
/// @param [in] *p_json_param    json格式的参数
/// @return int 0.成功; 非0.错误码
int dopi_3516dv3_main_start(const char* p_json_param);


/// @brief 关闭线程
/// @return 无
void dopi_3516dv3_main_stop();


/// @brief 退出
/// @return 无
void dopi_3516dv3_main_quit();


void dopi_3516dv3_main_set_stream_receiver(int index, klb_mnp_stream_cb cb_stream);


#endif // __DOPI_3516DV3_MAIN_H__
//end
