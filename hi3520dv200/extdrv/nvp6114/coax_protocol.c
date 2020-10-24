/*************************************************************************
        Project : AWB Mapping One Block Program
        File Name: nvp1104.c
        Compiler: AVR IAR Compiler
        Author  : NEXTCHIP(c) cshong
        Data    : 2008.04.25
        Version : 1.00
        Dicription

        ----------------------------------------------------------
            Copyright(c) 2001 by Black_List All Rights Reserved

**************************************************************************
	Header file include
**************************************************************************/
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/list.h>
#include <asm/delay.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>

#include "gpio_i2c.h"
#include "nvp6114.h"
#include "common.h"

/*************************************************************************
	Global Variables Definition
*************************************************************************/
unsigned char g_system = 1;
unsigned char g_coax_ch=0;
/*************************************************************************
	SAMSUNG PROTOCOL Regiter set value Definition
*************************************************************************/
										 //  0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F
unsigned char SAM_SET_Buf[] 		={ /*1*/ 0xAA,0x1C,0x18,0xFF,0xAA,0x3C,0xFF,0xFF,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_OSD_Buf[] 		={ /*1*/ 0xAA,0x1C,0x17,0x01,0xAA,0x3C,0xFF,0xFF,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_UP_Buf[] 			={ /*1*/ 0xAA,0x1C,0x03,0xFF,0xAA,0x3C,0xFF,0xFF,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_DOWN_Buf[] 		={ /*1*/ 0xAA,0x1C,0x09,0xFF,0xAA,0x3C,0xFF,0xFF,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_LEFT_Buf[] 		={ /*1*/ 0xAA,0x1C,0x05,0xFF,0xAA,0x3C,0xFF,0xFF,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_RIGHT_Buf[] 		={ /*1*/ 0xAA,0x1C,0x07,0xFF,0xAA,0x3C,0xFF,0xFF,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PTZ_UP_Buf[] 		={ /*1*/ 0xAA,0x1B,0x00,0x04,0xAA,0x3B,0x00,0x1F,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PTZ_DOWN_Buf[] 	={ /*1*/ 0xAA,0x1B,0x00,0x08,0xAA,0x3B,0x00,0x1F,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PTZ_LEFT_Buf[] 	={ /*1*/ 0xAA,0x1B,0x00,0x01,0xAA,0x3B,0x1F,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PTZ_RIGHT_Buf[] 	={ /*1*/ 0xAA,0x1B,0x00,0x02,0xAA,0x3B,0x1F,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_IRIS_OPEN_Buf[] 	={ /*1*/ 0xAA,0x1B,0x08,0x00,0xAA,0x3B,0x00,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_IRIS_CLOSE_Buf[]	={ /*1*/ 0xAA,0x1B,0x10,0x00,0xAA,0x3B,0x00,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_FOCUS_NEAR_Buf[]	={ /*1*/ 0xAA,0x1B,0x02,0x00,0xAA,0x3B,0x00,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_FOCUS_FAR_Buf[] 	={ /*1*/ 0xAA,0x1B,0x01,0x00,0xAA,0x3B,0x00,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_ZOOM_WIDE_Buf[] 	={ /*1*/ 0xAA,0x1B,0x40,0x00,0xAA,0x3B,0x00,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_ZOOM_TELE_Buf[] 	={ /*1*/ 0xAA,0x1B,0x20,0x00,0xAA,0x3B,0x00,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_SCAN_1SR_Buf[] 	={ /*1*/ 0xAA,0x1C,0x13,0x01,0xAA,0x3C,0x01,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_SCAN_1ST_Buf[] 	={ /*1*/ 0xAA,0x1C,0x13,0x00,0xAA,0x3C,0x01,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PRESET1_Buf[] 	={ /*1*/ 0xAA,0x1C,0x19,0x01,0xAA,0x3C,0x00,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PRESET2_Buf[] 	={ /*1*/ 0xAA,0x1C,0x19,0x02,0xAA,0x3C,0x00,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PRESET3_Buf[] 	={ /*1*/ 0xAA,0x1C,0x19,0x03,0xAA,0x3C,0x00,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PATTERN_1SR_Buf[] ={ /*1*/ 0xAA,0x1C,0x1B,0x01,0xAA,0x3C,0x01,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PATTERN_1ST_Buf[] ={ /*1*/ 0xAA,0x1C,0x1B,0x01,0xAA,0x3C,0x00,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PATTERN_2SR_Buf[] ={ /*1*/ 0xAA,0x1C,0x1B,0x02,0xAA,0x3C,0x01,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PATTERN_2ST_Buf[] ={ /*1*/ 0xAA,0x1C,0x1B,0x02,0xAA,0x3C,0x00,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PATTERN_3SR_Buf[] ={ /*1*/ 0xAA,0x1C,0x1B,0x03,0xAA,0x3C,0x01,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };
unsigned char SAM_PATTERN_3ST_Buf[] ={ /*1*/ 0xAA,0x1C,0x1B,0x03,0xAA,0x3C,0x00,0x00,0xAA,0x1B,0x00,0x00,0xAA,0x3B,0x00,0x00 };


/**************************************************************************
	Function prototype
**************************************************************************/
void init_OCTOPUS_CTRL( void );
void pelco_reset( void );
void pelco_set( void );
void pelco_up( void );
void pelco_down( void );
void pelco_left( void );
void pelco_right( void );
void pelco_osd( void );
void pelco_iris_open( void );
void pelco_iris_close( void );
void pelco_focus_near( void );
void pelco_focus_far( void );
void pelco_zoom_wide( void );
void pelco_zoom_tele( void );

void pelco_scan_1_sr( void );
void pelco_scan_1_st( void );
void pelco_preset_1( void );
void pelco_preset_2( void );
void pelco_preset_3( void );
void pelco_pattern_1_sr( void );
void pelco_pattern_1_st( void );
void pelco_pattern_2_sr( void );
void pelco_pattern_2_st( void );
void pelco_pattern_3_sr( void );
void pelco_pattern_3_st( void );
void pelco_pattern_run( void );


void samsung_clk_set( void );
void samsung_set( void );
void samsung_up( void );
void samsung_down( void );
void samsung_left( void );
void samsung_right( void );
void samsung_osd( void );
void samsung_ptz_up( void );
void samsung_ptz_down( void );
void samsung_ptz_left( void );
void samsung_ptz_right( void );
void samsung_iris_open( void );
void samsung_iris_close( void );
void samsung_focus_near( void );
void samsung_focus_far( void );
void samsung_zoom_wide( void );
void samsung_zoom_tele( void );

void samsung_scan_1_sr( void );
void samsung_scan_1_st( void );
void samsung_preset_1( void );
void samsung_preset_2( void );
void samsung_preset_3( void );
void samsung_pattern_1_sr( void );
void samsung_pattern_1_st( void );
void samsung_pattern_2_sr( void );
void samsung_pattern_2_st( void );
void samsung_pattern_3_sr( void );
void samsung_pattern_3_st( void );

extern unsigned int vdec_mode;
extern unsigned int outmode;
extern void vdec_write_table(unsigned char chip_addr, unsigned char addr, unsigned char *tbl_ptr, unsigned char tbl_cnt);

void vdec_coaxial_select_ch(unsigned int ch)
{
	int i = 0;
    #ifdef ADG708BRUZ
	gpio_i2c_write( NVP6114+i, 0xFF, 0x01);
	gpio_i2c_write( NVP6114+i, 0x4C, ch);	
	gpio_i2c_write( NVP6114+i, 0xFF, 0x04);
	gpio_i2c_write( NVP6114+i, 0xF7, 0x03);
	#else //74LV125
	gpio_i2c_write( NVP6114+i, 0xFF, 0x04);
	if(ch%4 < 2)
		gpio_i2c_write( NVP6114+i, 0xB7, ch);
	else
		gpio_i2c_write( NVP6114+i, 0xF7, ch);
	#endif 
}

#define COAX_BAUDRATE_SD      0x19
#define COAX_BAUDRATE_AHD     0x0E

void vdec_coaxial_init(void)
{
	int i = 0;
	gpio_i2c_write( NVP6114+i, 0xFF, 0x01);
	#ifdef ADG708BRUZ
	gpio_i2c_write( NVP6114+i, 0xBC, 0xCC);
	gpio_i2c_write( NVP6114+i, 0xBD, 0xDC);
	gpio_i2c_write( NVP6114+i, 0xBE, 0xDD);
	gpio_i2c_write( NVP6114+i, 0xBF, 0xDD);
	#else //74LV125
	gpio_i2c_write( NVP6114+i, 0xBC, 0xDD);
	gpio_i2c_write( NVP6114+i, 0xBD, 0xDD);
	gpio_i2c_write( NVP6114+i, 0xBE, 0xDD);
	gpio_i2c_write( NVP6114+i, 0xBF, 0xDD);
	#endif 
	//gpio_i2c_write( NVP6114+i, 0x8B, 0x00);
	if(outmode == NVP6114_OUT_ALL_960H)
	{
		gpio_i2c_write( NVP6114+i, 0xFF, 0x04);
		gpio_i2c_write( NVP6114+i, 0x82, COAX_BAUDRATE_SD);
		gpio_i2c_write( NVP6114+i, 0x87,vdec_mode%2==NTSC?0x09:0x0a);
		gpio_i2c_write( NVP6114+i, 0x8D, 0xC0);
		gpio_i2c_write( NVP6114+i, 0x8E, 0x05);
		gpio_i2c_write( NVP6114+i, 0xAF, 0x01);
		gpio_i2c_write( NVP6114+i, 0xC2,COAX_BAUDRATE_SD);
		gpio_i2c_write( NVP6114+i, 0xC7,vdec_mode%2==NTSC?0x09:0x0a);
		gpio_i2c_write( NVP6114+i, 0xCD,0xc0);
		gpio_i2c_write( NVP6114+i, 0xCE,0x05);
		gpio_i2c_write( NVP6114+i, 0xEF,0x01);
	}
	else if(outmode == NVP6114_OUT_2X720P_2X960H)
	{
		gpio_i2c_write( NVP6114+i, 0xFF, 0x04);
		gpio_i2c_write( NVP6114+i, 0x82,COAX_BAUDRATE_AHD);
		gpio_i2c_write( NVP6114+i, 0x87,vdec_mode%2==NTSC?0x14:0x15);
		gpio_i2c_write( NVP6114+i, 0x8D,vdec_mode%2==NTSC?0xb0:0xF0);
		gpio_i2c_write( NVP6114+i, 0x8E,vdec_mode%2==NTSC?0x05:0x06);
		gpio_i2c_write( NVP6114+i, 0xAF,0x00);
		gpio_i2c_write( NVP6114+i, 0xC2,COAX_BAUDRATE_SD);
		gpio_i2c_write( NVP6114+i, 0xC7,vdec_mode%2==NTSC?0x09:0x0a);
		gpio_i2c_write( NVP6114+i, 0xCD,0xc0);
		gpio_i2c_write( NVP6114+i, 0xCE,0x05);
		gpio_i2c_write( NVP6114+i, 0xEF,0x01);
	}
	else if(outmode == NVP6114_OUT_2X960_2X720P)
	{
		gpio_i2c_write( NVP6114+i, 0xFF, 0x04);
		gpio_i2c_write( NVP6114+i, 0x82, COAX_BAUDRATE_SD);
		gpio_i2c_write( NVP6114+i, 0x87, vdec_mode%2==NTSC?0x09:0x0a);
		gpio_i2c_write( NVP6114+i, 0x8D, 0xc0);
		gpio_i2c_write( NVP6114+i, 0x8E, 0x05);
		gpio_i2c_write( NVP6114+i, 0xAF, 0x01);
		gpio_i2c_write( NVP6114+i, 0xC2, COAX_BAUDRATE_AHD);
		gpio_i2c_write( NVP6114+i, 0xC7, vdec_mode%2==NTSC?0x14:0x15);
		gpio_i2c_write( NVP6114+i, 0xCD, vdec_mode%2==NTSC?0xb0:0xF0);
		gpio_i2c_write( NVP6114+i, 0xCE, vdec_mode%2==NTSC?0x05:0x06);
		gpio_i2c_write( NVP6114+i, 0xEF, 0x00);
	}
	else 
	{
		gpio_i2c_write( NVP6114+i, 0xFF,0x04);
		gpio_i2c_write( NVP6114+i, 0x82,COAX_BAUDRATE_AHD);
		gpio_i2c_write( NVP6114+i, 0x87,vdec_mode%2==NTSC?0x14:0x15);
		gpio_i2c_write( NVP6114+i, 0x8D,vdec_mode%2==NTSC?0xb0:0xF0);
		gpio_i2c_write( NVP6114+i, 0x8E,vdec_mode%2==NTSC?0x05:0x06);
		gpio_i2c_write( NVP6114+i, 0xAF,0x00);
		gpio_i2c_write( NVP6114+i, 0xC2,COAX_BAUDRATE_AHD);
		gpio_i2c_write( NVP6114+i, 0xC7,vdec_mode%2==NTSC?0x14:0x15);
		gpio_i2c_write( NVP6114+i, 0xCD,vdec_mode%2==NTSC?0xb0:0xF0);
		gpio_i2c_write( NVP6114+i, 0xCE,vdec_mode%2==NTSC?0x05:0x06);
		gpio_i2c_write( NVP6114+i, 0xEF,0x00);
	}
}

void pelco_coax_mode( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );

	gpio_i2c_write( FPGA_AFE, COAX_BAUD, 	0x37 );
	//gpio_i2c_write( FPGA_AFE, BL_HSP01, 	0x46 );
	gpio_i2c_write( FPGA_AFE, PACKET_MODE, 	0x06 );
	gpio_i2c_write( FPGA_AFE, PEL_CTEN, 	0x00 );
	gpio_i2c_write( FPGA_AFE, HSO_INV, 		0x00 );

	gpio_i2c_write( FPGA_AFE, 0xC0, 	0x37 );
	//gpio_i2c_write( FPGA_AFE, BL_HSP01, 	0x46 );
	gpio_i2c_write( FPGA_AFE, 0xCB, 	0x06 );
	gpio_i2c_write( FPGA_AFE, 0xCC, 	0x00 );
	gpio_i2c_write( FPGA_AFE, 0xED, 	0x00 );
	//gpio_i2c_write( FPGA_AFE, EVEN_LINE, 	0x01 );
	//gpio_i2c_write( FPGA_AFE, PEL_TXST1, 	0x09 ); //tony 11.26
	printk("Pelco 32bit mode\n");
}

void pelco_reset( void )
{
	//msleep( 20 );
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
	if(g_coax_ch<2)
	{
		gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
		msleep( 25 );
		gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
		msleep( 30 );	
	}
	else
	{
		gpio_i2c_write( FPGA_AFE, PEL_D0_34CH, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D1_34CH, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D2_34CH, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D3_34CH, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_OUT_34CH, 	0x01 );
		msleep( 25 );
		gpio_i2c_write( FPGA_AFE, PEL_OUT_34CH, 	0x00 );
		msleep( 30 );	
	}
}

void pelco_set(void)
{
	pelco_reset();
	if(g_coax_ch<2)
	{
		gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
	    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x40 );
	    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
		msleep( 25 );
		gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	}
	else
	{
		gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
	    gpio_i2c_write( FPGA_AFE, PEL_D0_34CH, 	0x40 );
	    gpio_i2c_write( FPGA_AFE, PEL_D1_34CH, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D2_34CH, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D3_34CH, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_OUT_34CH, 	0x01 );
		msleep( 25 );
		gpio_i2c_write( FPGA_AFE, PEL_OUT_34CH, 	0x00 );
	}	
	printk("Camera Setting mode\n");
}

void pelco_up(void)
{
	pelco_reset();
	if(g_coax_ch<2)
	{
		gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
		gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
		gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x10 );			// PELCO_DATA_02 FIRST
	    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x4C );
	    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
		msleep( 25 );
		gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	}
	else
	{
		gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
		gpio_i2c_write( FPGA_AFE, PEL_D0_34CH, 	0x00 );
		gpio_i2c_write( FPGA_AFE, PEL_D1_34CH, 	0x10 );			// PELCO_DATA_02 FIRST
	    gpio_i2c_write( FPGA_AFE, PEL_D2_34CH, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D3_34CH, 	0x4C );
	    gpio_i2c_write( FPGA_AFE, PEL_OUT_34CH, 	0x01 );
		msleep( 25 );
		gpio_i2c_write( FPGA_AFE, PEL_OUT_34CH, 	0x00 );
	}	
	printk("Camera Up moving mode\n");
}

void pelco_down(void)
{
	pelco_reset();
	if(g_coax_ch<2)
	{
		gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
	    gpio_i2c_write( FPGA_AFE, PEL_D0,   0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D1,   0x08 );
	    gpio_i2c_write( FPGA_AFE, PEL_D2,   0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D3,   0x4C );
	    gpio_i2c_write( FPGA_AFE, PEL_OUT,  0x01 );
		msleep( 25 );
		gpio_i2c_write( FPGA_AFE, PEL_OUT,  0x00 );
	}
	else
	{
		gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
	    gpio_i2c_write( FPGA_AFE, PEL_D0_34CH,   0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D1_34CH,   0x08 );
	    gpio_i2c_write( FPGA_AFE, PEL_D2_34CH,   0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D3_34CH,   0x4C );
	    gpio_i2c_write( FPGA_AFE, PEL_OUT_34CH,  0x01 );
		msleep( 25 );
		gpio_i2c_write( FPGA_AFE, PEL_OUT_34CH,  0x00 );
	}	
	printk("Camera Down moving mode\n");
}

void pelco_left(void)
{
	pelco_reset();
	if(g_coax_ch<2)
	{
		gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
	    gpio_i2c_write( FPGA_AFE, PEL_D0,  	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D1,  	0x20 );
	    gpio_i2c_write( FPGA_AFE, PEL_D2,  	0x4C );
	    gpio_i2c_write( FPGA_AFE, PEL_D3,  	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
		msleep( 25 );
		gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	}
	else
	{
		gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
	    gpio_i2c_write( FPGA_AFE, PEL_D0_34CH,  	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D1_34CH,  	0x20 );
	    gpio_i2c_write( FPGA_AFE, PEL_D2_34CH,  	0x4C );
	    gpio_i2c_write( FPGA_AFE, PEL_D3_34CH,  	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_OUT_34CH, 	0x01 );
		msleep( 25 );
		gpio_i2c_write( FPGA_AFE, PEL_OUT_34CH, 	0x00 );
	}	
	printk("Camera Left moving mode\n");
}

void pelco_right(void)
{
	pelco_reset();
	if(g_coax_ch<2)
	{
		gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
	    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x40 );
	    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x4C );
	    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
		msleep( 25 );
		gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	}
	else
	{
		gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
	    gpio_i2c_write( FPGA_AFE, PEL_D0_34CH, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_D1_34CH, 	0x40 );
	    gpio_i2c_write( FPGA_AFE, PEL_D2_34CH, 	0x4C );
	    gpio_i2c_write( FPGA_AFE, PEL_D3_34CH, 	0x00 );
	    gpio_i2c_write( FPGA_AFE, PEL_OUT_34CH, 	0x01 );
		msleep( 25 );
		gpio_i2c_write( FPGA_AFE, PEL_OUT_34CH, 	0x00 );
	}	
	printk("Camera Right moving mode\n");
}

void pelco_osd(void)
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0xC0 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0xFA );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	msleep( 25 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO OSD ON mode\n");
}

void pelco_iris_open( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x40 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	msleep( 25 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO IRIS OPEN mode\n");
}

void pelco_iris_close( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x20 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	msleep( 25 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO IRIS CLOSE mode\n");
}

void pelco_focus_near( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x80 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	msleep( 25 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO FOCUS NEAR mode\n");
}

void pelco_focus_far( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x01 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	msleep( 25 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO FOCUS FAR mode\n");
}

void pelco_zoom_wide( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x02 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	msleep( 25 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO ZOOM WIDE mode\n");
}

void pelco_zoom_tele( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	msleep( 25 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO ZOOM TELE mode\n");
}

void pelco_scan_1_sr( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0xE0 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x46 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO SCAN 1 SR mode\n");
}
void pelco_scan_1_st( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0xE0 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO SCAN 1 ST mode\n");
}
void pelco_preset_1( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0xE0 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x80 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO PRESET 1 mode\n");
}
void pelco_preset_2( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0xE0 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x40 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("\r\nPELCO PRESET 2 mode ");
}
void pelco_preset_3( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0xE0 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0xC0 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO PRESET 3 mode\n");
}
void pelco_pattern_1_sr( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0xF8 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x01 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO PATTERN 1 SR mode\n");
}
void pelco_pattern_1_st( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x84 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x01 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO PATTERN 1 ST mode\n");
}
void pelco_pattern_2_sr( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0xF8 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x02 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO PATTERN 2 SR mode\n");
}
void pelco_pattern_2_st( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x84 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x02 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO PATTERN 2 ST mode\n");
}
void pelco_pattern_3_sr( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0xF8 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x03 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO PATTERN 3 SR mode\n");
}
void pelco_pattern_3_st( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0x84 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x03 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO PATTERN 3 ST mode\n");
}
void pelco_pattern_run( void )
{
	pelco_reset();
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x04 );
    gpio_i2c_write( FPGA_AFE, PEL_D0, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D1, 	0xC4 );
    gpio_i2c_write( FPGA_AFE, PEL_D2, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_D3, 	0x00 );
    gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x01 );
	gpio_i2c_write( FPGA_AFE, PEL_OUT, 	0x00 );
	    
	printk("PELCO PATTERN RUN mode\n");
}

void samsung_coax_mode( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );

	gpio_i2c_write( FPGA_AFE, COAX_BAUD, 0x37 );
	gpio_i2c_write( FPGA_AFE, BL_HSP01, 0x46 );
	gpio_i2c_write( FPGA_AFE, TX_START, 0x00 );
	gpio_i2c_write( FPGA_AFE, TX_BYTE_LEN, 0x08 );			// 2Line mode
	gpio_i2c_write( FPGA_AFE, PACKET_MODE, 0x00 );

	// Video format select
	gpio_i2c_write( FPGA_AFE, BL_TXST1, 	g_system? 0x06 : 0x05 );	// TX Line
	gpio_i2c_write( FPGA_AFE, BL_RXST1, 	g_system? 0x08 : 0x07 );	// RX Line
	gpio_i2c_write( FPGA_AFE, COAX_BAUD, 	g_system? 0x23 : 0x37 );	// TX Duty rate
	gpio_i2c_write( FPGA_AFE, COAX_RBAUD, 	g_system? 0x23 : 0x37 );	// TX Duty rate

	gpio_i2c_write( FPGA_AFE, EVEN_LINE, 0x01 );
	
	printk("Samsung 2 line mode\n") ;
}

void samsung_clk_set( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
	gpio_i2c_write( FPGA_AFE, SAM_OUT, 0x01 );
	gpio_i2c_write( FPGA_AFE, SAM_OUT, 0x00);
	msleep( 200 );
}

void samsung_set( void )
{

	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_SET_Buf, 16);
	samsung_clk_set();
	
	printk("***samsung ENTER mode ***\n");
}

void samsung_up( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_UP_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung UP mode***");
}

void samsung_down( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_DOWN_Buf, 16);
	samsung_clk_set();
			
	printk("\r\n***samsung DOWN mode***");
}

void samsung_left( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_LEFT_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung LEFT SET ***");
}

void samsung_right( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_RIGHT_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung RIGHT SET ***");
}

void samsung_osd( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_OSD_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung OSD ON SET ***");
}

void samsung_ptz_up( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PTZ_UP_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung PTZ UP mode***");
}

void samsung_ptz_down( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PTZ_DOWN_Buf, 16);
	samsung_clk_set();						
			
	printk("\r\n***samsung PTZ DOWN mode***");
}

void samsung_ptz_left( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PTZ_LEFT_Buf, 16);
	samsung_clk_set();

	printk("\r\n***samsung PTZ LEFT SET ***");
}

void samsung_ptz_right( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PTZ_RIGHT_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung PTZ RIGHT SET ***");
}

void samsung_iris_open( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_IRIS_OPEN_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung IRIS OPEN SET ***");
}

void samsung_iris_close( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_IRIS_CLOSE_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung IRIS CLOSE SET ***");
}

void samsung_focus_near( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_FOCUS_NEAR_Buf, 16);
	samsung_clk_set();							
			
	printk("\r\n***samsung FOCUS NEAR mode***");
}

void samsung_focus_far( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_FOCUS_FAR_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung FOCUS FAR mode***");
}

void samsung_zoom_wide( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_ZOOM_WIDE_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung ZOOM WIDE SET ***");
}

void samsung_zoom_tele( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_ZOOM_TELE_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung ZOOM TELE SET ***");
}

void samsung_scan_1_sr( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_SCAN_1SR_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung SAM_SCAN_1SR SET ***");
}

void samsung_scan_1_st( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_SCAN_1ST_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung SAM_SCAN_1ST SET ***");
}

void samsung_preset_1( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PRESET1_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung PRESET1 SET ***");
}

void samsung_preset_2( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PRESET2_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung PRESET2 SET ***");
}

void samsung_preset_3( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PRESET3_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung PRESET3 SET ***");
}

void samsung_pattern_1_sr( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PATTERN_1SR_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung PATTERN_1SR SET ***");
}

void samsung_pattern_1_st( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PATTERN_1ST_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung PATTERN_1ST SET ***");
}

void samsung_pattern_2_sr( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PATTERN_2SR_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung PATTERN_2SR SET ***");
}

void samsung_pattern_2_st( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PATTERN_2ST_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung PATTERN_2ST SET ***") ;
}

void samsung_pattern_3_sr( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PATTERN_3SR_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung PATTERN_3SR SET ***") ;
}

void samsung_pattern_3_st( void )
{
	gpio_i2c_write( FPGA_AFE, 0xFF, 0x01 );
    vdec_write_table(FPGA_AFE, SAM_D0, SAM_PATTERN_3ST_Buf, 16);
	samsung_clk_set();
	
	printk("\r\n***samsung PATTERN_3ST SET ***") ;
}
