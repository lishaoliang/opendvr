#include "klb_type.h"
#include "mnp/klb_mnp_dev.h"
#include "dopi16ev2_main.h"


/// @brief 获取库文件版本
static uint32_t dopi16ev2_version()
{
    return 10101;
}

/// @brief 获取项目代号
static const char* dopi16ev2_project()
{
    return "dopi16ev2_hi3516ev200_nvp6134c";
}

KLB_API int klb_mnp_dev_open(uint32_t version, void* p_interface, int interface_size)
{
    klb_mnp_dev_interface_t* p_dev = (klb_mnp_dev_interface_t*)p_interface;

    p_dev->version = dopi16ev2_version;
    p_dev->project = dopi16ev2_project;

    p_dev->init = dopi16ev2_main_init;
    p_dev->start = dopi16ev2_main_start;
    p_dev->stop = dopi16ev2_main_stop;
    p_dev->quit = dopi16ev2_main_quit;

    p_dev->set_stream_receiver = dopi16ev2_main_set_stream_receiver;

    return 0;
}
