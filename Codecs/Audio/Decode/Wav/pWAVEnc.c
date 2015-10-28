/*
********************************************************************************
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* File Name：   pWAVEnc.c
*
* Description:  WAV coding processing control.
*
* History:      <author>          <time>        <version>
*               WangBo           2009-4-16          1.0
*    desc:    ORG.
********************************************************************************
*/


#include  "SysInclude.h"
#include  "audio_main.h"

//#ifdef A_CORE_DECODE
#ifdef ADPCM_ENC_INCLUDE


#include  "FsInclude.h"
#include  "File.h"
#include  "FDT.h"

#include  "pcm.h"
#include  "sf_wav.h"

#include  "RecordControl.h"

extern  SF_PRIVATE   sf_enc;


//to filter the signal its sampling rate is 8k or 16k with filter that band-pass frequency is from 200 to 3600.
#if 0
#define N 15
#define Q15 (1<<N)
#define sat(x) (short)((x)>32767?32767:((x)<-32768?-32768:(x)));
_ATTR_RECORD_CONTROL_DATA_                  static int coef[2][4]={(int)(-0.462938*Q15),(int)(0.209715*Q15),(int)(0.186694*Q15),(int)(0.373388*Q15),//截止频率为3KHz的低通滤波系数
                                                                              (int)(-1.561018*Q15),(int)(0.641352*Q15),(int)(0.800592*Q15),(int)( -1.601185*Q15)};//截止频率为800Hz的高通滤波器系数
_ATTR_RECORD_CONTROL_DATA_            static int x[2][3]={0};
_ATTR_RECORD_CONTROL_DATA_            static int y[2][3]={0};
//filter
_ATTR_RECORD_CONTROL_CODE_
void filter_init()
{
    int i=0,j=0;

    for(;j<2;j++)
    {
        for(;i<3;i++)
        {
            x[j][i]=0;
            y[j][i]=0;
        }
    }
}

///////////////////////////////////////////////
//filting
//*asInputData: a pointer to audio signal
//len:sample number
//numofchannel:channel number
//samplerate:sample rate：8000、16000
//filte 16k signal
//*asInputData: a pointer to audio signal
//len:sample number
//ch:channel number
//mode：filter mode
_ATTR_RECORD_CONTROL_CODE_
void filter_pro(signed short *asInputData, int len,int ch,int mode)
{
    int i,j=mode-1;
    int a2=coef[j][0],a3=coef[j][1],b1=coef[j][2],b2=coef[j][3];

    for(j=0;j<ch;j++)
    for(i=0;i<len;i++)
    {
        //
        x[j][0]=(int)asInputData[ch*i+j];
        y[j][0]=b1*x[j][0]+b2*x[j][1]+b1*x[j][2]-((__int64)a2*y[j][1]>>N)-((__int64)a3*y[j][2]>>N);
        y[j][2]=y[j][1];
        y[j][1]=y[j][0];
        x[j][2]=x[j][1];
        x[j][1]=x[j][0];
        asInputData[ch*i+j]=sat(y[j][0]>>N);
    }
}

#else

#define N 13

#define Q15 (1<<N)
#define sat(x) (short)((x)>32767?32767:((x)<-32768?-32768:(x)));
_ATTR_RECORD_CONTROL_DATA_            static int coef[2][4]={(int)(-0.462938*Q15),(int)(0.209715*Q15),(int)(0.186694*Q15),(int)(0.373388*Q15),//截止频率为3KHz的低通滤波系数
                                                                                 (int)(-1.561018*Q15),(int)(0.641352*Q15),(int)(0.800592*Q15),(int)( -1.601185*Q15)};//截止频率为800Hz的高通滤波器系数
_ATTR_RECORD_CONTROL_DATA_            static int x[2][3]={0};
_ATTR_RECORD_CONTROL_DATA_            static int y[2][3]={0};

_ATTR_RECORD_CONTROL_DATA_            static int x1[2][6]={0};
_ATTR_RECORD_CONTROL_DATA_            static int y1[2][6]={0};
// 6 level a2,a3,a4,a5,a6,b1,b2,b3,b4=-b3,b5=-b2,b6=-b1
// 1.000000000000000  -4.431342075207672   7.899109718924292  -7.065239137143228   3.163635689026466  -0.565450405391308
// 0.763175346659711  -3.778667082292422   7.520546083894349  -7.520546083894351   3.778667082292425  -0.763175346659712

_ATTR_RECORD_CONTROL_DATA_ static int coef1[2][8]={(int)(-4.431342*Q15),(int)(7.8991097*Q15),(int)(-7.065239*Q15),(int)(3.163636*Q15),(int)(-0.565450*Q15),(int)(0.763175*Q15),(int)(-3.778667*Q15),(int)(7.520546*Q15),
                                                                   (int)(-2.826135*Q15),(int)(4.221138*Q15), (int)(-3.577799*Q15),(int)(1.773972*Q15),(int)(-0.402551*Q15),(int)(0.058809*Q15),(int)(-0.024894*Q15),(int)(0.060396*Q15)};   // 2.5khz-30DB,300hz

/*
_ATTR_RECORD_CONTROL_DATA_ static int coef1[2][8]={(int)(-4.431342*Q15),(int)(7.8991097*Q15),(int)(-7.065239*Q15),(int)(3.163636*Q15),(int)(-0.565450*Q15),(int)(0.763175*Q15),(int)(-3.778667*Q15),(int)(7.520546*Q15),
                                                                   (int)( -2.936004*Q15),(int)(4.367064*Q15),(int)(-3.706414*Q15),(int)(1.805656*Q15),(int)(-0.403622*Q15),(int)(0.022322*Q15),(int)( 0.009902*Q15),(int)(0.031116*Q15)};   // 2.5K-40dB};
*/
/*
_ATTR_RECORD_CONTROL_DATA_ static int coef1[2][8]={(int)(-4.431342*Q15),(int)(7.8991097*Q15),(int)(-7.065239*Q15),(int)(3.163636*Q15),(int)(-0.565450*Q15),(int)(0.763175*Q15),(int)(-3.778667*Q15),(int)(7.520546*Q15),
                                                                   (int)(-3.436328*Q15),(int)(5.471755*Q15), (int)(-4.787579*Q15),(int)(2.303161*Q15),(int)(-0.484232*Q15),(int)(0.043798*Q15),(int)(-0.053167*Q15),(int)(0.042758*Q15)};   // 2khz,300hz
*/
/*
_ATTR_RECORD_CONTROL_DATA_ static int coef1[2][8]={(int)(-4.431342*Q15),(int)(7.8991097*Q15),(int)(-7.065239*Q15),(int)(3.163636*Q15),(int)(-0.565450*Q15),(int)(0.763175*Q15),(int)(-3.778667*Q15),(int)(7.520546*Q15),    // 300hz
                                                                     (int)(-0.669072*Q15),(int)(1.660651*Q15), (int)(-0.859230*Q15),(int)(0.679224*Q15),(int)(-0.218268*Q15),(int)(0.131257*Q15),(int)( 0.256451*Q15),(int)(0.408943*Q15)};   // 4K
*/
/////////////////////

//filter initialization.
_ATTR_RECORD_CONTROL_CODE_
void filter_init()
{
      int i=0,j=0;

      for(;j<2;j++)
      {
          for(;i<3;i++)
          {
                x[j][i]=0;
                y[j][i]=0;
          }
      }
}

//to do filter the signal that sampling rate is 16k.
//*asInputData: point to audio signal.
//len:the total of spectrums.(no care of signal channel or two channel.)
_ATTR_RECORD_CONTROL_CODE_
void filter_pro(signed short *asInputData, int len,int ch,int mode)
{
    int i,j=mode-1;
    int a2=coef[j][0],a3=coef[j][1],b1=coef[j][2],b2=coef[j][3];

    for(j=0;j<ch;j++)
    {
        for(i=0;i<len;i++)
        {
            //
            x[j][0]=(int)asInputData[ch*i+j];
            y[j][0]=b1*x[j][0]+b2*x[j][1]+b1*x[j][2]-((__int64)a2*y[j][1]>>N)-((__int64)a3*y[j][2]>>N);
            y[j][2]=y[j][1];
            y[j][1]=y[j][0];
            x[j][2]=x[j][1];
            x[j][1]=x[j][0];
            asInputData[ch*i+j]=sat(y[j][0]>>N);
        }
    }
}

////////////////////////
_ATTR_RECORD_CONTROL_CODE_
void filter_initI()
{
      memset(x1,0,sizeof(int)*12);
      memset(y1,0,sizeof(int)*12);
}

_ATTR_RECORD_CONTROL_CODE_
void filter_proI(signed short *asInputData, int len,int mode)
{
    int i,j=mode-1;
    int a2=coef1[j][0],a3=coef1[j][1],a4=coef1[j][2],a5=coef1[j][3],a6=coef1[j][4],b1=coef1[j][5],b2=coef1[j][6],b3=coef1[j][7];
    int b4=b3,b5=b2,b6=b1;

    if(mode==1)
    {
        b4=-b4;
        b5=-b5;
        b6=-b6;
    }
    for(i=0;i<len;i++)
    {
        //
        x1[j][0]=(int)asInputData[i];
        y1[j][0]=b1*x1[j][0]+b2*x1[j][1]+b3*x1[j][2]+b4*x1[j][3]+b5*x1[j][4]+b6*x1[j][5]-((__int64)a2*y1[j][1]>>N)-((__int64)a3*y1[j][2]>>N)-((__int64)a4*y1[j][3]>>N)-((__int64)a5*y1[j][4]>>N)-((__int64)a6*y1[j][5]>>N);
        //y1[j][0]=b1*x1[j][0]+b2*x1[j][1]+b3*x1[j][2]+b4*x1[j][3]+b5*x1[j][4]+b6*x1[j][5]-a2*y1[j][1]-a3*y1[j][2]-a4*y1[j][3]-a5*y1[j][4]-a6*y1[j][5];
        y1[j][5]=y1[j][4];
        y1[j][4]=y1[j][3];
        y1[j][3]=y1[j][2];
        y1[j][2]=y1[j][1];
        y1[j][1]=y1[j][0];
        x1[j][5]=x1[j][4];
        x1[j][4]=x1[j][3];
        x1[j][3]=x1[j][2];
        x1[j][2]=x1[j][1];
        x1[j][1]=x1[j][0];
        asInputData[i]=sat((y1[j][0]>>N));
    }
}

_ATTR_RECORD_CONTROL_DATA_   int ref[15]={0};
_ATTR_RECORD_CONTROL_DATA_   int w[15]={0};
_ATTR_RECORD_CONTROL_DATA_   int mu=(int)(0.001*32768);
//filter initialization
_ATTR_RECORD_CONTROL_CODE_
short threshold(short in)
{
      short value;

      if(in<1000&&in>-800)
      {
            value=0;
      }
    else
    {
            value=in;
    }
    return value;
}

_ATTR_RECORD_CONTROL_DATA_ short last=0;
_ATTR_RECORD_CONTROL_DATA_ short last1=0;
_ATTR_RECORD_CONTROL_CODE_
short threshold1(short in)
{

      short value;

      if(in<800&&in>-500)
      {
            //value=0.5*in+0.2*last;
            value=((int)16*in+(int)6*last)>>5;
      }
      else
      {
            value=((int)25*in+(int)6*last)>>5;
      }

      last=value;

    return value;
}


_ATTR_RECORD_CONTROL_CODE_
void lms_filter(short *in,int len)
{
      int i,j;
      int y,err;

      for(j=0;j<len;j++)
      {
            //err=threshold1(in[j]);
            err=in[j];
            in[j]=((int)(last1+err))>>1;
            last1=err;

      }
      for(j=0;j<len;j++)
      {
            y=0;
            ref[9]=in[j];  //Q15
            for(i=0;i<15;i++)
            {
                  y+=((w[i]*ref[i])>>18);  //Q30=Q15*15
            }

            //DEBUG("this y[%d]=%f\n",j,y/32768.0);
            err=threshold(in[j])-y; //Q30=(Q15<<15)-Q30
            for(i=0;i<15;i++)
            {
                  w[i]+=(((__int64)mu*ref[i]*err)>>22);  //Q20
            }

            for(i=0;i<14;i++)
            {
                  ref[i]=ref[i+1];
            }
            //DEBUG("%f  ",y);
            in[j]=(short)(y);
      }
}
#endif

//record extern variable
//------------------------------------------------------------------------------
//extern RecordBlock gRecordBlock;
extern UINT32 gEncodeDone;

//UINT16 DmaTranferCallback;

//static void DMATranferCallBack(void)
//{
//    DmaTranferCallback = 1;
//}

//------------------------------------------------------------------------------

_ATTR_RECORD_CONTROL_CODE_
void ADPCM_Init(void * ptr)
{
    #ifndef A_CORE_DECODE

    gEncodeDone = 0;
    MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_ENCODE_INIT, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
    MailBoxWriteA2BData((uint32)ptr, MAILBOX_ID_0, MAILBOX_CHANNEL_1);

    while (!gEncodeDone)
    {
#ifdef _LOG_DEBUG_
        BBDebug();
#endif
        __WFI();
    }
    gEncodeDone = 0;

    #endif

}

/*
--------------------------------------------------------------------------------
  Function name : void WavEncodeVariableInit()
  Author        : WangBo
  Description   :

  Input         :
  Return        :

  History:     <author>         <time>         <version>
                 WangBo         2009-4-16          1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
_ATTR_RECORD_CONTROL_CODE_
void WavEncodeVariableInit(void)
{
    UINT16 i;

    filter_init();

    RecordInputBufferIndex   = 0;   //used for buffer switch.
    pRecordInputBuffer       = (UINT16*)WavEncodeInputBuffer;
    pRecordPCMInputBuffer    = pRecordInputBuffer;
    pRecordEncodeInputBuffer = pRecordInputBuffer + WAV_AD_PIPO_BUFFER_SIZE;

    for (i = 0; i < WAV_AD_PIPO_BUFFER_SIZE * 2; i++)
    {
        *(pRecordInputBuffer + i) = 0;
    }

    //WriteBuffer initialization.
    WriteBuffer.uHead     = 0;
    WriteBuffer.uTail     = 0;
    WriteBuffer.uCounter  = 0;

    //cache buffer for write initialization.
    for (i = 0; i < WAV_ENCODER_WRITE_BUFFER_LENGTH; i++)
    {
        WriteBuffer.Data[i] = 0;
    }

    DEBUG("[RECORD] WavEncodeVariableInit start.");

    PCM_s.usSampleRate = RecordSampleRate;    //sampling rate.
    PCM_s.ucChannels = RecordChanel;


#ifdef A_CORE_DECODE
    InitADPCMEncoder(&PCM_s);

    sf_enc.sf.channels = PCM_s.ucChannels;
    sf_enc.sf.samplerate = PCM_s.usSampleRate;
    sf_enc.sf.format = (SF_FORMAT_WAV | SF_FORMAT_MS_ADPCM);
    sf_enc.mode = SFM_WRITE;
    sf_enc.datalength = PCM_s.ulLength;

    msadpcm_enc_init (&sf_enc, PCM_s.usBytesPerBlock, PCM_s.usSamplesPerBlock);
#else

    ADPCM_Init(&PCM_s);

#endif
    DEBUG("[RECORD] WavEncodeVariableInit over");

    DEBUG("[RECORD] channls = %d samplerate = %d",PCM_s.ucChannels,PCM_s.usSampleRate);
    DEBUG("[RECORD] usBytesPerBlock = %d usSamplesPerBlock = %d",PCM_s.usBytesPerBlock, PCM_s.usSamplesPerBlock);
    RecordInputBufferLength = PCM_s.usSamplesPerBlock * RecordChanel;     //the number of AD sampling.

    RecordSampleCounter[0] = 0; //ch 0
    RecordSampleCounter[1] = 0; //ch 1

    /////////////////////////////////////////////////////////////////////////
    //zyz
    if (RECORD_QUALITY_HIGH == gbRecordQuality)
    {
        RecordCounterPerSamples = 2;                        //get 1 point from 2 sampling points.
    }
    else
    {
        RecordCounterPerSamples = 2; // 4;                        //get 1 point from 4 sampling points.
    }

    /////////////////////////////////////////////////////////////////////////

    RecordEncodeCounter = RecordInputBufferLength * RecordCounterPerSamples;

    RecordFileOffset   = 0;

    RecordConFlag = RECORD_CON_NULL;                     //the flag that be used to control coding.
    RecordStaFlag = RECORD_STA_PCMBUF_EMPTY;             //the flag that be used to control input buffer empty or full.

    RecordNOISECount = RECORD_STARTNOISEBLOCK;
}

#if 0
/*
--------------------------------------------------------------------------------
  Function name : void WavEncodeIsr()
  Author        : WangBo
  Description   : interrupt callback to record.
                  新修改的编码中断函数只将编码的数据写入写缓冲Buffer,
                  写文件放在 WinService 中处理。
                  可以解决 flash 写入速度慢的问题，可能不会出现断音的问题
                  the interrupt function that is be changed newly just only write the data to cache buffer.
                  write to file service be put in WinService.
                  it can solve the slowly write problem,may don't happen the phenomenon of staccato .
  Input         :
  Return        :

  History:     <author>         <time>         <version>
               WangBo           2009-4-16          1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
_ATTR_RECORD_CONTROL_CODE_
void WavEncodeIsr(void)
{
    UINT16 i;
    UINT16  uEncodeDataLength;
    UINT16  uSamples;           // 编码的样本数.
    UINT16  *pSampleBuffer;

    if (RecordConFlag & RECORD_CON_ENCODE_ENABLE) //code enter into force.
    {
        if (RecordStaFlag & RECORD_STA_PCMBUF_FULL) //input buffer is full.
        {
            #if 0
            if (RecordMuteFrameCount < RecordNOISECount)   //removal the noise of record beginning.
            {
                if (++RecordMuteFrameCount == RecordNOISECount)
                {
                    RecordNOISECount = RECORD_KEYNOISEBLOCK;
                }
                else
                {
                    RecordStaFlag &= ~RECORD_STA_PCMBUF_FULL;
                    RecordStaFlag |=  RECORD_STA_PCMBUF_EMPTY;
                    return;
                }
            }
            #endif
            pSampleBuffer =  pRecordEncodeInputBuffer;


                  ////////////fliter
                    #if 0
                  if(RecordType == RECORD_TYPE_MIC)
                  {
                    filter_proI(pSampleBuffer,RecordInputBufferLength,1);
                   filter_proI(pSampleBuffer,RecordInputBufferLength,2);
                  }
            #endif

#ifdef A_CORE_DECODE
            // coding
            if (EncodeType == RECORD_ENCODE_TYPE_WAV)
            {
                uEncodeDataLength = msadpcm_write_s(&sf_enc, pSampleBuffer , RecordInputBufferLength, &WavEncodeBuf);   //msadpcm_write_s (&sf, &out[0] , 1152);
            }
            else if (EncodeType == RECORD_ENCODE_TYPE_PCM)
            {
                WavEncodeBuf = (uint8*)pSampleBuffer;
                uEncodeDataLength = RecordInputBufferLength * 2;
            }


            if (uEncodeDataLength + WriteBuffer.uCounter <= WAV_ENCODER_WRITE_BUFFER_LENGTH)
            {
                      char *pp = (char *)pSampleBuffer;
                for (i = 0; i < uEncodeDataLength; i++)
                {
                    UINT16  uTail;

                    uTail = WriteBuffer.uTail;
                    //WriteBuffer.Data[uTail] = pp[i];//pSampleBuffer[i];//WavEncodeBuf[i];
                              WriteBuffer.Data[uTail] = WavEncodeBuf[i];
                    WriteBuffer.uTail = (uTail + 1) % WAV_ENCODER_WRITE_BUFFER_LENGTH;
                    WriteBuffer.uCounter++;
                }
            }


            RecordStaFlag &= ~RECORD_STA_PCMBUF_FULL;
            RecordStaFlag |= RECORD_STA_PCMBUF_EMPTY;

            RecordEncodeing = 0;
            SendMsg(MSG_RECORDWIN_DISPFLAG_TIME); //Send the flag that mark up time update.

#else   //B core encode
            // TODO:
            if(EncodeType == RECORD_ENCODE_TYPE_WAV)
            {
                //todo:
                //...
            }
            else if (EncodeType == RECORD_ENCODE_TYPE_PCM)
            {
                uEncodeDataLength = RecordInputBufferLength * 2;

                if(gRecordBlock.encoding == 0)
                {
                    gRecordBlock.encoding = 1;
                    gEncodeDone = 0;
                    MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_ENCODE_PREPARE, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    MailBoxWriteA2BData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);

                    while(!gEncodeDone)
                    {
#ifdef _LOG_DEBUG_
                        BBDebug();
#endif
                        __WFI();
                    }
                    gEncodeDone = 0;

                    //dma transmit data to B core.
                    {
                         eDMA_CHN channel;
                         DMA_CFGX DmaCfg = {DMA_CTLL_M2M_WORD, DMA_CFGL_M2M_WORD, DMA_CFGH_M2M_WORD, 0};

                         DmaTranferCallback = 0;

                         channel = DmaGetChannel();
                         if (channel != DMA_FALSE)
                         {
                             DmaStart((uint32)(channel),
                                        (UINT32)(pSampleBuffer),
                                        (uint32)(gRecordBlock.bufptr),
                                        RecordInputBufferLength / 2,
                                        &DmaCfg,
                                        DMATranferCallBack);

                             while(1)
                             {
                                 __WFI();
                                 if(DmaTranferCallback == 1)
                                     break;
                             }

                         }
                    }

                    MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_ENCODE, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    MailBoxWriteA2BData(uEncodeDataLength, MAILBOX_ID_0, MAILBOX_CHANNEL_1);

                    return;
                }
                else if(gRecordBlock.encodeOver == 1)
                {
                    //dma transmit data to B core.
                    {
                         eDMA_CHN channel;
                         DMA_CFGX DmaCfg = {DMA_CTLL_M2M_WORD, DMA_CFGL_M2M_WORD, DMA_CFGH_M2M_WORD, 0};

                         DmaTranferCallback = 0;

                         channel = DmaGetChannel();
                         if (channel != DMA_FALSE)
                         {
                             DmaStart((uint32)(channel),
                                        (UINT32)(pSampleBuffer),
                                        (uint32)(gRecordBlock.bufptr),
                                        RecordInputBufferLength / 2,
                                        &DmaCfg,
                                        DMATranferCallBack);

                             while(1)
                             {
                                 __WFI();
                                 if(DmaTranferCallback == 1)
                                     break;
                             }

                         }
                    }

                    MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_ENCODE, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    MailBoxWriteA2BData(uEncodeDataLength, MAILBOX_ID_0, MAILBOX_CHANNEL_1);

                    return;
                }
                else if(gRecordBlock.encoding == 1)
                {
                    return;
                }


            }
#endif



        }
    }

}
#endif
/*
--------------------------------------------------------------------------------
  Function name : void WavEncodeHeaderInit()
  Author        : WangBo
  Description   : write the head of wav file.

  Input         :
  Return        :

  History:     <author>         <time>         <version>
               WangBo           2009-4-16          1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
_ATTR_RECORD_CONTROL_CODE_
void WavEncodeHeaderInit()
{
    PCMWAVEFORMAT sWaveFormat;
    int ulIdx,i;
    extern int AdaptCoeff11[];
    extern int AdaptCoeff22[];

    WavEncodeHeadBuffer = &(WriteBuffer.Data[0]);

    for (i=4095;i>=0;i--)                   //init  WavEncodeHeadBuffer[512]
    {
        WavEncodeHeadBuffer[i]=0x00;
    }

    WavEncodeHeadBuffer[0] = 'R';
    WavEncodeHeadBuffer[1] = 'I';
    WavEncodeHeadBuffer[2] = 'F';
    WavEncodeHeadBuffer[3] = 'F';

    WavEncodeHeadBuffer[8]  = 'W';
    WavEncodeHeadBuffer[9]  = 'A';
    WavEncodeHeadBuffer[10] = 'V';
    WavEncodeHeadBuffer[11] = 'E';

    WavEncodeHeadBuffer[12] = 'f';
    WavEncodeHeadBuffer[13] = 'm';
    WavEncodeHeadBuffer[14] = 't';
    WavEncodeHeadBuffer[15] = ' ';
    WavEncodeHeadBuffer[16] = 50;
    WavEncodeHeadBuffer[17] = 0;
    WavEncodeHeadBuffer[18] = 0;
    WavEncodeHeadBuffer[19] = 0;

    if(RECORD_PCM == RECORD_TYPE_CONFIG)
    {
        sWaveFormat.wFormatTag = 1;
        sWaveFormat.nChannels = PCM_s.ucChannels;
        sWaveFormat.nSamplesPerSec = PCM_s.usSampleRate;
        sWaveFormat.nAvgBytesPerSec = PCM_s.usByteRate;
        sWaveFormat.nBlockAlign = PCM_s.usBytesPerBlock;
        sWaveFormat.wBitsPerSample = 16;
        sWaveFormat.cbSize = 32;
        sWaveFormat.wSamplesPerBlock = PCM_s.usSamplesPerBlock;
        sWaveFormat.wNumCoef = 0;
        for (ulIdx = 0; ulIdx < 7; ulIdx++)
        {
            sWaveFormat.aCoef[ulIdx].iCoef1 = 0;
            sWaveFormat.aCoef[ulIdx].iCoef2 = 0;
        }
    }
    else
    {
        sWaveFormat.wFormatTag = 2;
        sWaveFormat.nChannels = PCM_s.ucChannels;
        sWaveFormat.nSamplesPerSec = PCM_s.usSampleRate;
        sWaveFormat.nAvgBytesPerSec = PCM_s.usByteRate;
        sWaveFormat.nBlockAlign = PCM_s.usBytesPerBlock;
        sWaveFormat.wBitsPerSample = 4;
        sWaveFormat.cbSize = 32;
        sWaveFormat.wSamplesPerBlock = PCM_s.usSamplesPerBlock;
        sWaveFormat.wNumCoef = 7;

        for (ulIdx = 0; ulIdx < 7; ulIdx++)
        {
            sWaveFormat.aCoef[ulIdx].iCoef1 = AdaptCoeff11[ulIdx];
            sWaveFormat.aCoef[ulIdx].iCoef2 = AdaptCoeff22[ulIdx];
        }
    }

    memcpy(WavEncodeHeadBuffer + 20, (void *)&sWaveFormat, 50);

    WavEncodeHeadBuffer[70] = 'f';
    WavEncodeHeadBuffer[71] = 'a';
    WavEncodeHeadBuffer[72] = 'c';
    WavEncodeHeadBuffer[73] = 't';
    WavEncodeHeadBuffer[74] = 4;
    WavEncodeHeadBuffer[75] = 0;
    WavEncodeHeadBuffer[76] = 0;
    WavEncodeHeadBuffer[77] = 0;

    WavEncodeHeadBuffer[78] = PCM_s.ulTimePos;
    WavEncodeHeadBuffer[79] = PCM_s.ulTimePos >> 8;
    WavEncodeHeadBuffer[80] = PCM_s.ulTimePos >> 16;
    WavEncodeHeadBuffer[81] = PCM_s.ulTimePos >> 24;

    WavEncodeHeadBuffer[82] = 0x4c;//"list"
    WavEncodeHeadBuffer[83] = 0x49;
    WavEncodeHeadBuffer[84] = 0x53;
    WavEncodeHeadBuffer[85] = 0x54;

    WavEncodeHeadBuffer[86] = 0x9e;
    WavEncodeHeadBuffer[87] = 0x0f;      //by Vincent

    WavEncodeHeadBuffer[90] = 0x49;//INFO
    WavEncodeHeadBuffer[91] = 0x4e;
    WavEncodeHeadBuffer[92] = 0x46;
    WavEncodeHeadBuffer[93] = 0x4f;

    WavEncodeHeadBuffer[94] = 'R';
    WavEncodeHeadBuffer[95] = 'o';
    WavEncodeHeadBuffer[96] = 'c';
    WavEncodeHeadBuffer[97] = 'k';

    WavEncodeHeadBuffer[98]  = 0x92; //by Vincent
    WavEncodeHeadBuffer[99]  = 0x0f; //by Vincent

    WavEncodeHeadBuffer[4088] = 'd';
    WavEncodeHeadBuffer[4089] = 'a';
    WavEncodeHeadBuffer[4090] = 't';
    WavEncodeHeadBuffer[4091] = 'a';

    WavEncodeHeadBuffer[4092] = 0Xff;
    WavEncodeHeadBuffer[4093] = 0Xff;
    WavEncodeHeadBuffer[4094] = 0Xff;
    WavEncodeHeadBuffer[4095] = 0Xff;

    WavEncodeHeadBuffer[4] = 0Xff;
    WavEncodeHeadBuffer[5] = 0Xff;
    WavEncodeHeadBuffer[6] = 0Xff;
    WavEncodeHeadBuffer[7] = 0Xff;

}

#endif
//#endif
