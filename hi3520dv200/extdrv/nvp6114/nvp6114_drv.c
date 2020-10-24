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
#include "video.h"
#include "coax_protocol.h"
#include "motion.h"
#include "common.h"


//extern void sil9034_1080i60_init(void);
//extern void sil9034_1080i50_init(void);
//extern void sil9034_1080p30_init(void);
extern unsigned char g_coax_ch;

unsigned int vdec_mode = 1;  //0:ntsc, 1: pal
unsigned int vdec_mode_bak = 0xFF;
unsigned int chmode = 0x00;  //1 port 1ch mode
unsigned int vdec_cnt = 0;
unsigned int vdec_slave_addr[4] = {0x60, 0x62, 0x64, 0x66};
unsigned int outmode=NVP6114_OUT_ALL_720P;
module_param(outmode, uint, S_IRUGO);
module_param(chmode, uint, S_IRUGO);
module_param(vdec_mode, uint, S_IRUGO);
spinlock_t  nvp6114_lock;

int check_id(unsigned int dec)
{
	int ret;
	gpio_i2c_write(dec, 0xFF, 0x01);
	ret = gpio_i2c_read(dec, 0xf4);
	return ret;
}

int vdec_open(struct inode * inode, struct file * file)
{
	return 0;
} 

int vdec_close(struct inode * inode, struct file * file)
{
	return 0;
}

//int vdec_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
long vdec_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	unsigned int __user *argp = (unsigned int __user *)arg;	
	unsigned int on;
	unsigned char vloss=0;
	unsigned long ptz_ch;
	unsigned int sens[16];
	vdec_video_mode vmode;
	vdec_video_adjust v_adj;
	unsigned int ret=0;
	

	switch (cmd)
	{
			case IOC_VDEC_GET_VIDEO_LOSS:
			spin_lock(&nvp6114_lock);	
			{
				gpio_i2c_write(vdec_slave_addr[1], 0xFF, 0x01);
				vloss=gpio_i2c_read(vdec_slave_addr[1], 0xD8)<<4;
				gpio_i2c_write(vdec_slave_addr[0], 0xFF, 0x01);
				vloss|=gpio_i2c_read(vdec_slave_addr[0], 0xD8);
			}
			spin_unlock(&nvp6114_lock);
			//printk("IOC_VDEC_GET_VIDEO_LOSS ret[%x]\n", ret);
			if(copy_to_user(argp, &vloss, sizeof(unsigned char)))
				printk("IOC_VDEC_GET_VIDEO_LOSS error\n");
			break;
        case IOC_VDEC_SET_VIDEO_MODE:
            if (copy_from_user(&vmode, argp, sizeof(vdec_video_mode)))
				return -1;
			spin_lock(&nvp6114_lock);
			#if 1
			vdec_mode = vmode.mode;
			if(vdec_mode_bak != (vdec_mode%2))
			{
				/*第一次启动的时候初始化一次6114*/
				if(vdec_mode%2 == NTSC)
					nvp6114_720p_30fps();
				else
					nvp6114_720p_25fps();
				vdec_mode_bak = vdec_mode%2;
				printk("init once is needed\n");
			}
			if(outmode == NVP6114_OUT_ALL_960H)
			{
				nvp6114_960H_setting(vdec_mode&0x01, 0x0F);
				printk("NVP6114_OUT_ALL_960H\n");
			}
			else if(outmode == NVP6114_OUT_2X960_2X720P)
			{
				nvp6114_960H_setting(vdec_mode&0x01, 0x03);
				printk("NVP6114_OUT_2X960_2X720P\n");
			}
			else if(outmode == NVP6114_OUT_2X720P_2X960H)
			{
				nvp6114_960H_setting(vdec_mode&0x01, 0x0C);
				printk("NVP6114_OUT_2X720P_2X960H\n");
			}
			else
			{
				nvp6114_960H_setting(vdec_mode&0x01, 0x00);
				printk("NVP6114_OUT_4X720P\n");
			}
			#endif
			spin_unlock(&nvp6114_lock);
			break;
#if 1
		case IOC_VDEC_SET_BRIGHTNESS:
            if(copy_from_user(&v_adj, argp, sizeof(vdec_video_adjust))) return -1;
			spin_lock(&nvp6114_lock);
			vdec_video_set_brightness(v_adj.ch, v_adj.value, vdec_mode&1);
			spin_unlock(&nvp6114_lock);
			break;
		case IOC_VDEC_SET_CONTRAST:
			if(copy_from_user(&v_adj, argp, sizeof(vdec_video_adjust))) return -1;
			spin_lock(&nvp6114_lock);
			vdec_video_set_contrast(v_adj.ch, v_adj.value, vdec_mode&1);
			spin_unlock(&nvp6114_lock);
			break;
		case IOC_VDEC_SET_HUE:
			if(copy_from_user(&v_adj, argp, sizeof(vdec_video_adjust))) return -1;
			spin_lock(&nvp6114_lock);
			vdec_video_set_hue(v_adj.ch, v_adj.value, vdec_mode&1);
			spin_unlock(&nvp6114_lock);
			break;
		case IOC_VDEC_SET_SATURATION:
			if(copy_from_user(&v_adj, argp, sizeof(vdec_video_adjust))) return -1;
			spin_lock(&nvp6114_lock);
			vdec_video_set_saturation(v_adj.ch, v_adj.value, vdec_mode&1);
			spin_unlock(&nvp6114_lock);
			break;
		case IOC_VDEC_SET_SHARPNESS:
			break;
#endif 
		case IOC_VDEC_PTZ_PELCO_INIT:
			spin_lock(&nvp6114_lock);
			vdec_coaxial_init();
			pelco_coax_mode();
			spin_unlock(&nvp6114_lock);
			break;
		case IOC_VDEC_PTZ_PELCO_RESET:
			spin_lock(&nvp6114_lock);
			pelco_reset();
			spin_unlock(&nvp6114_lock);
			break;
		case IOC_VDEC_PTZ_PELCO_SET:
			spin_lock(&nvp6114_lock);
			pelco_set();
			spin_unlock(&nvp6114_lock);
			break;
		case IOC_VDEC_PTZ_CHANNEL_SEL:
            if (copy_from_user(&ptz_ch, argp, sizeof(ptz_ch)))
			{
				return -1;
			}
			spin_lock(&nvp6114_lock);
			vdec_coaxial_select_ch(ptz_ch);
			#ifdef ADG708BRUZ
			g_coax_ch = 3;
			#else //74LV125
			g_coax_ch = ptz_ch;
			#endif 
			spin_unlock(&nvp6114_lock);
			break;
		case IOC_VDEC_PTZ_PELCO_UP:
			spin_lock(&nvp6114_lock);
		 	pelco_up();
			spin_unlock(&nvp6114_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_DOWN:
			spin_lock(&nvp6114_lock);
		 	pelco_down();
			spin_unlock(&nvp6114_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_LEFT:
			spin_lock(&nvp6114_lock);
		 	pelco_left();
			spin_unlock(&nvp6114_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_RIGHT:
			spin_lock(&nvp6114_lock);
			pelco_right();
			spin_unlock(&nvp6114_lock);
			break;
		case IOC_VDEC_PTZ_PELCO_FOCUS_NEAR:
			//FIXME
			spin_lock(&nvp6114_lock);
			pelco_osd();
			spin_unlock(&nvp6114_lock);
			//pelco_focus_near();
		 	break;
		case IOC_VDEC_PTZ_PELCO_FOCUS_FAR:
			//FIXME
			spin_lock(&nvp6114_lock);
			pelco_set();
			spin_unlock(&nvp6114_lock);
			//pelco_focus_far();
		 	break;
		case IOC_VDEC_PTZ_PELCO_ZOOM_WIDE:
			//pelco_zoom_wide();
			spin_lock(&nvp6114_lock);
			pelco_iris_open();
			spin_unlock(&nvp6114_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_ZOOM_TELE:
			//pelco_zoom_tele();
			spin_lock(&nvp6114_lock);
			pelco_iris_close();
			spin_unlock(&nvp6114_lock);
			break;

		case IOC_VDEC_INIT_MOTION:
			spin_lock(&nvp6114_lock);
			vdec_motion_init();
			hi3520_init_blank_data(0);
			spin_unlock(&nvp6114_lock);
			break;
		case IOC_VDEC_ENABLE_MOTION:
			break;
		case IOC_VDEC_DISABLE_MOTION:
			break;
		case IOC_VDEC_SET_MOTION_AREA:
			break;
		case IOC_VDEC_GET_MOTION_INFO:
			vdec_get_motion_info(0);
			break;
		case IOC_VDEC_SET_MOTION_DISPLAY:
            if(copy_from_user(&on, argp, sizeof(unsigned int))) return -1;
			spin_lock(&nvp6114_lock);
			vdec_motion_display(0,on);
			spin_unlock(&nvp6114_lock);
			break;
		case IOC_VDEC_SET_MOTION_SENS:
            if(copy_from_user(&sens, argp, sizeof(unsigned int)*16)) return -1;
			spin_lock(&nvp6114_lock);
			vdec_motion_sensitivity(sens);
			spin_unlock(&nvp6114_lock);
			break;
		case IOC_VDEC_ENABLE_LOW_RES:
			//vdec_low_resoultion_enable(0xff);
			break;
		case IOC_VDEC_DISABLE_LOW_RES:
			//vdec_low_resoultion_disable(0xff);
			break;
		case IOC_VDEC_ENABLE_BW:
			//vdec_bw_detection_enable(0xff);
			break;
		case IOC_VDEC_DISABLE_BW:
			//vdec_bw_detection_disable(0xff);
			break;
		case IOC_VDEC_READ_BALCK_COUNT:
			//value = vdec_bw_black_count_read(0);
			//copy_to_user(arg,&value, sizeof(int));
			break;
		case IOC_VDEC_READ_WHITE_COUNT:
			break;
		case IOC_VDEC_4CH_VIDEO_SEQUENCE:
			break;

		case IOC_AUDIO_SET_CHNNUM:
			{
				unsigned int chnnum;
				if(copy_from_user(&chnnum, argp, sizeof(unsigned int))) return -1;
				spin_lock(&nvp6114_lock);
				ret = audio_set_chnnum(chnnum);
				spin_unlock(&nvp6114_lock);				
			}
			break;
		case IOC_AUDIO_SET_SAMPLE_RATE:
			{
				nvp6114_audio_samplerate_en e_samplerate;
				if(copy_from_user(&e_samplerate, argp, sizeof(nvp6114_audio_samplerate_en))) return -1;
				spin_lock(&nvp6114_lock);
				ret = audio_set_samplerate(e_samplerate);
				spin_unlock(&nvp6114_lock);	
			}
			break;

		case IOC_AUDIO_SET_BITWIDTH:
			{
				nvp6114_audio_bites_en e_bites;
				if(copy_from_user(&e_bites, argp, sizeof(nvp6114_audio_bites_en))) return -1;
				spin_lock(&nvp6114_lock);
				ret = audio_set_bites_width(e_bites);
				spin_unlock(&nvp6114_lock);					
			}
			break;
			
#if 0
				
        case IOC_VIDEO_GET_VIDEO_MODE:
		case IOC_VIDEO_SET_MOTION:
        case IOC_VIDEO_GET_MOTION:
		case IOC_VIDEO_SET_MOTION_EN:
		case IOC_VIDEO_SET_MOTION_SENS:
		case IOC_VIDEO_SET_MOTION_TRACE:
        case IOC_VIDEO_GET_VIDEO_LOSS:
		case IOC_VIDEO_GET_IMAGE_ADJUST:       
        case IOC_AUDIO_SET_AUDIO_PLAYBACK:
        case IOC_AUDIO_SET_AUDIO_DA_VOLUME:
		case IOC_AUDIO_SET_BRIGHTNESS:
		case IOC_AUDIO_SET_CONTRAST:
		case IOC_AUDIO_SET_HUE:
		case IOC_AUDIO_SET_SATURATION:
		case IOC_AUDIO_SET_SHARPNESS:
		case IOC_AUDIO_SET_AUDIO_MUTE:
		case IOC_AUDIO_SET_LIVE_CH:
		case IOC_AUDIO_SET_PB_CH:
#endif
		default:
            //printk("drv:invalid nc decoder ioctl cmd[%x]\n", cmd);
			break;
	}
	return ret;
}

static struct file_operations vdec_fops = {
	.owner      = THIS_MODULE,
    //.ioctl      = vdec_ioctl,
    .unlocked_ioctl	= vdec_ioctl,
	.open       = vdec_open,
	.release    = vdec_close
};

static struct miscdevice vdec_dev = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= "nvp6114",
	.fops  		= &vdec_fops,
};

int chip_id[4];
static int __init vdec_module_init(void)
{
	int ret = 0, i = 0;

	ret = misc_register(&vdec_dev);

   	if (ret)
	{
		printk("ERROR: could not register NC Decoder devices:%#x \n",i);		
	}
	

	for(i=0;i<4;i++)
	{
		chip_id[i] = check_id(vdec_slave_addr[i]);
		if( (chip_id[i] != NVP6114_R0_ID ) )
		{
			printk("nvp6114 Device ID Error... %x\n", chip_id[i]);
		}
		else
		{
			printk("nvp6114 Device (0x%x) ID OK... %x\n", vdec_slave_addr[i], chip_id[i]);
			vdec_cnt++;
		}
	}
	

	printk("NVP6114 Count = %x\n", vdec_cnt);
	#if 1
	if(outmode < 4)
	{
		if(vdec_mode_bak != (vdec_mode%2))
		{
			/*第一次启动的时候初始化一次6114*/
			if(vdec_mode%2 == NTSC)
				nvp6114_720p_30fps();
			else
				nvp6114_720p_25fps();
			vdec_mode_bak = vdec_mode%2;
		}
		
		if(outmode == NVP6114_OUT_ALL_960H)
		{
			nvp6114_960H_setting(vdec_mode&0x01, 0x0F);
			printk("NVP6114_OUT_ALL_960H\n");
		}
		else if(outmode == NVP6114_OUT_2X960_2X720P)
		{
			nvp6114_960H_setting(vdec_mode&0x01, 0x03);
			printk("NVP6114_OUT_2X960_2X720P\n");
		}
		else if(outmode == NVP6114_OUT_2X720P_2X960H)
		{
			nvp6114_960H_setting(vdec_mode&0x01, 0x0C);
			printk("NVP6114_OUT_2X720P_2X960H\n");
		}
		else
		{
			nvp6114_960H_setting(vdec_mode&0x01, 0x00);
			printk("NVP6114_OUT_ALL_720P\n");
		}
	}
	
	#endif	

	audio_init(vdec_slave_addr[0], 4, 0, 0);
	
	spin_lock_init(&nvp6114_lock);
	return 0;
}



static void __exit vdec_module_exit(void)
{
	misc_deregister(&vdec_dev);	
}

module_init(vdec_module_init);
module_exit(vdec_module_exit);

MODULE_LICENSE("GPL");

