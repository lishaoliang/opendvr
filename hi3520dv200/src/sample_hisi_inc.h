#ifndef __SAMPLE_HISI_INC_H__
#define __SAMPLE_HISI_INC_H__


#include "klb_type.h"
#include "image/klb_resolution.h"
#include "mnp/klb_mnp.h"
#include "mnp/klb_mnp_dev.h"
#include <stdio.h>
#include "sample_comm.h"

#if defined(__cplusplus)
extern "C" {
#endif


//////////////////////////////////////////////////////////////////////////
// sys

uint64_t sample_hisi_calculate_block_size(uint32_t w, uint32_t h);
void sample_hisi_report_vb_config(const VB_CONF_S* p_vb_config);


//////////////////////////////////////////////////////////////////////////
// vi
int sample_hisi_vi_init();
void sample_hisi_vi_quit();
int sample_hisi_vi_bind_vpss();
void sample_hisi_vi_unbind_vpss();


//////////////////////////////////////////////////////////////////////////
// vpss

int sample_hisi_vpss_init(int w, int h);
void sample_hisi_vpss_quit();


//////////////////////////////////////////////////////////////////////////
// venc
int sample_hisi_venc_init(int venc_grp, int venc_id, PAYLOAD_TYPE_E payload, int resolution/*klb_resolution_e*/);
void sample_hisi_venc_quit(int venc_grp, int venc_id);

int sample_hisi_venc_recv(int venc_id, VENC_STREAM_S* p_stream, VENC_PACK_S* p_pack/*max=KLB_MNP_PACK_MAX*/);
int sample_hisi_venc_free(int venc_id, VENC_STREAM_S* p_stream);

//////////////////////////////////////////////////////////////////////////
// vdec



#ifdef __cplusplus
}
#endif

#endif // __SAMPLE_HISI_INC_H__
//end
