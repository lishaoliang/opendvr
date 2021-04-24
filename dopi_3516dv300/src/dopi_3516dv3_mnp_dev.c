#include "klb_type.h"
#include "klbbase/klb_mnp_dev.h"
#include "dopi_3516dv3_main.h"


/// @brief 获取库文件版本
static uint32_t dopi_3516dv3_version()
{
    return 10101;
}

/// @brief 获取项目代号
static const char* dopi_3516dv3_project()
{
    return "dopi_3516dv3_hi3516dv300_imx307";
}

KLB_API int klb_mnp_dev_open(uint32_t version, void* p_interface, int interface_size)
{
    klb_mnp_dev_interface_t* p_dev = (klb_mnp_dev_interface_t*)p_interface;

    p_dev->version = dopi_3516dv3_version;
    p_dev->project = dopi_3516dv3_project;

    p_dev->init = dopi_3516dv3_main_init;
    p_dev->start = dopi_3516dv3_main_start;
    p_dev->stop = dopi_3516dv3_main_stop;
    p_dev->quit = dopi_3516dv3_main_quit;

    p_dev->set_stream_receiver = dopi_3516dv3_main_set_stream_receiver;

    return 0;
}
