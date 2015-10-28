/* Copyright (C) 2009 ROCK-CHIPS FUZHOU . All Rights Reserved. */
/*
File    : \Audio\pMP3.c
Desc    :

Author  :
Date    :
Notes   :

$Log    :
*
*
*/
/****************************************************************/
#include "RKOS.h"
#include "../include/audio_main.h"
#include "../include/audio_globals.h"
#include "../include/audio_file_access.h"

#ifdef WMA_DEC_INCLUDE
#include "typedef.h"
#include "mailbox.h"
#include "pWMA.h"
//#include <stdio.h>
//#include <string.h> //for memcpy(),memmove()
#include ".\wmaInclude\predefine.h"
#if 1//ndef WMAAPI_NO_DRM//USB 会调用，故放在系统段
const unsigned char g_pmid[] =
{
    0x32, 0x33, 0x44, 0x45, 0x38, 0x45, 0x32, 0x41, 0x39, 0x34,
    0x45, 0x44, 0x43, 0x43, 0x30, 0x46
};
#endif
#pragma arm section zidata = "WmaDecoderBss",rwdata = "WmaDecoderData"

//------------------global varibles----------------------------------
extern unsigned int coed_dma_channel;

#ifdef A_CORE_DECODE
static WMA_DEC_INFO gWMADecInfo;

static unsigned int bytesCnt = MAX_SAMPLES_OF_ONE_CHANNEL*2;
//unsigned int g_DecodedSamples;
//unsigned int g_cCountGetPCM;
static unsigned long g_TotalSamples;
static unsigned int g_wmaBufIndex;

static long wma_frame_cnt = 0;

//extern tWMAFileContDesc *g_pdesc;
//extern tWMAFileHeader g_hdr;
short *gWmaOutputPtr[2];
extern unsigned long SRC_Num_Forehead; //for src

#else
//#include "sysinclude.h"

#include "driverinclude.h"
extern MediaBlock  gpMediaBlock;
extern uint32  gError;
extern unsigned char *DecDataBuf[2];
extern unsigned char DecBufID;
static UINT16 DmaTranferCallback;

static void DMATranferCallBack(void)
{
    //printf("DMACallBack give sema osAudioDecodeOk\n");
    DmaDev_RealseChannel(hAudioDma, coed_dma_channel);
    rkos_semaphore_give_fromisr(osAudioDecodeOk);
}

#endif
//static FILE *g_fpWMA;
//-----------------------------------------------------------------
extern tWMAFileStatus wma_decoder_dec(WMA_DEC_INFO* dec_info);
extern tWMAFileStatus wma_decoder_init(WMA_DEC_INFO* dec_info);
extern tWMAFileStatus wma_decoder_seek(WMA_DEC_INFO* dec_info);

//*************************************************************************************************************//
//the achievement of functions.：
//SUBFN_CODEC_GETNAME  :   get decoder name
//SUBFN_CODEC_GETARTIST:   get artist name.
//SUBFN_CODEC_GETTITLE :   get song title.
//SUBFN_CODEC_GETBITRATE:  get bit rate.
//SUBFN_CODEC_GETSAMPLERATE: get sample rate.
//SUBFN_CODEC_GETCHANNELS: get channel number.
//SUBFN_CODEC_GETLENGTH :  get total play time [unit:ms]
//SUBFN_CODEC_GETTIME  :   get current play time.[unit:ms].note:this time get by timestamp,there may be error if file is been demage..
//SUBFN_CODEC_OPEN_DEC :   open deooder(initialization.)
//SUBFN_CODEC_DECODE   :   deocode.
//SUBFN_CODEC_ENCODE   :   not support.
//SUBFN_CODEC_SEEK     :   location by time directly.[unit:ms]
//SUBFN_CODEC_CLOSE    :   close decoder.
//SUBFN_CODEC_SETBUFFER:   set cache area,point out the position to put save result.
/******************************************************
Name:
Desc:
Param: ulIoctl child function number.
    ulParam1 child function parameter 1.
    ulParam2 child function parameter 2.
    ulParam3 child function parameter 3.
    ulParam4 child function parameter 4.

Return:
Global:
Note:
Author:
Log:
******************************************************/
#pragma arm section code = "WmaCommonCode"
unsigned long WMAFunction(unsigned long ulIoctl, unsigned long ulParam1,
                          unsigned long ulParam2, unsigned long ulParam3)
{
    unsigned int *pTrack,i;


#ifdef A_CORE_DECODE
    switch (ulIoctl)
    {
#if 1
        case SUBFN_CODEC_GETNAME:
            {
                char **ppcName;

                ppcName = (char **)ulParam1;
                *ppcName = "WMA Windows Media(tm) Audio";
                return 1;

            }
        case SUBFN_CODEC_GETARTIST:
            {

                //char **ppcName = (char **)ulParam1;
                if (gWMADecInfo.pdesc)
                {
                    *((unsigned int*)ulParam1) = (unsigned int)(gWMADecInfo.pdesc->pAuthor);
                    return 1;
                }
                return 0;

            }

        case SUBFN_CODEC_GETTITLE:
            {
                //char **ppcName;

                //ppcName = (char **)ulParam1;
                if (gWMADecInfo.pdesc)
                {
                    *((unsigned int*)ulParam1) = (unsigned int)(gWMADecInfo.pdesc->pTitle);
                    return 1;
                }
                return 0;

            }

        case SUBFN_CODEC_GETBITRATE:
            {
                //unsigned long *pulBitRate;

                //pulBitRate = ;
                *((unsigned long *)ulParam1) = (unsigned long)(gWMADecInfo.hdr->bitrate);
                return 1;

            }
        case SUBFN_CODEC_GETBPS:
            {
                *((unsigned long *)ulParam1) = (unsigned long)(gWMADecInfo.hdr->valid_bits_per_sample);
                return 1;
            }

        case SUBFN_CODEC_GETSAMPLERATE:
            {
                unsigned long *pulSampleRate;

                pulSampleRate = (unsigned long *)ulParam1;
                *pulSampleRate = (unsigned long)(gWMADecInfo.hdr->sample_rate);
                return 1;

            }

        case SUBFN_CODEC_GETCHANNELS:
            {
                unsigned long *pulChannels;

                pulChannels = (unsigned long *)ulParam1;
                *pulChannels = (unsigned long)(gWMADecInfo.hdr->num_channels);
                return 1;

            }

        case SUBFN_CODEC_GETLENGTH:
            {
                unsigned long *pulLength;

                pulLength = (unsigned long *)ulParam1;
                *pulLength = (unsigned long)(gWMADecInfo.hdr->duration);
                return 1;

            }

        case SUBFN_CODEC_GETTIME:
            {
                unsigned long *pulTime;
                unsigned int decode_samples = gWMADecInfo.decoded_samples;
                unsigned long sample_rate = gWMADecInfo.hdr->sample_rate;
                pulTime = (unsigned long *)ulParam1;

                *pulTime = (decode_samples/ sample_rate * 1000) + ((decode_samples % sample_rate) * 1000  / sample_rate);

                return 1;

            }

        case SUBFN_CODEC_OPEN_ENC:
            {
                return 1;
            }
#endif
        case SUBFN_CODEC_OPEN_DEC:
            {
#ifdef WMAINITIALIZE
                int rc;

                wma_frame_cnt = 0;

                gWMADecInfo.fhandle = (unsigned long)pRawFileCache;

                rc = wma_decoder_init(&gWMADecInfo);
                if (rc != cWMA_NoErr)
                {
                    //DEBUG("WMA Decoder Init Failed!\n");
                    //goto ErrorExit;
                    return 0;
                }
                if ((gWMADecInfo.hdr->bitrate == 22000) && (gWMADecInfo.hdr->sample_rate == 32000) && (gWMADecInfo.hdr->num_channels == 2))//此格式不支持,原因看文档
                {
                    //DEBUG("Don't support format!\n");
                    //goto ErrorExit;
                    return 0;
                }
                if (gWMADecInfo.isHighRate == 1)
                    //if (isWMAHighRate())
                {
                    //DEBUG("wma is highrate!\n");
#ifndef WMAHIGHRATE
                    //DEBUG("wma highrate declib is close!\n");
                    //goto ErrorExit;
                    return 0;
#endif
                }
                else
                {
                    //DEBUG("wma is midrate or lowrate!\n");
#ifndef WMAMIDRATELOWRATE
                    //DEBUG("wma midrate/lowrate declib is close!\n");
                    //goto ErrorExit;
                    return 0;
#endif
                }

                g_TotalSamples = (gWMADecInfo.hdr->duration / 1000) * gWMADecInfo.hdr->sample_rate + (gWMADecInfo.hdr->duration % 1000) * gWMADecInfo.hdr->sample_rate / 1000;

                g_wmaBufIndex = 0;
                gWMADecInfo.curPtr = (unsigned long)&gWmaOutputPtr[g_wmaBufIndex][SRC_Num_Forehead];
                memset((void*)gWMADecInfo.curPtr,0,MAX_SAMPLES_OF_ONE_CHANNEL*sizeof(short));
#endif
                return 1;
            }
        case SUBFN_CODEC_GETBUFFER:
            {
                // return the buffer address
                //*(unsigned int *)ulParam1 = (unsigned int)(gWMADecInfo.curPtr);
                *(unsigned int *)ulParam1 = (unsigned int)(&gWmaOutputPtr[g_wmaBufIndex][SRC_Num_Forehead]);
                g_wmaBufIndex ^= 1;
                *(unsigned int *)ulParam2 = (bytesCnt>>2);
                //(short*)ulParam1 = (uint32)(gWmaOutputPtr[g_wmaBufIndex]);

                return 1;
            }

        case SUBFN_CODEC_DECODE:
            {
#if defined(WMAHIGHRATE) || defined(WMAMIDRATELOWRATE)
                tWMAFileStatus rc;
                /* decode loop */
                // while(1)

                gWMADecInfo.curPtr = (unsigned long)&gWmaOutputPtr[g_wmaBufIndex][SRC_Num_Forehead];

                {
                    rc = wma_decoder_dec(&gWMADecInfo);
                    if (rc != cWMA_NoErr)
                    {
                        memset((void*)gWMADecInfo.curPtr,0,MAX_SAMPLES_OF_ONE_CHANNEL*sizeof(short));
                        return 0;
                    }
                    else
                    {
                        bytesCnt = gWMADecInfo.frameLen * 2 * gWMADecInfo.hdr->valid_bits_per_sample / 8;
                        if (wma_frame_cnt < 6) //(gWmaIsRightAfterSeek)
                        {

                        }
                        else
                        {
                            //g_wmaBufIndex ^= 1;
                        }

                        wma_frame_cnt ++;

                        return 1;
                    }
                }
#endif
            }

        case SUBFN_CODEC_SEEK:
            {
#if defined(WMAHIGHRATE) || defined(WMAMIDRATELOWRATE)
                tWMAFileStatus rc;

                wma_frame_cnt = 0;

                //reset dma,synth,imdct
                /*
                *((volatile unsigned long *) 0x40010014) = 0x00000070;
                *((volatile unsigned long *) 0x40010014) = 0x00000000;
                */
                gWMADecInfo.curPtr = (unsigned long)&gWmaOutputPtr[g_wmaBufIndex^1][SRC_Num_Forehead];

                gWMADecInfo.msSeekTo = ulParam1;
                rc = wma_decoder_seek(&gWMADecInfo);
#endif

                memset(&gWmaOutputPtr[0][SRC_Num_Forehead],0,(MAX_SAMPLES_OF_ONE_CHANNEL)*sizeof(short));
                memset(&gWmaOutputPtr[1][SRC_Num_Forehead],0,(MAX_SAMPLES_OF_ONE_CHANNEL)*sizeof(short));
                if (rc != cWMA_NoErr)
                    return 0;
                return 1;
            }

        case SUBFN_CODEC_CLOSE:
            {
                return 1;
            }

#if 0
        case SUBFN_CODEC_ENCODE:
            {
                return 1;
            }
#endif

        default:
            {
                return 0;
            }
    }
#else //b-core decode
    switch (ulIoctl)
    {
#if 1
        case SUBFN_CODEC_GETNAME:
            {
                return 1;

            }
        case SUBFN_CODEC_GETARTIST:
            {
                return 1;
            }

        case SUBFN_CODEC_GETTITLE:
            {
                return 1;
            }

        case SUBFN_CODEC_GETBITRATE:
            {
                *(int *)ulParam1 = gpMediaBlock.BitRate;
                return 1;
            }

        case SUBFN_CODEC_GETSAMPLERATE:
            {
                *(int *)ulParam1 = gpMediaBlock.SampleRate;
                return 1;
            }

        case SUBFN_CODEC_GETCHANNELS:
            {
                *(int *)ulParam1 = gpMediaBlock.Channel;
                return 1;
            }

        case SUBFN_CODEC_GETLENGTH:
            {
                *(unsigned long *)ulParam1 = gpMediaBlock.TotalPlayTime;
                return 1;
            }

        case SUBFN_CODEC_GETTIME:
            {
                *(unsigned long *)ulParam1 = gpMediaBlock.CurrentPlayTime;
                return 1;
            }

        case SUBFN_CODEC_GETBPS:
            {
                *(int *)ulParam1 = gpMediaBlock.Bps;
                return(1);
            }


#endif
        case SUBFN_CODEC_OPEN_DEC:
            {
                MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_DEC_OPEN, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                MailBoxWriteA2BData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                gpMediaBlock.needDecode = 0;
                rkos_semaphore_take(osAudioDecodeOk, MAX_DELAY);
                if ( gError )   //codec decode open error
                    return 0;
                else
                    return(1);
            }
        case SUBFN_CODEC_GETBUFFER:
            {
retry:
                IntDisable(INT_ID_MAILBOX1);
                if(gpMediaBlock.DecodeOver == 1)
                {
                    if (gpMediaBlock.DecodeErr == 1)
                    {
                        IntEnable(INT_ID_MAILBOX1);
                        return 0;
                    }

                    // *(int *)ulParam1 = gpMediaBlock.Outptr;
                    *(int *)ulParam2 = gpMediaBlock.OutLength;
                    gpMediaBlock.DecodeOver = 0;


                    gpMediaBlock.Decoding = 1;
                    MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_DECODE, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    MailBoxWriteA2BData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    //rk_printf("gpMediaBlock.OutLength = %d", gpMediaBlock.OutLength);
                    //memory copy hram 2 lram.
                    if (DecBufID == 0)
                    {
                        #if 1
                        {

                             rk_err_t ret;
                             DMA_CFGX DmaCfg = {DMA_CTLL_M2M_WORD, DMA_CFGL_M2M_WORD, DMA_CFGH_M2M_WORD, 0};

                             ret = DmaDev_GetChannel(hAudioDma);
                             coed_dma_channel = ret;
                             if (ret < 0)
                             {
                                  memcpy(DecDataBuf[0],(uint8*)(gpMediaBlock.Outptr) ,(gpMediaBlock.OutLength * gpMediaBlock.Bps) / 4);
                             }
                             else
                             {
                                 DmaDev_DmaStart(hAudioDma, (uint32)(ret), (UINT32)(gpMediaBlock.Outptr),(uint32)(DecDataBuf[0]),(gpMediaBlock.OutLength * gpMediaBlock.Bps) / 16,&DmaCfg, DMATranferCallBack);

                                 rkos_semaphore_take(osAudioDecodeOk, MAX_DELAY);

                             }
                        }
                        #else

                        memcpy(&DecDataBuf[0],(uint8*)(gpMediaBlock.Outptr) ,(gpMediaBlock.OutLength * gpMediaBlock.Bps) / 4);

                        #endif
                        *(int *)ulParam1 = (uint32)DecDataBuf[0];
                        DecBufID = 1;
                    }
                    else
                    {

                        #if 1
                        {

                             rk_err_t ret;
                             DMA_CFGX DmaCfg = {DMA_CTLL_M2M_WORD, DMA_CFGL_M2M_WORD, DMA_CFGH_M2M_WORD, 0};

                             ret = DmaDev_GetChannel(hAudioDma);
                             coed_dma_channel = ret;
                             if (ret < 0)
                             {
                                  //printf("NO  DmaDev_GetChannel\n ");
                                  memcpy(DecDataBuf[1],(uint8*)(gpMediaBlock.Outptr) ,(gpMediaBlock.OutLength * gpMediaBlock.Bps) / 4);
                             }
                             else
                             {
                                 //printf("@#DmaDev_GetChannel ret=%d OutLength =%d Bps=%d\n",ret,gpMediaBlock.OutLength,gpMediaBlock.Bps);
#if 0
                                 pTrack = (unsigned int *)gpMediaBlock.Outptr;
                                 for (i = 0; i < 50; i++)//pstWriteAudioDev->TrackLen
                                 {
                                     printf("## Track NO.%d = 0x%x \n", i, *pTrack);
                                     pTrack++;
                                 }

#endif
                                 DmaDev_DmaStart(hAudioDma, (uint32)(ret), (UINT32)(gpMediaBlock.Outptr),(uint32)(DecDataBuf[1]),(gpMediaBlock.OutLength * gpMediaBlock.Bps) / 16,&DmaCfg, DMATranferCallBack);

                                 rkos_semaphore_take(osAudioDecodeOk, MAX_DELAY);

                             }
                        }
                        #else

                        memcpy(&DecDataBuf[1],(uint8*)(gpMediaBlock.Outptr) ,(gpMediaBlock.OutLength * gpMediaBlock.Bps) / 4);

                        #endif

                        *(int *)ulParam1 = (uint32)DecDataBuf[1];
                        DecBufID = 0;
                    }

                    IntEnable(INT_ID_MAILBOX1);

                    return(1);
                }
                else if(gpMediaBlock.Decoding == 0)
                {
                    gpMediaBlock.Decoding = 1;
                    gpMediaBlock.needDecode = 1;
                    MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_DECODE, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    MailBoxWriteA2BData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    //DEBUG("hifi ape get buffer...DecodeOver = %d",gpMediaBlock.DecodeOver);
                    IntEnable(INT_ID_MAILBOX1);
                    rkos_semaphore_take(osAudioDecodeOk, MAX_DELAY);
                    goto retry;
                }
                else if(gpMediaBlock.Decoding == 1)
                {
                    //DEBUG("hifi ape get buffer...DecodeOver = %d",gpMediaBlock.DecodeOver);
                    gpMediaBlock.needDecode = 1;
                    IntEnable(INT_ID_MAILBOX1);
                    rkos_semaphore_take(osAudioDecodeOk, MAX_DELAY);
                    goto retry;
                }

                return(0);
            }
                            //memory copy hram 2 lram.
                            /*
                                                if (DecBufID == 0)
                            {
#if 1
                                {
                                    eDMA_CHN channel;
                                    DMA_CFGX DmaCfg = {DMA_CTLL_M2M_WORD, DMA_CFGL_M2M_WORD, DMA_CFGH_M2M_WORD, 0};

                                    DmaTranferCallback = 0;

                                    channel = DmaGetChannel();
                                    if (channel != DMA_FALSE)
                                    {
                                        DmaStart((uint32)(channel), (UINT32)(gpMediaBlock.Outptr),(uint32)(&DecDataBuf[0][0]),(gpMediaBlock.OutLength * gpMediaBlock.Bps) / 16,&DmaCfg, DMATranferCallBack);

                                        while (1)
                                        {
                                            __WFI();
                                            if (DmaTranferCallback == 1)
                                                break;
                                        }
                                    }
                                }
#else

                                memcpy(&DecDataBuf[0][0],(uint8*)(gpMediaBlock.Outptr) ,(gpMediaBlock.OutLength * gpMediaBlock.Bps) / 4);

#endif
                                *(int *)ulParam1 = (uint32)&DecDataBuf[0][0];
                                DecBufID = 1;
                            }
                            else
                            {
#if 1
                                {
                                    eDMA_CHN channel;
                                    DMA_CFGX DmaCfg = {DMA_CTLL_M2M_WORD, DMA_CFGL_M2M_WORD, DMA_CFGH_M2M_WORD, 0};

                                    DmaTranferCallback = 0;

                                    channel = DmaGetChannel();
                                    if (channel != DMA_FALSE)
                                    {
                                        DmaStart((uint32)(channel), (UINT32)(gpMediaBlock.Outptr),(uint32)(&DecDataBuf[1][0]),(gpMediaBlock.OutLength * gpMediaBlock.Bps) / 16,&DmaCfg, DMATranferCallBack);

                                        while (1)
                                        {
                                            __WFI();
                                            if (DmaTranferCallback == 1)
                                                break;
                                        }
                                    }
                                }
#else
                                memcpy(&DecDataBuf[1][0],(uint8*)(gpMediaBlock.Outptr) ,(gpMediaBlock.OutLength * gpMediaBlock.Bps) / 4);
#endif

                                *(int *)ulParam1 = (uint32)&DecDataBuf[1][0];
                                DecBufID = 0;
                            }

                            return(1);
                        }
                        else if (gpMediaBlock.Decoding == 0)
                        {
                            gpMediaBlock.Decoding = 1;
                            MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_DECODE, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                            MailBoxWriteA2BData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                            return 2;
                        }
                        else if (gpMediaBlock.Decoding == 1)
                        {
                            return 2;
                        }

                        return(0);
                                        */

        case SUBFN_CODEC_DECODE:
            {
                MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_DECODE, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                MailBoxWriteA2BData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                return 1;
            }

        case SUBFN_CODEC_SEEK:
            {
                MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_DECODE_SEEK, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                MailBoxWriteA2BData(ulParam1, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                rkos_semaphore_take(osAudioDecodeOk, MAX_DELAY);
                return 1;
            }

        case SUBFN_CODEC_CLOSE:
            {
                MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_DECODE_CLOSE, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                MailBoxWriteA2BData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                rkos_semaphore_take(osAudioDecodeOk, MAX_DELAY);
                return 1;
            }

        case SUBFN_CODEC_GET_FRAME_LEN:
            {
                *(int *)ulParam1 = gpMediaBlock.OutLength;
            }

        default:
            {
                return 0;
            }
    }
#endif
}
#pragma arm section code
#endif
