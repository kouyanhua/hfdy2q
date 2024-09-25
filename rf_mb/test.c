/*
 * test.c
 *
 *  Created on: 2024��3��4��
 *      Author: Administrator
 */


#include <stdio.h>
#include "xparameters.h"
#include "xil_io.h"
//#include "xtmrctr.h"
#include "xil_exception.h"
#include "xil_printf.h"

#define FIFO_SEND_BASE_ADDR 0x44a00000
#define FIFO_RECEIVE_BASE_ADDR 0x44a10000
#define RF_FREQ 0x40000000
#define RF_UP_GAIN 0x40010000
#define RF_DOWN_GAIN 0x40020000
#define RF_ctrl 0x40030000
#define RF_pre_cpi 0x40040000
#define RF_send_num 0x40050000
#define RF_ans_num 0x40060000
#define init 0x40070000
#define stop 0x40080000
#define RF_ask_flag 0x40090000
#define GPIO_0 0x400a0000
#define GPIO_1 0x400b0000

//仿真参数包
    //CPI头
    unsigned int CPIHEAD_Frame                      = 0xAAAABB01            ;
    unsigned int CPIHEAD_CPI_Cnt                    = 0x00000001            ;
    unsigned int CPIHEAD_DeltaT                     = 0x00000000            ;
    unsigned int CPIHEAD_WaveId                     = 0x00000001            ;
    //总体参数
    unsigned int OverALL_FreqNum                    = 0x00000001            ;
    unsigned int OverALL_PRTNum                     = 0x00000002            ;
    unsigned int OverALL_PRFType                    = 0x00000000            ;
    unsigned int OverALL_CPIPeriod                  = 0x00008020            ;
    unsigned int OverALL_FsNum                      = 0x00000000            ;
    unsigned int OverALL_ChFlag                     = 0x00000075            ;
    unsigned int OverALL_Freq[16]                   = {0x00001770}          ;
    unsigned int OverALL_PRT[32]                    = {0x00004010}          ;
    //波形参数
    //波形参数初始化
    unsigned int MainPulse_SignalType               = 0x00000001            ;
    unsigned int MainPulse_TP                       = 0x00000320            ;
    //发射信道信号参数
    unsigned int MainPulse_LFM_Trans_StartFreqCtrl  = 0xFF999DB3            ;
    unsigned int MainPulse_LFM_Trans_kCtrl          = 0x00000831            ;
    unsigned int MainPulse_LFM_Trans_kCtrl_Fu       = 0xFFFFF7CF            ;
    //接收信道去斜参考信号参数
    unsigned int MainPulse_LFM_Ref_StartFreqCtrl    = 0x33CCCDD3            ;
    unsigned int MainPulse_LFM_Ref_kCtrl            = 0x0000020C            ;
    unsigned int MainPulse_LFM_Ref_kCtrl_Fu         = 0xFFFFFDF4            ;
    //目标回波基带产生信号参数
    unsigned int MainPulse_LFM_Echo_StartFreqCtrl   = 0xFCCDD2F2            ;
    unsigned int MainPulse_LFM_Echo_kCtrl           = 0x00020C4A            ;
    unsigned int MainPulse_LFM_Echo_kCtrl_Fu        = 0xFFFDF3B6            ;
    //
    unsigned int MainPulse_Phase_CodeLen            = 0x00000001            ;
    unsigned int MainPulse_Phase_Type               = 0x00000000            ;
    unsigned int MainPulse_Phase_CodeT              = 0x00000000            ;
    unsigned int MainPulse_Phase_CodeSeq[32]        = {0x00000000}          ;
    unsigned int MainPulse_Freq_FreqType            = 0x00000002            ;
    unsigned int MainPulse_Freq_FreqSeq[4]          = {0x00000000}          ;

    //掩护脉冲
    unsigned int ScreenPulse_SignalType             = 0x00000001            ;
    unsigned int ScreenPulse_Tp                     = 0x00000000            ;
    unsigned int ScreenPulse_LFM_StartFreqCtrl      = 0x33333333            ;
    unsigned int ScreenPulse_LFM_kCtrl              = 0x00000000            ;
    unsigned int ScreenPulse_LFM_kCtrl_Fu           = 0x00000000            ;
    //相位编码、复杂编码，控制字同主脉冲
    unsigned int ScreenPulse_Phase_CodeLen          = 0x00000001            ;
    unsigned int ScreenPulse_Phase_Type             = 0x00000000            ;
    unsigned int ScreenPulse_Phase_CodeT            = 0x00000000            ;
    unsigned int ScreenPulse_Phase_CodeSeq[32]      = {0x00000000}          ;
    unsigned int ScreenPulse_Freq_FreqType          = 0x00000000            ;
    unsigned int ScreenPulse_Freq_FreqSeq[4]        = {0x00000000}          ;


    //信道参数
    //发射信道（5个通道、1个掩护通道）
    unsigned int Trans_Mode_TransSigMode            = 0x00000001            ;
    unsigned int Trans_MainPulse_PowerType[5]       = {0,11,12,13,14}       ;
    unsigned int Trans_MainPulse_DelayClkNum[5]     = {0x000003E8,1,2,3,4}  ;
    unsigned int Trans_MainPulse_res[5]             = {0,0,0,0,0}           ;
    unsigned int Trans_MainPulse_DopplerCtrl[5]     = {0,0,0,0,0}           ;
    unsigned int Trans_MainPulse_UpGain[5]          = {128,129,130,131,132} ;

    unsigned int Trans_ScreenPulse_PowerType        = 0x00000000            ;
    unsigned int Trans_ScreenPulse_DelayClkNum      = 0x000003E8            ;
    unsigned int Trans_ScreenPulse_res              = 0x00000000            ;
    unsigned int Trans_ScreenPulse_DopplerCtrl      = 0x00000000            ;
    unsigned int Trans_ScreenPulse_UpGain           = 0x00000080            ;

    unsigned int ChPara_Recv_Gain[5]                = {0x00000080,0,0,0,0}  ;
    unsigned int ChPara_Recv_DelayClk[5]            = {0x00000fe4,0,0,0,0}  ;
    unsigned int ChPara_Recv_SampleGateClk[5]       = {0x00006666,0,0,0,0}  ;
    unsigned int ChPara_Recv_DopplerCtrl[5]         = {0,0,0,0,0}           ;

    unsigned int ChPara_Recv_SumChCoe[5]            = {0x0000001A,0,0,0,0}  ;

    unsigned int ChPara_Recv_AziChCoe[5]            = {0x2FFF0000,0,0,0,0}  ;

    unsigned int ChPara_Recv_EleChCoe[5]            = {0x1FFF0000,0,0,0,0}  ;

    unsigned int ChPara_Recv_AuxCh1Coe[5]           = {0x27101111,0,0,0,0}  ;

    unsigned int ChPara_Recv_AuxCh2Coe[5]           = {0x00002222,0,0,0,0}  ;

    unsigned int ChPara_Recv_AuxCh3Coe[5]           = {0x1FFF3333,0,0,0,0}  ;

    unsigned int ChPara_Recv_AuxCh4Coe[5]           = {0x1FFF4444,0,0,0,0}  ;

    unsigned int ChPara_Recv_AuxCh5Coe[5]           = {0x1FFF5555,0,0,0,0}  ;

    //目标参数
    unsigned int TarPara_TarNum                     = 0x00000001            ;
    unsigned int TarPara_Type                       = 0x00000000            ;
    unsigned int TarPara_RCSCoef[32] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

    unsigned int TarPara_DelayClk[32]               = {0x000043A0}          ;
    unsigned int TarPara_DopplerCtrl[32]            = {0}                   ;

    unsigned int TarPara_AziCoe[32]                 = {0x008C0000}          ;
    unsigned int TarPara_EleCoe[32]                 = {0X11110000}          ;
    unsigned int TarPara_MainPower[32]              = {0X00006CFF}          ;

    //杂波参数
    unsigned int ClutterPara_Mode                   = 0x00000000            ;
    unsigned int ClutterPara_MainLobe_DelayClkNum   = 0x00000053            ;
    unsigned int ClutterPara_MainLobe_RangeNum      = 0x00000BD4            ;

    unsigned int ClutterPara_MainLobe_DopplerCenter = 0x00000000            ;
    unsigned int ClutterPara_MainLobe_DopplerB      = 0x0000000F            ;
    unsigned int ClutterPara_MainLobe_Power         = 0x00008000            ;
    unsigned int ClutterPara_Height_DelayClkNum     = 0x00000053            ;
    unsigned int ClutterPara_Height_RangeNum        = 0x00000BD4            ;
    unsigned int ClutterPara_Height_DopplerCenter   = 0x00000000            ;
    unsigned int ClutterPara_Height_DopplerB        = 0x00000011            ;
    unsigned int ClutterPara_Height_Power           = 0x00008000            ;

    //接收机参数
    //用于仿真截位噪声
    unsigned int Recv_JieWei                        = 0x00000008            ;
    unsigned int Recv_NoisePower                    = 0x00000001            ;

    //噪声参数
    //用于仿真接收机噪声
    unsigned int Noise_SampleGateDelayCtrl          = 0x00000053            ;
    unsigned int Noise_SampleNumCtrl                = 0x00000BE2            ;
    unsigned int Noise_Power                        = 0x00000003            ;

    //NCO控制字
    unsigned int NCO                                = 0x60000000            ;

    //回波补充参数
    unsigned int EchoSupple_ChnNum                  = 0x00000004            ;
    unsigned int EchoSupple_SampleNum               = 0x00000BE2            ;

    //干扰替换参数
    unsigned int JamZeroRepa_Before                 = 0x00000000            ;
    unsigned int JamZeroRepa_Behind                 = 0x00000000            ;

    //非线性调频
    unsigned int NLFM_Echo_FrepCtrl                 = 0x00000000            ;
    unsigned int NLFM_Echo_AmpCtrl                  = 0x00000000            ;
    unsigned int NLFM_Trans_FrepCtrl                = 0x00000000            ;
    unsigned int NLFM_Trans_AmpCtrl                 = 0x00000000            ;

    //射频控制
    unsigned int RFCtrl_TransCtrl_GateDelay[5]      = {10,11,12,13,14}      ;
    unsigned int RFCtrl_TransCtrl_GateWidth[5]      = {100,110,120,130,140} ;
    unsigned int RFCtrl_RecvCtrl_GateDelay[5]       = {20,21,22,23,24}      ;
    unsigned int RFCtrl_RecvCtrl_GateWidth[5]       = {200,210,220,230,240} ;

//int main(){
//    for(int i = 0;i<532;i++){
//        switch (i)
//        {
//        case  0 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x01DCEF18                          ); break;
//        case  1 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0xAA5555AA                          ); break;
//        case  2 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000001                          ); break;
//        case  3 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000001                          ); break;
//        case  4 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000001                          ); break;
//        case  5 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000001                          ); break;
//        case  6 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000200                          ); break;
//        case  7 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000200                          ); break;
//        case  8 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case  9 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case  10: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case  11: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case  12: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case  13: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case  14: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case  15: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case  16: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, CPIHEAD_Frame                       ); break;
//        case  17: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, CPIHEAD_CPI_Cnt                     ); break;
//        case  18: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, CPIHEAD_DeltaT                      ); break;
//        case  19: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, CPIHEAD_WaveId                      ); break;
//        case  20: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_FreqNum                     ); break;
//        case  21: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRTNum                      ); break;
//        case  22: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRFType                     ); break;
//        case  23: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_CPIPeriod                   ); break;
//        case  24: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_FsNum                       ); break;
//        case  25: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_ChFlag                      ); break;
//        case  26: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[ 0]                    ); break;
//        case  27: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[ 1]                    ); break;
//        case  28: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[ 2]                    ); break;
//        case  29: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[ 3]                    ); break;
//        case  30: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[ 4]                    ); break;
//        case  31: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[ 5]                    ); break;
//        case  32: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[ 6]                    ); break;
//        case  33: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[ 7]                    ); break;
//        case  34: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[ 8]                    ); break;
//        case  35: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[ 9]                    ); break;
//        case  36: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[10]                    ); break;
//        case  37: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[11]                    ); break;
//        case  38: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[12]                    ); break;
//        case  39: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[13]                    ); break;
//        case  40: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[14]                    ); break;
//        case  41: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_Freq[15]                    ); break;
//        case  42: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[ 0]                     ); break;
//        case  43: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[ 1]                     ); break;
//        case  44: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[ 2]                     ); break;
//        case  45: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[ 3]                     ); break;
//        case  46: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[ 4]                     ); break;
//        case  47: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[ 5]                     ); break;
//        case  48: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[ 6]                     ); break;
//        case  49: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[ 7]                     ); break;
//        case  50: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[ 8]                     ); break;
//        case  51: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[ 9]                     ); break;
//        case  52: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[ 0]                     ); break;
//        case  53: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[11]                     ); break;
//        case  54: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[12]                     ); break;
//        case  55: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[13]                     ); break;
//        case  56: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[14]                     ); break;
//        case  57: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[15]                     ); break;
//        case  58: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[16]                     ); break;
//        case  59: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[17]                     ); break;
//        case  60: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[18]                     ); break;
//        case  61: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[19]                     ); break;
//        case  62: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[20]                     ); break;
//        case  63: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[21]                     ); break;
//        case  64: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[22]                     ); break;
//        case  65: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[23]                     ); break;
//        case  66: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[24]                     ); break;
//        case  67: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[25]                     ); break;
//        case  68: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[26]                     ); break;
//        case  69: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[27]                     ); break;
//        case  70: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[28]                     ); break;
//        case  71: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[29]                     ); break;
//        case  72: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[30]                     ); break;
//        case  73: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, OverALL_PRT[31]                     ); break;
//        case  74: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_SignalType                ); break;
//        case  75: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_TP                        ); break;
//        case  76: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_LFM_Trans_StartFreqCtrl   ); break;
//        case  77: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_LFM_Trans_kCtrl           ); break;
//        case  78: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_LFM_Trans_kCtrl_Fu        ); break;
//        case  79: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_LFM_Ref_StartFreqCtrl     ); break;
//        case  80: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_LFM_Ref_kCtrl             ); break;
//        case  81: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_LFM_Ref_kCtrl_Fu          ); break;
//        case  82: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_LFM_Echo_StartFreqCtrl    ); break;
//        case  83: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_LFM_Echo_kCtrl            ); break;
//        case  84: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_LFM_Echo_kCtrl_Fu         ); break;
//        case  85: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeLen             ); break;
//        case  86: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_Type                ); break;
//        case  87: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeT               ); break;
//        case  88: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[ 0]         ); break;
//        case  89: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[ 1]         ); break;
//        case  90: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[ 2]         ); break;
//        case  91: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[ 3]         ); break;
//        case  92: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[ 4]         ); break;
//        case  93: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[ 5]         ); break;
//        case  94: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[ 6]         ); break;
//        case  95: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[ 7]         ); break;
//        case  96: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[ 8]         ); break;
//        case  97: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[ 9]         ); break;
//        case  98: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[10]         ); break;
//        case  99: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[11]         ); break;
//        case 100: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[12]         ); break;
//        case 101: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[13]         ); break;
//        case 102: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[14]         ); break;
//        case 103: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[15]         ); break;
//        case 104: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[16]         ); break;
//        case 105: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[17]         ); break;
//        case 106: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[18]         ); break;
//        case 107: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[19]         ); break;
//        case 108: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[20]         ); break;
//        case 109: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[21]         ); break;
//        case 110: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[22]         ); break;
//        case 111: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[23]         ); break;
//        case 112: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[24]         ); break;
//        case 113: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[25]         ); break;
//        case 114: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[26]         ); break;
//        case 115: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[27]         ); break;
//        case 116: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[28]         ); break;
//        case 117: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[29]         ); break;
//        case 118: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[30]         ); break;
//        case 119: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Phase_CodeSeq[31]         ); break;
//        case 120: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Freq_FreqType             ); break;
//        case 121: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Freq_FreqSeq[0]           ); break;
//        case 122: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Freq_FreqSeq[1]           ); break;
//        case 123: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Freq_FreqSeq[2]           ); break;
//        case 124: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, MainPulse_Freq_FreqSeq[3]           ); break;
//        case 125: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_SignalType              ); break;
//        case 126: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Tp                      ); break;
//        case 127: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_LFM_StartFreqCtrl       ); break;
//        case 128: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_LFM_kCtrl               ); break;
//        case 129: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_LFM_kCtrl_Fu            ); break;
//        case 130: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeLen           ); break;
//        case 131: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_Type              ); break;
//        case 132: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeT             ); break;
//        case 133: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[ 0]       ); break;
//        case 134: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[ 1]       ); break;
//        case 135: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[ 2]       ); break;
//        case 136: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[ 3]       ); break;
//        case 137: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[ 4]       ); break;
//        case 138: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[ 5]       ); break;
//        case 139: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[ 6]       ); break;
//        case 140: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[ 7]       ); break;
//        case 141: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[ 8]       ); break;
//        case 142: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[ 9]       ); break;
//        case 143: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[10]       ); break;
//        case 144: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[11]       ); break;
//        case 145: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[12]       ); break;
//        case 146: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[13]       ); break;
//        case 147: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[14]       ); break;
//        case 148: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[15]       ); break;
//        case 149: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[16]       ); break;
//        case 150: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[17]       ); break;
//        case 151: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[18]       ); break;
//        case 152: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[19]       ); break;
//        case 153: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[20]       ); break;
//        case 154: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[21]       ); break;
//        case 155: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[22]       ); break;
//        case 156: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[23]       ); break;
//        case 157: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[24]       ); break;
//        case 158: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[25]       ); break;
//        case 159: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[26]       ); break;
//        case 160: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[27]       ); break;
//        case 161: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[28]       ); break;
//        case 162: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[29]       ); break;
//        case 163: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[30]       ); break;
//        case 164: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Phase_CodeSeq[31]       ); break;
//        case 165: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Freq_FreqType           ); break;
//        case 166: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Freq_FreqSeq[0]         ); break;
//        case 167: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Freq_FreqSeq[1]         ); break;
//        case 168: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Freq_FreqSeq[2]         ); break;
//        case 169: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ScreenPulse_Freq_FreqSeq[3]         ); break;
//        case 170: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_Mode_TransSigMode             ); break;
//        case 171: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_PowerType[0]        ); break;
//        case 172: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_DelayClkNum[0]      ); break;
//        case 173: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_res[0]              ); break;
//        case 174: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_DopplerCtrl[0]      ); break;
//        case 175: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_UpGain[0]           ); break;
//        case 176: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_PowerType[1]        ); break;
//        case 177: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_DelayClkNum[1]      ); break;
//        case 178: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_res[1]              ); break;
//        case 179: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_DopplerCtrl[1]      ); break;
//        case 180: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_UpGain[1]           ); break;
//        case 181: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_PowerType[2]        ); break;
//        case 182: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_DelayClkNum[2]      ); break;
//        case 183: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_res[2]              ); break;
//        case 184: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_DopplerCtrl[2]      ); break;
//        case 185: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_UpGain[2]           ); break;
//        case 186: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_PowerType[3]        ); break;
//        case 187: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_DelayClkNum[3]      ); break;
//        case 188: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_res[3]              ); break;
//        case 189: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_DopplerCtrl[3]      ); break;
//        case 190: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_UpGain[3]           ); break;
//        case 191: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_PowerType[4]        ); break;
//        case 192: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_DelayClkNum[4]      ); break;
//        case 193: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_res[4]              ); break;
//        case 194: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_DopplerCtrl[4]      ); break;
//        case 195: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_MainPulse_UpGain[4]           ); break;
//        case 196: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_ScreenPulse_PowerType         ); break;
//        case 197: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_ScreenPulse_DelayClkNum       ); break;
//        case 198: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_ScreenPulse_res               ); break;
//        case 199: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_ScreenPulse_DopplerCtrl       ); break;
//        case 200: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Trans_ScreenPulse_UpGain            ); break;
//        case 201: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_Gain[0]                 ); break;
//        case 202: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_DelayClk[0]             ); break;
//        case 203: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_SampleGateClk[0]        ); break;
//        case 204: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_DopplerCtrl[0]          ); break;
//        case 205: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_SumChCoe[0]             ); break;
//        case 206: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AziChCoe[0]             ); break;
//        case 207: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_EleChCoe[0]             ); break;
//        case 208: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh1Coe[0]            ); break;
//        case 209: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh2Coe[0]            ); break;
//        case 210: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh3Coe[0]            ); break;
//        case 211: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh4Coe[0]            ); break;
//        case 212: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh5Coe[0]            ); break;
//        case 213: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_Gain[1]                 ); break;
//        case 214: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_DelayClk[1]             ); break;
//        case 215: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_SampleGateClk[1]        ); break;
//        case 216: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_DopplerCtrl[1]          ); break;
//        case 217: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_SumChCoe[1]             ); break;
//        case 218: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AziChCoe[1]             ); break;
//        case 219: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_EleChCoe[1]             ); break;
//        case 220: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh1Coe[1]            ); break;
//        case 221: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh2Coe[1]            ); break;
//        case 222: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh3Coe[1]            ); break;
//        case 223: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh4Coe[1]            ); break;
//        case 224: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh5Coe[1]            ); break;
//        case 225: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_Gain[2]                 ); break;
//        case 226: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_DelayClk[2]             ); break;
//        case 227: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_SampleGateClk[2]        ); break;
//        case 228: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_DopplerCtrl[2]          ); break;
//        case 229: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_SumChCoe[2]             ); break;
//        case 230: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AziChCoe[2]             ); break;
//        case 231: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_EleChCoe[2]             ); break;
//        case 232: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh1Coe[2]            ); break;
//        case 233: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh2Coe[2]            ); break;
//        case 234: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh3Coe[2]            ); break;
//        case 235: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh4Coe[2]            ); break;
//        case 236: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh5Coe[2]            ); break;
//        case 237: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_Gain[3]                 ); break;
//        case 238: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_DelayClk[3]             ); break;
//        case 239: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_SampleGateClk[3]        ); break;
//        case 240: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_DopplerCtrl[3]          ); break;
//        case 241: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_SumChCoe[3]             ); break;
//        case 242: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AziChCoe[3]             ); break;
//        case 243: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_EleChCoe[3]             ); break;
//        case 244: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh1Coe[3]            ); break;
//        case 245: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh2Coe[3]            ); break;
//        case 246: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh3Coe[3]            ); break;
//        case 247: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh4Coe[3]            ); break;
//        case 248: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh5Coe[3]            ); break;
//        case 249: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_Gain[4]                 ); break;
//        case 250: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_DelayClk[4]             ); break;
//        case 251: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_SampleGateClk[4]        ); break;
//        case 252: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_DopplerCtrl[4]          ); break;
//        case 253: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_SumChCoe[4]             ); break;
//        case 254: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AziChCoe[4]             ); break;
//        case 255: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_EleChCoe[4]             ); break;
//        case 256: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh1Coe[4]            ); break;
//        case 257: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh2Coe[4]            ); break;
//        case 258: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh3Coe[4]            ); break;
//        case 259: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh4Coe[4]            ); break;
//        case 260: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ChPara_Recv_AuxCh5Coe[4]            ); break;
//        case 261: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_TarNum                      ); break;
//        case 262: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_Type                        ); break;
//        case 263: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[ 0]                 ); break;
//        case 264: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[ 1]                 ); break;
//        case 265: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[ 2]                 ); break;
//        case 266: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[ 3]                 ); break;
//        case 267: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[ 4]                 ); break;
//        case 268: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[ 5]                 ); break;
//        case 269: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[ 6]                 ); break;
//        case 270: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[ 7]                 ); break;
//        case 271: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[ 8]                 ); break;
//        case 272: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[ 9]                 ); break;
//        case 273: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[10]                 ); break;
//        case 274: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[11]                 ); break;
//        case 275: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[12]                 ); break;
//        case 276: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[13]                 ); break;
//        case 277: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[14]                 ); break;
//        case 278: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[15]                 ); break;
//        case 279: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[16]                 ); break;
//        case 280: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[17]                 ); break;
//        case 281: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[18]                 ); break;
//        case 282: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[19]                 ); break;
//        case 283: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[20]                 ); break;
//        case 284: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[21]                 ); break;
//        case 285: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[22]                 ); break;
//        case 286: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[23]                 ); break;
//        case 287: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[24]                 ); break;
//        case 288: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[25]                 ); break;
//        case 289: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[26]                 ); break;
//        case 290: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[27]                 ); break;
//        case 291: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[28]                 ); break;
//        case 292: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[29]                 ); break;
//        case 293: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[30]                 ); break;
//        case 294: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_RCSCoef[31]                 ); break;
//        case 295: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[ 0]                ); break;
//        case 296: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[ 0]             ); break;
//        case 297: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[ 0]                  ); break;
//        case 298: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[ 0]                  ); break;
//        case 299: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[ 0]               ); break;
//        case 300: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[ 1]                ); break;
//        case 301: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[ 1]             ); break;
//        case 302: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[ 1]                  ); break;
//        case 303: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[ 1]                  ); break;
//        case 304: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[ 1]               ); break;
//        case 305: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[ 2]                ); break;
//        case 306: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[ 2]             ); break;
//        case 307: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[ 2]                  ); break;
//        case 308: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[ 2]                  ); break;
//        case 309: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[ 2]               ); break;
//        case 310: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[ 3]                ); break;
//        case 311: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[ 3]             ); break;
//        case 312: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[ 3]                  ); break;
//        case 313: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[ 3]                  ); break;
//        case 314: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[ 3]               ); break;
//        case 315: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[ 4]                ); break;
//        case 316: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[ 4]             ); break;
//        case 317: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[ 4]                  ); break;
//        case 318: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[ 4]                  ); break;
//        case 319: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[ 4]               ); break;
//        case 320: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[ 5]                ); break;
//        case 321: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[ 5]             ); break;
//        case 322: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[ 5]                  ); break;
//        case 323: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[ 5]                  ); break;
//        case 324: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[ 5]               ); break;
//        case 325: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[ 6]                ); break;
//        case 326: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[ 6]             ); break;
//        case 327: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[ 6]                  ); break;
//        case 328: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[ 6]                  ); break;
//        case 329: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[ 6]               ); break;
//        case 330: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[ 7]                ); break;
//        case 331: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[ 7]             ); break;
//        case 332: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[ 7]                  ); break;
//        case 333: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[ 7]                  ); break;
//        case 334: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[ 7]               ); break;
//        case 335: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[ 8]                ); break;
//        case 336: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[ 8]             ); break;
//        case 337: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[ 8]                  ); break;
//        case 338: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[ 8]                  ); break;
//        case 339: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[ 8]               ); break;
//        case 340: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[ 9]                ); break;
//        case 341: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[ 9]             ); break;
//        case 342: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[ 9]                  ); break;
//        case 343: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[ 9]                  ); break;
//        case 344: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[ 9]               ); break;
//        case 345: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[10]                ); break;
//        case 346: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[10]             ); break;
//        case 347: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[10]                  ); break;
//        case 348: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[10]                  ); break;
//        case 349: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[10]               ); break;
//        case 350: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[11]                ); break;
//        case 351: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[11]             ); break;
//        case 352: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[11]                  ); break;
//        case 353: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[11]                  ); break;
//        case 354: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[11]               ); break;
//        case 355: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[12]                ); break;
//        case 356: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[12]             ); break;
//        case 357: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[12]                  ); break;
//        case 358: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[12]                  ); break;
//        case 359: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[12]               ); break;
//        case 360: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[13]                ); break;
//        case 361: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[13]             ); break;
//        case 362: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[13]                  ); break;
//        case 363: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[13]                  ); break;
//        case 364: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[13]               ); break;
//        case 365: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[14]                ); break;
//        case 366: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[14]             ); break;
//        case 367: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[14]                  ); break;
//        case 368: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[14]                  ); break;
//        case 369: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[14]               ); break;
//        case 370: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[15]                ); break;
//        case 371: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[15]             ); break;
//        case 372: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[15]                  ); break;
//        case 373: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[15]                  ); break;
//        case 374: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[15]               ); break;
//        case 375: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[16]                ); break;
//        case 376: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[16]             ); break;
//        case 377: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[16]                  ); break;
//        case 378: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[16]                  ); break;
//        case 379: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[16]               ); break;
//        case 380: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[17]                ); break;
//        case 381: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[17]             ); break;
//        case 382: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[17]                  ); break;
//        case 383: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[17]                  ); break;
//        case 384: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[17]               ); break;
//        case 385: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[18]                ); break;
//        case 386: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[18]             ); break;
//        case 387: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[18]                  ); break;
//        case 388: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[18]                  ); break;
//        case 389: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[18]               ); break;
//        case 390: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[19]                ); break;
//        case 391: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[19]             ); break;
//        case 392: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[19]                  ); break;
//        case 393: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[19]                  ); break;
//        case 394: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[19]               ); break;
//        case 395: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[20]                ); break;
//        case 396: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[20]             ); break;
//        case 397: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[20]                  ); break;
//        case 398: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[20]                  ); break;
//        case 399: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[20]               ); break;
//        case 400: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[21]                ); break;
//        case 401: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[21]             ); break;
//        case 402: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[21]                  ); break;
//        case 403: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[21]                  ); break;
//        case 404: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[21]               ); break;
//        case 405: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[22]                ); break;
//        case 406: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[22]             ); break;
//        case 407: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[22]                  ); break;
//        case 408: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[22]                  ); break;
//        case 409: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[22]               ); break;
//        case 410: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[23]                ); break;
//        case 411: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[23]             ); break;
//        case 412: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[23]                  ); break;
//        case 413: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[23]                  ); break;
//        case 414: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[23]               ); break;
//        case 415: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[24]                ); break;
//        case 416: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[24]             ); break;
//        case 417: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[24]                  ); break;
//        case 418: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[24]                  ); break;
//        case 419: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[24]               ); break;
//        case 420: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[25]                ); break;
//        case 421: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[25]             ); break;
//        case 422: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[25]                  ); break;
//        case 423: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[25]                  ); break;
//        case 424: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[25]               ); break;
//        case 425: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[26]                ); break;
//        case 426: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[26]             ); break;
//        case 427: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[26]                  ); break;
//        case 428: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[26]                  ); break;
//        case 429: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[26]               ); break;
//        case 430: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[27]                ); break;
//        case 431: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[27]             ); break;
//        case 432: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[27]                  ); break;
//        case 433: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[27]                  ); break;
//        case 434: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[27]               ); break;
//        case 435: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[28]                ); break;
//        case 436: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[28]             ); break;
//        case 437: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[28]                  ); break;
//        case 438: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[28]                  ); break;
//        case 439: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[28]               ); break;
//        case 440: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[29]                ); break;
//        case 441: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[29]             ); break;
//        case 442: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[29]                  ); break;
//        case 443: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[29]                  ); break;
//        case 444: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[29]               ); break;
//        case 445: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[30]                ); break;
//        case 446: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[30]             ); break;
//        case 447: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[30]                  ); break;
//        case 448: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[30]                  ); break;
//        case 449: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[30]               ); break;
//        case 450: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DelayClk[31]                ); break;
//        case 451: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_DopplerCtrl[31]             ); break;
//        case 452: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_AziCoe[31]                  ); break;
//        case 453: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_EleCoe[31]                  ); break;
//        case 454: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, TarPara_MainPower[31]               ); break;
//        case 455: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ClutterPara_Mode                    ); break;
//        case 456: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ClutterPara_MainLobe_DelayClkNum    ); break;
//        case 457: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ClutterPara_MainLobe_RangeNum       ); break;
//        case 458: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ClutterPara_MainLobe_DopplerCenter  ); break;
//        case 459: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ClutterPara_MainLobe_DopplerB       ); break;
//        case 460: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ClutterPara_MainLobe_Power          ); break;
//        case 461: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ClutterPara_Height_DelayClkNum      ); break;
//        case 462: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ClutterPara_Height_RangeNum         ); break;
//        case 463: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ClutterPara_Height_DopplerCenter    ); break;
//        case 464: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ClutterPara_Height_DopplerB         ); break;
//        case 465: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, ClutterPara_Height_Power            ); break;
//        case 466: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Recv_JieWei                         ); break;
//        case 467: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Recv_NoisePower                     ); break;
//        case 468: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Noise_SampleGateDelayCtrl           ); break;
//        case 469: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Noise_SampleNumCtrl                 ); break;
//        case 470: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, Noise_Power                         ); break;
//        case 471: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, NCO                                 ); break;
//        case 472: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, EchoSupple_ChnNum                   ); break;
//        case 473: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, EchoSupple_SampleNum                ); break;
//        case 474: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, JamZeroRepa_Before                  ); break;
//        case 475: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, JamZeroRepa_Behind                  ); break;
//        case 476: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, NLFM_Echo_FrepCtrl                  ); break;
//        case 477: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, NLFM_Echo_AmpCtrl                   ); break;
//        case 478: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, NLFM_Trans_FrepCtrl                 ); break;
//        case 479: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, NLFM_Trans_AmpCtrl                  ); break;
//        case 480: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_TransCtrl_GateDelay[0]       ); break;
//        case 481: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_TransCtrl_GateWidth[0]       ); break;
//        case 482: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_RecvCtrl_GateDelay[0]        ); break;
//        case 483: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_RecvCtrl_GateWidth[0]        ); break;
//        case 484: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_TransCtrl_GateDelay[1]       ); break;
//        case 485: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_TransCtrl_GateWidth[1]       ); break;
//        case 486: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_RecvCtrl_GateDelay[1]        ); break;
//        case 487: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_RecvCtrl_GateWidth[1]        ); break;
//        case 488: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_TransCtrl_GateDelay[2]       ); break;
//        case 489: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_TransCtrl_GateWidth[2]       ); break;
//        case 490: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_RecvCtrl_GateDelay[2]        ); break;
//        case 491: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_RecvCtrl_GateWidth[2]        ); break;
//        case 492: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_TransCtrl_GateDelay[3]       ); break;
//        case 493: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_TransCtrl_GateWidth[3]       ); break;
//        case 494: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_RecvCtrl_GateDelay[3]        ); break;
//        case 495: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_RecvCtrl_GateWidth[3]        ); break;
//        case 496: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_TransCtrl_GateDelay[4]       ); break;
//        case 497: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_TransCtrl_GateWidth[4]       ); break;
//        case 498: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_RecvCtrl_GateDelay[4]        ); break;
//        case 499: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, RFCtrl_RecvCtrl_GateWidth[4]        ); break;
//        case 500: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 501: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 502: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 503: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 504: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 505: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 506: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 507: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 508: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 509: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 510: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 511: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 512: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 513: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 514: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 515: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 516: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 517: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 518: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 519: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 520: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 521: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 522: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 523: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 524: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 525: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 526: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 527: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//    
//        case 528: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000                          ); break;
//        case 529: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x5A5A55AA                          ); break;
//        case 530: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0xAAAAAAAA                          ); break;
//        case 531: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0xAAAAAAAA                          ); break;
//        
//        default:break;
//        }
//    }
//    for(int j = 0;j<(24+4096);j++){//匹配函数包 max = 24+8192
//        if((j>19)&&(j<=19+4096)){
//            Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000        );
//        }
//        else {
//            switch (j)
//            {
//            case 0 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x01DCEF18       ); break;
//            case 1 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0xAA5555AA       ); break;
//            case 2 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x0000000B       ); break;
//            case 3 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000001       ); break;
//            case 4 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000001       ); break;
//            case 5 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000001       ); break;
//            case 6 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00001018       ); break;
//            case 7 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000200       ); break;
//            case 8 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 9 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 10: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 11: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 12: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 13: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 14: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 15: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//
//            case 16: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 17: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 18: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 19: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//
//            case 4096+20:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 4096+21:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x5A5A55AA       ); break;
//            case 4096+22:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0xAAAAAAAA       ); break;
//            case 4096+23:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0xAAAAAAAA       ); break;
//                break;
//            default:
//                break;
//            }
//        }
//    }
//    for(int k = 0;k<(30+4096);k++){//杂波参数包
//        if((k>19)&&(k<=19+4096)){
//            Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000        );
//        }
//        else{
//            switch (k)
//            {
//            case 0 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x01DCEF18       ); break;
//            case 1 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0xAA5555AA       ); break;
//            case 2 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x0000000C       ); break;
//            case 3 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000001       ); break;
//            case 4 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000001       ); break;
//            case 5 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000001       ); break;
//            case 6 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x0000101E       ); break;
//            case 7 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000200       ); break;
//            case 8 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 9 : Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 10: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 11: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 12: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 13: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 14: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 15: Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//
//            case 4096+20:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 4096+21:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 4096+22:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 4096+23:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 4096+24:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 4096+25:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 4096+26:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x00000000       ); break;
//            case 4096+27:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0x5A5A55AA       ); break;
//            case 4096+28:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0xAAAAAAAA       ); break;
//            case 4096+29:Xil_Out32(FIFO_SEND_BASE_ADDR + 0x10, 0xAAAAAAAA       ); break;
//                break;
//            default:
//                break;
//            }
//        }
//    }
//}

int main(){
    //Xil_Out32(stop, 0x0001);
    //Xil_Out32(stop, 0x0000);
    //while(1){
    //Xil_Out32(RF_ask_flag, 0x0001);
    //Xil_Out32(RF_ask_flag, 0x0000);
    //}
    Xil_Out32(RF_ask_flag, 0x0001);
    Xil_Out32(RF_ask_flag, 0x0000);
    Xil_Out32(init, 0x0001);
    Xil_Out32(init, 0x0000);

    while(1){
    Xil_Out32(RF_FREQ, 0x18381838);
    Xil_Out32(RF_UP_GAIN, 0x0000);
    Xil_Out32(RF_DOWN_GAIN, 0x0000);
    Xil_Out32(RF_ctrl, 0x0001);
    Xil_Out32(RF_ctrl, 0x0000);

    Xil_Out32(RF_pre_cpi, 0x0001);
    Xil_Out32(RF_pre_cpi, 0x0000);
    }
    
}