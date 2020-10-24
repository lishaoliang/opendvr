#ifndef _HI_COMM_IVE_H_
#define _HI_COMM_IVE_H_
#include "hi_type.h"

#ifdef __cplusplus
    #if __cplusplus
    extern "C"{
    #endif
#endif 

typedef HI_S32 IVE_HANDLE;

typedef struct hiIVE_DATA_ATTR_S
{
    HI_U32 u32PhyAddr;  /*Physical address of the data*/
    HI_U32 u32Stride;   /*Data stride*/
}IVE_MEM_INFO_S;

/*
*Type of the input data
*The input data is classified into three types:
*SP420, SP422, and single component (including the components Y, U, V, R, G, and B)
*/
typedef enum hiIVE_SRC_FMT_E
{
    IVE_SRC_FMT_SINGLE = 0, /*Single component*/
    IVE_SRC_FMT_SP420,      /*YUV SP420*/
    IVE_SRC_FMT_SP422,      /*YUV SP422*/
    IVE_SRC_FMT_BUTT
}IVE_SRC_FMT_E;

/*
*Input data format
*Information about the data formats supported by the IVE. The IVE supports the data in the following formats:
*1 YUV SP422/SP420
*2 RGB package/planar 
*3 Single component
*/
typedef struct hiIVE_SRC_INFO_S
{
    IVE_SRC_FMT_E  enSrcFmt;
    IVE_MEM_INFO_S stSrcMem; /*Data information*/
    
    HI_U32 u32Height;   /*Data height*/
    HI_U32 u32Width;    /*Data width*/
}IVE_SRC_INFO_S;


/*
*Type of the color space conversion (CSC) output results
*/
typedef enum hiIVE_CSC_OUT_FMT_E
{
    IVE_CSC_OUT_FMT_PACKAGE = 0,      
    IVE_CSC_OUT_FMT_PLANAR,           
    IVE_CSC_OUT_FMT_BUTT
}IVE_CSC_OUT_FMT_E;

/* 
*CSC working mode
*/
typedef enum hiIVE_CSC_MODE_E
{
    IVE_CSC_MODE_VIDEO_BT601_AND_BT656 = 0, /*CSC video transfer mode [16, 235]*/    
    IVE_CSC_MODE_VIDEO_BT709,               /*CSC video transfer mode [16, 235]*/
    IVE_CSC_MODE_PIC_BT601_AND_BT656,       /*CSC picture transfer mode [0, 255]*/
    IVE_CSC_MODE_PIC_BT709,                 /*CSC picture transfer mode [0, 255]*/
    IVE_CSC_MODE_BUTT
}IVE_CSC_MODE_E;

/*
*CSC control parameters
*You need to set these parameters when using the CSC operator.
*/
typedef struct hiIVE_CSC_CTRL_S
{
    IVE_CSC_OUT_FMT_E enOutFmt;  /*Output format*/
    IVE_CSC_MODE_E    enCscMode; /*Working mode*/
}IVE_CSC_CTRL_S;


/*
*Filter control parameters
*You need to set these parameters when using the filter operator.
*/
typedef struct hiIVE_FILTER_CTRL_S
{
    HI_S8 as8Mask[9];        /*Template parameter filter coefficient*/
    HI_S8 u8Norm;            /*Sum of all mask parameter values for normalization*/
}IVE_FILTER_CTRL_S;


/*
*Filter+CSC control parameters
*You need to set these parameters when using the filter+CSC operator.
*The control information about both the filter and CSC needs to be configured.
*/
typedef struct hiIVE_FILTER_AND_CSC_CTRL_S
{
    IVE_CSC_OUT_FMT_E enOutFmt;  /*Output format*/
     IVE_CSC_MODE_E enCscMode;   /*CSC working mode*/
        HI_S8 as8Mask[9];        /*Template parameter filter coefficient*/
        HI_S8 u8Norm;            /*Sum of all mask parameter values for normalization*/
}IVE_FILTER_AND_CSC_CTRL_S;

/*
*SOBEL control parameter
*/
typedef struct hiIVE_SOBEL_CTRL_S
{
    HI_S8 as8Mask[9];         /*Template parameter*/
}IVE_SOBEL_CTRL_S;



/*
*Type of the CANNY output results
*/
typedef enum hiIVE_CANNY_OUT_FMT_E
{
    IVE_CANNY_OUT_FMT_ONLY_MAG = 0,      /*Only the magnitude is output.*/    
    IVE_CANNY_OUT_FMT_MAG_AND_ANG,       /*The magnitude and angle are output.*/
    IVE_CANNY_OUT_FMT_BUTT
}IVE_CANNY_OUT_FMT_E;

/*
*CANNY control parameter
*/
typedef struct hiIVE_CANNY_CTRL_S
{
    IVE_CANNY_OUT_FMT_E enOutFmt;
    HI_S8 as8Mask[9];         /*The template parameter is the same as that of the SOBEL operator.*/
}IVE_CANNY_CTRL_S;


/*
*Dilate control parameters
*/
typedef struct hiIVE_DILATE_CTRL_S
{
    HI_U8 au8Mask[9];         /*The template parameter value must be 0 or 255.*/
}IVE_DILATE_CTRL_S;


/*
*Erode control parameter
*/
typedef struct hiIVE_ERODE_CTRL_S
{
    HI_U8 au8Mask[9];         /*The template parameter value must be 0 or 255.*/
}IVE_ERODE_CTRL_S;


/*
*Type of the thresh output results
*/
typedef enum hiIVE_THRESH_OUT_FMT_E
{
    IVE_THRESH_OUT_FMT_BINARY = 0,/*If the source value is greater than the thresh, the maximum value is used; if the source value is smaller than or equal to the thresh, the minimum value is used.*/
    IVE_THRESH_OUT_FMT_TRUNC,     /*If the source value is greater than the thresh, the maximum value is used; if the source value is smaller than or equal to the thresh, the source value is retained.*/
    IVE_THRESH_OUT_FMT_TOZERO,    /*If the source value is greater than the thresh, the source value is retained; if the source value is smaller than or equal to the thresh, the minimum value is used.*/
    IVE_THRESH_OUT_FMT_BUTT
}IVE_THRESH_OUT_FMT_E;

/*
*Thresh control parameters
*/
typedef struct hiIVE_THRESH_CTRL_S
{
    IVE_THRESH_OUT_FMT_E enOutFmt;
    HI_U32 u32Thresh; /*user-defined threshold*/
    HI_U32 u32MinVal; /*Minimum value when binarization*/
    HI_U32 u32MaxVal; /*Maxmum value when binarization*/
}IVE_THRESH_CTRL_S;


/*
*Type of the SUB output results
*/
typedef enum hiIVE_SUB_OUT_FMT_E
{
    IVE_SUB_OUT_FMT_ABS = 0,          /*Absolute value of the difference*/
    IVE_SUB_OUT_FMT_SFR,              /*The output result is obtained by shifting the result one digit right to reserve the signed bit.*/ 
    IVE_SUB_OUT_FMT_BUTT
}IVE_SUB_OUT_FMT_E;

#ifdef __cplusplus
    #if __cplusplus
}
    #endif
#endif
#endif/*__HI_COMM_IVE_H__*/