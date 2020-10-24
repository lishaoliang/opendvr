#ifndef __DOPI16EV2_MAIN_H__
#define __DOPI16EV2_MAIN_H__

#include "klb_type.h"
#include "sample_hisi_inc.h"


#define DOPI16EV2_VENC_0_V1      0


typedef struct dopi16ev2_main_t_
{
    int     ad_type;
    int     ad_w;
    int     ad_h;
}dopi16ev2_main_t;


extern dopi16ev2_main_t* g_dopi16ev2_main;


/// @brief 初始化
/// @param [in] *p_json_param    json格式的参数
/// @return int 0.成功; 非0.错误码
int dopi16ev2_main_init(const char* p_json_param);


/// @brief 启动线程, 开始运行
/// @param [in] *p_json_param    json格式的参数
/// @return int 0.成功; 非0.错误码
int dopi16ev2_main_start(const char* p_json_param);


/// @brief 关闭线程
/// @return 无
void dopi16ev2_main_stop();


/// @brief 退出
/// @return 无
void dopi16ev2_main_quit();


void dopi16ev2_main_set_stream_receiver(int index, klb_mnp_stream_cb cb_stream);


#endif // __DOPI16EV2_MAIN_H__
//end
