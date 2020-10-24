#include "dopi16ev2_main.h"
#include "mem/klb_mem.h"
#include "log/klb_log.h"


/* extern */dopi16ev2_main_t* g_dopi16ev2_main = NULL;


/// @brief 初始化
/// @param [in] *p_json_param    json格式的参数
/// @return int 0.成功; 非0.错误码
int dopi16ev2_main_init(const char* p_json_param)
{
    KLB_LOG("dopi16ev2_main_init...\n");

    assert(NULL == g_dopi16ev2_main);
    g_dopi16ev2_main = KLB_MALLOC(dopi16ev2_main_t, 1, 0);
    KLB_MEMSET(g_dopi16ev2_main, 0, sizeof(dopi16ev2_main_t));

    // vb

    // vi 

    // vpss

    // vi bind vpss

    // venc

    // venc bind vpss


    KLB_LOG("dopi16ev2_main_init over...\n");
    return 0;
}


/// @brief 启动线程, 开始运行
/// @param [in] *p_json_param    json格式的参数
/// @return int 0.成功; 非0.错误码
int dopi16ev2_main_start(const char* p_json_param)
{

    return 0;
}


/// @brief 关闭线程
/// @return 无
void dopi16ev2_main_stop()
{

}


/// @brief 退出
/// @return 无
void dopi16ev2_main_quit()
{
    KLB_LOG("dopi16ev2_main_quit...\n");


    KLB_FREE(g_dopi16ev2_main);
    KLB_LOG("dopi16ev2_main_quit over...\n");
}

//////////////////////////////////////////////////////////////////////////

void dopi16ev2_main_set_stream_receiver(int index, klb_mnp_stream_cb cb_stream)
{

}
