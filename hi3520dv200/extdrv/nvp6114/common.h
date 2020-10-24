/*************************************************************************
        Project : AWB Mapping One Block Program
        File Name: common.h
        Compiler: AVR IAR Compiler
        Author  : NEXTCHIP(c) cshong
        Data    : 2006.12.02
        Version : 1.00
        Dicription

        ----------------------------------------------------------
            Copyright(c) 2001 by Black_List All Rights Reserved

	common define
**************************************************************************/
//#define BIT(X)		(1<<X)

// device address define
#define NVP1108		0x60
#define NVP1104B	0x60
#define NVP1104B_1	0x60
//#define NVP6114		0x62
#define NVP1108B4	0x66
//#define NVP1104B_2	0x62
//#define NVP1104B_3	0x64
//#define NVP1104B_4	0x66
#define NVP1104B_AFE	0x66
#define NVP1108_AFE	0x66

#define NVP1114A	0x70
//#define NVP1104A_1	0x70
#define NVP1114A_AFE		0x76
#define NVP1104A_AFE		0x76
#define NVP5000_SLAVE_ADDR	0xDC
//#define FPGA_CTRL	0x56
//#define FPGA_AFE	0x56
#define FPGA_CTRL	0x64
#define SDR_CTRL	0x60
#define FPGA_AFE	0x60
#define FPGA_HDMI	0x50

//#define ADG708BRUZ


// Port Data Register
#define PORT7   	7
#define PORT6   	6
#define PORT5   	5
#define PORT4   	4
#define PORT3   	3
#define PORT2   	2
#define PORT1   	1
#define PORT0   	0
//system define
#define NTSC		0x00
#define PAL			0x01

// HSIZE
#define H960		0x00
#define H720		0x01

#define TYPE_04B	0x77
#define TYPE_08		0x75

#define CVBS		0x00
#define SVID		0x01
//init define
#define HIGH		1
#define LOW			0

#define ON		1
#define OFF		0

#define KEY_NORMAL  0x01
#define KEY_DEBUG	0x02
#define KEY_MENU	0x03

#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

#define _BIT0		0xFE
#define _BIT1		0xFD
#define _BIT2		0xFB
#define _BIT3		0xF7
#define _BIT4		0xEF
#define _BIT5		0xDF
#define _BIT6		0xBF
#define _BIT7		0x7F

//KEY
#define KEY_CH1		1
#define KEY_CH2		2
#define KEY_CH3		3
#define KEY_CH4		4

#define KEY_SAVE	5
#define KEY_QUAD	6
#define KEY_MODE	7
#define KEY_HEX		7
#define KEY_SET     8

#define KEY_LEFT	3
#define KEY_RIGH	4
#define KEY_UP		1
#define KEY_DOWN	2
#define KEY_DSEL	10

#define TRUE		1
#define FALSE		0

#define SAVEDMODE 		0x77

#define RESOL_800X600	0
#define RESOL_1024X768	1
#define RESOL_1280X1024	2

// EEPROM Address define
#define sADTFLAG	7

#define PALADDR 		0x0100
#define	EE_INT			0x0150	//Initial

#define FORMAT			0x0130
#define OUTPUT			0x0131
#define H_MEM			0x0132

// brightness
#define eBRIGHT			0x0200
#define eCONTRAST		0x0201
#define eHUE			0x0202
#define eSATURATION		0x0203
#define eSHARPNESS		0x0204
#define eVGARESOL		0x0205
#define eVGANRF			0x0206
#define eCHANNEL		0x0170

#define KEYDEF 		0x0FFF

// DECODER Define
#define DEC_ID		0x1B

/*********************************************************
	COAXIAL PROTOCOL Variables Definition
**********************************************************/
#if 0
//NVP1108S Define
#define PEL_D0	0x20
#define PEL_D1	0x21
#define PEL_D2	0x22
#define PEL_D3	0x23
#define PEL_OUT	0x0F
#define COAX_BAUD	0x00
#define BL_HSP01	0x0D
#define PACKET_MODE	0x0B
#define PEL_CTEN	0x0C
#define PEL_TXST1	0x07
#define EVEN_LINE	0x2F

#define SAM_D0	0x10
#define SAM_OUT	0x09
#endif

//NVP6114 Define
#define PEL_D0	0xA0
#define PEL_D1	0xA1
#define PEL_D2	0xA2
#define PEL_D3	0xA3
#define PEL_OUT	0x8C

#define PEL_D0_34CH	0xE0
#define PEL_D1_34CH	0xE1
#define PEL_D2_34CH	0xE2
#define PEL_D3_34CH	0xE3
#define PEL_OUT_34CH	0xCC

#define PEL_CTEN	0x8C
#define PEL_TXST1	0x87
#define EVEN_LINE	0xAF

#define COAX_CH_SEL	0x8B
#define COAX_BAUD	0x80
#define COAX_RBAUD	0x81
#define BL_TXST1	0x83
#define BL_RXST1	0x85
#define BL_HSP01	0x8D
#define PACKET_MODE	0x8B
#define TX_START	0x89
#define TX_BYTE_LEN	0x8A



#define SAM_D0	0x90
#define SAM_OUT	0x89
#define HSO_INV	0xAD


