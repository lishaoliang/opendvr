#include "klb_type.h"
#include "mnp/klb_mnp_dev.h"
#include "ahb7004t_main.h"


/// @brief 获取库文件版本
static uint32_t ahb7004t_version()
{
    return 10101;
}

/// @brief 获取项目代号
static const char* ahb7004t_project()
{
    return "ahb7004t_hi3520dv200_nvp6134c";
}

KLB_API int klb_mnp_dev_open(uint32_t version, void* p_interface, int interface_size)
{
    klb_mnp_dev_interface_t* p_dev = (klb_mnp_dev_interface_t*)p_interface;

    p_dev->version = ahb7004t_version;
    p_dev->project = ahb7004t_project;

    p_dev->init = ahb7004t_main_init;
    p_dev->start = ahb7004t_main_start;
    p_dev->stop = ahb7004t_main_stop;
    p_dev->quit = ahb7004t_main_quit;

    p_dev->set_stream_receiver = ahb7004t_main_set_stream_receiver;

    return 0;
}
