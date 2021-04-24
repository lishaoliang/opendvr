#ifndef __HI3516DV300_DEFINE_H__
#define __HI3516DV300_DEFINE_H__

#include "klb_type.h"
#include "klbbase/klb_resolution.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define __USE_KLB_RESOLUTION__      1

#define SENSOR0_TYPE    SONY_IMX307_2L_MIPI_2M_30FPS_12BIT
#define SENSOR1_TYPE    SONY_IMX307_2L_MIPI_2M_30FPS_12BIT


/// @brief 图像参考码率
/// @param [in] resolution      尺寸类型: klb_resolution_e
/// @param [in] frame_rate      帧率
/// @return int 参考码流kbps
int hi3516dv300_resolution_bit_rate(int resolution/*klb_resolution_e*/, int frame_rate);


#ifdef __cplusplus
}
#endif

#endif // __HI3516DV300_DEFINE_H__
//end
