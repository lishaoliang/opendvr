#ifndef _COAX_PROTOCOL_H_
#define _COAX_PROTOCOL_H_



extern void vdec_coaxial_select_ch(unsigned int ch);
extern void vdec_coaxial_init(void);

extern void pelco_coax_mode( void );
extern void pelco_reset( void );
extern void pelco_set( void );
extern void pelco_up( void );
extern void pelco_down( void );
extern void pelco_left( void );
extern void pelco_right( void );
extern void pelco_osd( void );
extern void pelco_iris_open( void );
extern void pelco_iris_close( void );
extern void pelco_focus_near( void );
extern void pelco_focus_far( void );
extern void pelco_zoom_wide( void );
extern void pelco_zoom_tele( void );

extern void pelco_scan_1_sr( void );
extern void pelco_scan_1_st( void );
extern void pelco_preset_1( void );
extern void pelco_preset_2( void );
extern void pelco_preset_3( void );
extern void pelco_pattern_1_sr( void );
extern void pelco_pattern_1_st( void );
extern void pelco_pattern_2_sr( void );
extern void pelco_pattern_2_st( void );
extern void pelco_pattern_3_sr( void );
extern void pelco_pattern_3_st( void );
extern void pelco_pattern_run( void );

extern void samsung_coax_mode( void );
extern void samsung_set( void );
extern void samsung_up( void );
extern void samsung_down( void );
extern void samsung_left( void );
extern void samsung_right( void );
extern void samsung_osd( void );
extern void samsung_ptz_up( void );
extern void samsung_ptz_down( void );
extern void samsung_ptz_left( void );
extern void samsung_ptz_right( void );
extern void samsung_iris_open( void );
extern void samsung_iris_close( void );
extern void samsung_focus_near( void );
extern void samsung_focus_far( void );
extern void samsung_zoom_wide( void );
extern void samsung_zoom_tele( void );

extern void samsung_scan_1_sr( void );
extern void samsung_scan_1_st( void );
extern void samsung_preset_1( void );
extern void samsung_preset_2( void );
extern void samsung_preset_3( void );
extern void samsung_pattern_1_sr( void );
extern void samsung_pattern_1_st( void );
extern void samsung_pattern_2_sr( void );
extern void samsung_pattern_2_st( void );
extern void samsung_pattern_3_sr( void );
extern void samsung_pattern_3_st( void );

#endif
