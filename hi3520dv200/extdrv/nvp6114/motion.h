#ifndef _MOTION_H_
#define _MOTION_H_

extern void vdec_hi3520_viu_init(void);
extern void hi3520_init_blank_data(unsigned int ch);
extern void vdec_motion_init(void);
extern vdec_motion_area vdec_get_motion_info(unsigned int ch);
extern void vdec_motion_display(unsigned int ch, unsigned int on);
extern void vdec_motion_sensitivity(unsigned int sens[16]);
#endif
