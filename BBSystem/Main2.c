/*
*********************************************************************************************************
*                                       NANO_OS The Real-Time Kernel
*                                         FUNCTIONS File for V0.X
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*File    : APP.C
* By      : Zhu Zhe
*Version : V0.x
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            DESCRIPTION
*  RK_NANO_OS is a system designed specifically for real-time embedded SOC operating system ,before using
*RK_NANO_OS sure you read the user's manual
*  The TASK NAME TABLE:
*
*
*  The DEVICE NAME TABLE:
*  "UartDevice",              Uart Serial communication devices
*  "ADCDevice",               The analog signal is converted to a digital signal device
*  "KeyDevice",               Key driver device
*
*
*
*
*
*
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "BspConfig.h"
#include "RKOS.h"
#include "stdio.h"
#include "mailbox.h"
#include "audio_globals.h"
#include "audio_main.h"
#include "audio_file_access.h"
#include "AppInclude.h"
#include "Msg.h"

typedef enum _MEDIA_MSGBOX_ENCODE_CMD
{
    MEDIA_MSGBOX_CMD_ENCODE_NULL = 200,

    MEDIA_MSGBOX_CMD_ENC_OPEN,
    MEDIA_MSGBOX_CMD_ENC_OPEN_CMPL,

    //record cmd
    MEDIA_MSGBOX_CMD_ENCODE,
    MEDIA_MSGBOX_CMD_ENCODE_PREPARE,
    MEDIA_MSGBOX_CMD_ENCODE_INIT,
    MEDIA_MSGBOX_CMD_ENCODE_INIT_CMPL,
//    MEDIA_MSGBOX_CMD_ENCODE_DATA_REQ,
    MEDIA_MSGBOX_CMD_ENCODE_CMPL,      /*完成一次编码操作*/
    MEDIA_MSGBOX_CMD_ENCODE_ERR,       /*编码出错*/

    MEDIA_MSGBOX_CMD_REC_FILE_CLOSE,
    MEDIA_MSGBOX_CMD_REC_FILE_CLOSE_CMPL,
    MEDIA_MSGBOX_CMD_REC_FILE_CLOSE_HANDSHK,

}MEDIA_MSGBOX_ENCODE_CMD;

/*
*********************************************************************************************************
*                                        Macro Define
*********************************************************************************************************
*/
#define mainTimer_TASK_PRIORITY     configMAX_PRIORITIES-1
#define mainTest1_TASK_PRIORITY     2
#define mainTest2_TASK_PRIORITY     1
#define mainTest_QUEUE_SIZE            3

/*
*********************************************************************************************************
*                                        Variable Define
*********************************************************************************************************
*/
_ATTR_BB_SYS_DATA_
uint32 gCmd = 0;
_ATTR_BB_SYS_DATA_
uint32 gData = 0;
_ATTR_BB_SYS_DATA_
uint8  gDecStatus = 0;

_ATTR_BB_SYS_DATA_
uint8  gFileOpStatus = 0;

extern uint8 gCmdDone;
extern uint32  CodecBufSize2;

typedef struct _BBCore_Pcb_
{
    uint32 audio_decode_status;
    uint32 audio_decode_param;
}BbCore_PCB;

enum CORE2_RUN_STATUS
{
    //audio decode
    AUDIO_DECODE_IDLE = 0,
    AUDIO_DECODE_OPEN,
    AUDIO_DECODE_DECODE,
    AUDIO_DECODE_SEEK,
    AUDIO_DECODE_GETBUFFER,
    AUDIO_DECODE_GETTIME,
    AUDIO_DECODE_CLOSE,

    //file operate
    AUDIO_DECODE_FILE_OPEN,
    AUDIO_DECODE_FILE_SEEK,
    AUDIO_DECODE_FILE_READ,
    AUDIO_DECODE_FILE_TELL,
    AUDIO_DECODE_FILE_GETLENGTH,
    AUDIO_DECODE_FILE_CLOSE,

    //record encord cmd
    RECORD_ENCORD_INIT,
    RECORD_ENCORD_ENCODE,
    RECORD_ENCORD_GETDATA,
    RECORD_ENCORD_CLOSE,

};


enum AUDIO_FILE_OPT_STATUS
{
    AUDIO_FILE_OPT_IDLE = 0,
    AUDIO_FILE_OPEN_CMPL,
    AUDIO_FILE_SEEK_CMPL,
    AUDIO_FILE_READ_CMPL,
    AUDIO_FILE_WRITE_CMPL,
    AUDIO_FILE_TELL_CMPL,
    AUDIO_FILE_GET_LEN_CMPL,
    AUDIO_FILE_CLOSE_CMPL,

    RECORD_FILE_CREATE_CMPL,
    RECORD_FILE_CLOSE_CMPL,

};

static BbCore_PCB pcb;
static MediaBlock gMediaBlockInfo;

static FILE_HANDLE_t * gFileHandle;
static RecFileHdl_t  * gRecFileHdl;
static RecordBlock     gRecordBlockInfo;

/*
*********************************************************************************************************
*                                      extern varible
*********************************************************************************************************
*/
extern int CurrentCodec2;
extern uint32 FileTotalSize;
extern uint32 CurFileOffset[8];
extern uint32  AudioFileBufBusy2;
extern uint32  AudioFileBufSize2[2];
extern uint32  AudioFileWrBufID2;
extern FILE_READ_OP_t gFileReadParam;
extern uint8 gBufByPass;


#ifdef WAV_DEC_INCLUDE2
extern INT16    hRecFile;
extern uint32   RecFileOffset;
extern tPCM_enc PCM_source;
extern uint32   WavEncodeInputBuffer2[WAV_AD_PIPO_BUFFER_SIZE];  //Buffer must align 4
extern UINT16   *pRecordEncodeInputBuffer2;
extern UINT16   *pRecordPCMInputBuffer2;
extern UINT16   *pRecordInputBuffer2;

extern uint16 RecordInputBufIndex;
#endif
/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
#ifdef _LOG_DEBUG_
//this fun just for debuging
static uint8* Cmd2String2(uint32 cmd)
{
    uint8* pstr;

    switch (cmd)
    {
        case MEDIA_MSGBOX_CMD_FILE_OPEN_CMPL:
            pstr = "FILE OPEN CMPL";
            break;

        case MEDIA_MSGBOX_CMD_FILE_SEEK_CMPL:
            pstr = "FILE SEEK CMPL";
            break;

        case MEDIA_MSGBOX_CMD_FILE_READ_CMPL:
            pstr = "FILE READ CMPL";
            break;

        case MEDIA_MSGBOX_CMD_FILE_WRITE_CMPL:
            pstr = "FILE WRITE CMPL";
            break;

        case MEDIA_MSGBOX_CMD_FILE_TELL_CMPL:
            pstr = "FILE TELL CMPL";
            break;

        case MEDIA_MSGBOX_CMD_FILE_GET_LENGTH_CMPL:
            pstr = "FILE GET_LEN CMPL";
            break;

        case MEDIA_MSGBOX_CMD_FILE_CLOSE_CMPL:
            pstr = "FILE CLOSE CMPL";
            break;

        default:
            pstr = "NOT FOUND CMD";
            break;
    }

    return pstr;
}

void dumpMemoryByte(uint8* addr,uint32 size)
{
    uint32 index = 0;
    uint8 *ptemp;
    ptemp = addr;

    for (index = 0; index < size;index++)
    {
        bb_printf1("%02x",*ptemp);
        ptemp++;
    }

}

void dumpMemoryShort(uint16* addr,uint32 size)
{
    uint32 index = 0;
    uint8 data[2];
    uint16 *ptemp;
    ptemp = addr;

    for(index = 0; index < size;index++)
    {
        if(index == 0)
            bb_printf1("\r\n");
        if(index % 8 == 0 && index != 0)
            bb_printf1("\n");

        data[1] = *ptemp >> 8 & 0xff;
        data[0] = *ptemp & 0xff;

        bb_printf1("%02x ",data[1] );
        bb_printf1("%02x ",data[0] );
        ptemp++;
    }
}

void dumpMemoryInt(uint32* addr,uint32 size)
{
    uint32 index = 0,i;
    uint8 data[4];
    uint32 *ptemp;
    ptemp = addr;

    for(index = 0; index < size;index++)
    {
        if(index == 0)
            bb_printf1("\r\n");
        if(index % 4 == 0 && index != 0)
            bb_printf1("\n");

        data[3] = *ptemp >> 24 & 0xff;
        data[2] = *ptemp >> 16 & 0xff;
        data[1] = *ptemp >> 8  & 0xff;
        data[0] = *ptemp & 0xff;

        for(i = 4;i > 0;i--)
        {
            bb_printf1("%02x ",data[i-1] );
        }
        ptemp++;
    }
}

#endif

__irq  void MailBoxDecService()
{
    uint32 cmd;
    uint32 data;
    MailBoxClearA2BInt(MAILBOX_ID_0, MAILBOX_INT_1);

    cmd = MailBoxReadA2BCmd(MAILBOX_ID_0, MAILBOX_CHANNEL_1);
    data = MailBoxReadA2BData(MAILBOX_ID_0, MAILBOX_CHANNEL_1);

    switch (cmd)
    {
         case MEDIA_MSGBOX_CMD_FILE_OPEN_CMPL:
            gFileHandle = (FILE_HANDLE_t *)data;
            pRawFileCache = (FILE *)gFileHandle->handle1;
            FileTotalSize = gFileHandle->filesize;
            CurFileOffset[(uint32)pRawFileCache] = gFileHandle->curfileoffset[0];
            CurrentCodec2 = gFileHandle->codecType;

#ifdef AAC_DEC_INCLUDE
            if (CurrentCodec2 == CODEC_AAC_DEC)
            {
                pAacFileHandleOffset = (FILE *)gFileHandle->handle2;
                CurFileOffset[(uint32)pAacFileHandleOffset] = gFileHandle->curfileoffset[1];
                pAacFileHandleSize = (FILE *)gFileHandle->handle3;
                CurFileOffset[(uint32)pAacFileHandleSize] = gFileHandle->curfileoffset[2];
            }
#endif

#ifdef HIFI_AlAC_DECODE
            if (CurrentCodec2 == CODEC_HIFI_ALAC_DEC)
            {
                pAacFileHandleOffset = (FILE *)gFileHandle->handle2;
                CurFileOffset[(uint32)pAacFileHandleOffset] = gFileHandle->curfileoffset[1];
                pAacFileHandleSize = (FILE *)gFileHandle->handle3;
                CurFileOffset[(uint32)pAacFileHandleSize] = gFileHandle->curfileoffset[2];
            }
#endif

#ifdef FLAC_DEC_INCLUDE
            if (CurrentCodec2 == CODEC_FLAC_DEC)
            {
                pFlacFileHandleBake = (FILE *)gFileHandle->handle2;
                CurFileOffset[(uint32)pFlacFileHandleBake] = gFileHandle->curfileoffset[1];
            }
#endif

            gFileOpStatus = AUDIO_FILE_OPEN_CMPL;

            break;

        case MEDIA_MSGBOX_CMD_FILE_CLOSE_CMPL:
            if (gData)
            {
                bb_printf1("A 2 B data is 0x%08x",gData);
                gFileOpStatus = AUDIO_FILE_CLOSE_CMPL;
            }
            bb_printf1("recive FILE_CLOSE_CMPL");
            gCmdDone = 1;
            break;

        case MEDIA_MSGBOX_CMD_DEC_OPEN:
            pcb.audio_decode_status = AUDIO_DECODE_OPEN;
            bb_printf1("STA = [DEC OPEN]");
            break;

        case MEDIA_MSGBOX_CMD_DECODE:
            pcb.audio_decode_status = AUDIO_DECODE_DECODE;
            //bb_printf1("STA = [DECODE]");
            break;

        case MEDIA_MSGBOX_CMD_DECODE_GETBUFFER:
            pcb.audio_decode_status = AUDIO_DECODE_GETBUFFER;
            bb_printf1("STA = [DEC GET_BUFFER]");
            break;

        case MEDIA_MSGBOX_CMD_DECODE_GETTIME:
            pcb.audio_decode_status = AUDIO_DECODE_GETTIME;
            //bb_printf1("STA = [DEC GET_TIME]");
            break;

        case MEDIA_MSGBOX_CMD_DECODE_SEEK:
            pcb.audio_decode_status = AUDIO_DECODE_SEEK;
            pcb.audio_decode_param  = data;
            bb_printf1("STA = [DECODE SEEK] data = %d",data);
            break;

        case MEDIA_MSGBOX_CMD_DECODE_CLOSE:
            pcb.audio_decode_status = AUDIO_DECODE_CLOSE;
            pcb.audio_decode_param  = data;
            bb_printf1("STA = [DECODE CLOSE]");
            break;

//------------------------------------------------------------------------------
        case MEDIA_MSGBOX_CMD_ENCODE_INIT:
            pcb.audio_decode_status = RECORD_ENCORD_INIT;
            pcb.audio_decode_param  = data;
            //bb_printf1("STA = [ENCODE INIT]");
            break;

//        case MEDIA_MSGBOX_CMD_ENCODE_PREPARE:
//            pcb.audio_decode_status = RECORD_ENCORD_GETDATA;
//            pcb.audio_decode_param  = data;
//            //bb_printf1("STA = [ENCODE PREPARE]");
//            break;

        case MEDIA_MSGBOX_CMD_ENCODE:
            pcb.audio_decode_status = RECORD_ENCORD_ENCODE;
            pcb.audio_decode_param  = data;
            //bb_printf1("STA = [ENCODE PROCESS]");
            break;

//------------------------------------------------------------------------------

        default:
            bb_printf1("###[UNKOWN], cmd is %d",cmd);
            break;
    }
}

void RegHifiDecodeServer()
{
    IntRegister2(INT_ID_MAILBOX1 , (void*)MailBoxDecService);
    IntPendingClear2(INT_ID_MAILBOX1);
    IntEnable2(INT_ID_MAILBOX1);
    MailBoxEnableA2BInt(MAILBOX_ID_0, MAILBOX_INT_1);
}


__irq void MailBoxFileService()
{
    MailBoxClearA2BInt(MAILBOX_ID_0, MAILBOX_INT_2);

    gCmd = MailBoxReadA2BCmd(MAILBOX_ID_0, MAILBOX_CHANNEL_2);
    gData = MailBoxReadA2BData(MAILBOX_ID_0, MAILBOX_CHANNEL_2);

    //bb_printf1("A 2 B cmd is [%s]",Cmd2String2(gCmd));
    //bb_printf1("A 2 B data is 0x%08x",gData);

    switch (gCmd)
    {
        case MEDIA_MSGBOX_CMD_FILE_CREATE_CMPL:
#ifdef WAV_DEC_INCLUDE2
            {
                gRecFileHdl   = (RecFileHdl_t *)gData;
                hRecFile      = gRecFileHdl->fileHandle;
                RecFileOffset = gRecFileHdl->fileOffset;

                gFileOpStatus = RECORD_FILE_CREATE_CMPL;
            }
#endif
            break;

        case MEDIA_MSGBOX_CMD_FILE_SEEK_CMPL:
            gFileOpStatus = AUDIO_FILE_SEEK_CMPL;
            gCmdDone = 1;
            break;

        case MEDIA_MSGBOX_CMD_FILE_READ_CMPL:
            gFileOpStatus = AUDIO_FILE_READ_CMPL;
            if(gBufByPass == 0)
            {
                CurFileOffset[gFileReadParam.handle] = gFileReadParam.NumBytes;

                AudioFileBufSize2[AudioFileWrBufID2] = gData;

                AudioFileBufBusy2 = 0;
            }
            gCmdDone = 1;
            break;

        case MEDIA_MSGBOX_CMD_FILE_WRITE_CMPL:
            gFileOpStatus = AUDIO_FILE_WRITE_CMPL;
            gCmdDone = 1;
            break;

        case MEDIA_MSGBOX_CMD_FILE_TELL_CMPL:
            gFileOpStatus = AUDIO_FILE_TELL_CMPL;
            gCmdDone = 1;
            break;

        case MEDIA_MSGBOX_CMD_FILE_GET_LENGTH_CMPL:
            gFileOpStatus = AUDIO_FILE_GET_LEN_CMPL;
            gCmdDone = 1;
            break;


            //bb_printf1("A 2 B cmd is [%s]",Cmd2String2(gCmd));
            //bb_printf1("A 2 B data is [0x%08x]",gData);

            /*
            gFileHandle = (FILE_HANDLE_t *)gData;
            pRawFileCache = (FILE *)gFileHandle->handle1;
            CurrentCodec2 = gFileHandle->codecType;

            #ifdef AAC_DEC_INCLUDE
            if(CurrentCodec2 == CODEC_AAC_DEC)
            {
                pAacFileHandleOffset = (FILE *)gFileHandle->handle2;
                pAacFileHandleSize = (FILE *)gFileHandle->handle3;
            }
            #endif

            #ifdef FLAC_DEC_INCLUDE
            if(CurrentCodec2 == CODEC_FLAC_DEC)
            {
                pFlacFileHandleBake = (FILE *)gFileHandle->handle2;
            }
            #endif
            */

        //record file close
        case MEDIA_MSGBOX_CMD_REC_FILE_CLOSE:
            if(gData)
                gFileOpStatus = RECORD_FILE_CLOSE_CMPL;

            bb_printf1("recive RECORD FILE_CLOSE_CMPL");
            gCmdDone = 1;
            break;

        case MEDIA_MSGBOX_CMD_FLAC_SEEKFAST_CMPL:
        case MEDIA_MSGBOX_CMD_FLAC_SEEKFAST_INFO_CMPL:
            gCmdDone = 1;
            break;


        default:
            return;
    }

}

void RegHifiFileServer()
{
    bb_printf1("HifiFileInit ...");
    gCmdDone   = 0;

    IntRegister2(INT_ID_MAILBOX2 , (void*)MailBoxFileService);
    IntPendingClear2(INT_ID_MAILBOX2);
    IntEnable2(INT_ID_MAILBOX2);
    MailBoxEnableA2BInt(MAILBOX_ID_0, MAILBOX_INT_2);
}
/*
*********************************************************************************************************
*                                              Main(void)
*
* Description:  This Function is the first function.
*
* Argument(s) : none
*
* Return(s)   : int
*
* Note(s)     : none.
*********************************************************************************************************
*/



/*
--------------------------------------------------------------------------------
  Function name :
  Author        : ZHengYongzhi
  Description   : 模块信息表

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

extern uint32 Image$$BB_SYS_DATA$$ZI$$Base;
extern uint32 Image$$BB_SYS_DATA$$ZI$$Length;

void ScatterLoader2(void)
{
    uint32 i,len;
    uint8  *pDest;

    //清除Bss段
    pDest = (uint8*)((uint32)(&Image$$BB_SYS_DATA$$ZI$$Base));
    len = (uint32)((uint32)(&Image$$BB_SYS_DATA$$ZI$$Length));
    for (i = 0; i < len; i++)
    {
        *pDest++ = 0;
    }
}

int Main2(void)
{
    uint outptr;
    uint OutLength;

    ScatterLoader2();

#ifdef BB_SYS_JTAG
    MailBoxWriteB2ACmd(MSGBOX_CMD_SYSTEM_START_OK, MAILBOX_ID_0, MAILBOX_CHANNEL_0);
    MailBoxWriteB2AData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_0);
#endif

    BSP_Init2();

    RegHifiDecodeServer();

    RegHifiFileServer();

    ClearMsg(MSG_AUDIO_DECODE_FILL_BUFFER);

    MailBoxWriteB2ACmd(MSGBOX_CMD_SYSTEM_START_OK, MAILBOX_ID_0, MAILBOX_CHANNEL_0);
    MailBoxWriteB2AData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_0);

    MemSet2(&gMediaBlockInfo,  0, sizeof(gMediaBlockInfo));
    MemSet2(&gRecordBlockInfo, 0, sizeof(gRecordBlockInfo));
    //PWM4 (for test)
    //Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin0,IOMUX_GPIO2A0_IO);
    //Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin0,GPIO_OUT);
    //Gpio_SetPinLevel(GPIO_CH2,GPIOPortA_Pin0,GPIO_LOW);

    while (1)
    {
        //process audio decode
        switch (pcb.audio_decode_status)
        {
            case AUDIO_DECODE_IDLE:
               #if 1
               IntMasterDisable2();
               if((pcb.audio_decode_status == AUDIO_DECODE_IDLE) && (gFileOpStatus == AUDIO_FILE_OPT_IDLE))
               {
                   __WFI2();
               }
               IntMasterEnable2();
               #endif

                break;

            case AUDIO_DECODE_OPEN:
                {
                    pcb.audio_decode_status = AUDIO_DECODE_IDLE;

                    if (1 != CodecOpen2(0, CODEC_OPEN_DECODE))
                    {
                        bb_printf1("###AUDIO_DECODE_OPEN error!###");
                        MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_DEC_OPEN_ERR, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                        MailBoxWriteB2AData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                        break;
                    }
                    bb_printf1("### get par ###");
                    {
                        CodecGetSampleRate2(&gMediaBlockInfo.SampleRate);
                        CodecGetChannels2(&gMediaBlockInfo.Channel);
                        CodecGetBitrate2(&gMediaBlockInfo.BitRate);
                        //CodecGetTime2(&gMediaBlockInfo.CurrentPlayTime);
                        CodecGetLength2(&gMediaBlockInfo.TotalPlayTime);
                        CodecGetBps2(&gMediaBlockInfo.Bps);

#ifdef MP3_DEC_INCLUDE2
                        if (CurrentCodec2 == CODEC_MP3_DEC)
                        {
                            mp3_wait_synth();
                        }
#endif
                        bb_printf1("### get par ok###");
                        AudioCodecGetBufferSize2(CurrentCodec2,gMediaBlockInfo.SampleRate);

                        //AudioCodecGetBufferSize2(CurrentCodec2,gMediaBlockInfo.SampleRate);

#ifdef FLAC_DEC_INCLUDE2
                        if (CurrentCodec2 != CODEC_FLAC_DEC)
#endif
                        {
#ifdef AAC_DEC_INCLUDE2
                            if (CurrentCodec2 != CODEC_AAC_DEC)
#endif
                            {
#ifdef HIFI_AlAC_DECODE2
                                if (CurrentCodec2 != CODEC_HIFI_ALAC_DEC)
#endif
                                {
                                    AudioFileChangeBuf2(pRawFileCache,CodecBufSize2);
                                }
                            }
                        }

                        bb_printf1("SampleRate = %d",gMediaBlockInfo.SampleRate);
                        bb_printf1("Channel = %d",gMediaBlockInfo.Channel);
                        bb_printf1("BitRate = %d",gMediaBlockInfo.BitRate);
                        //bb_printf1("CurrentPlayTime = %d",gMediaBlockInfo.CurrentPlayTime);
                        bb_printf1("TotalPlayTime = %d",gMediaBlockInfo.TotalPlayTime);
                        bb_printf1("Bps = %d",gMediaBlockInfo.Bps);
                    }

                    CodecGetCaptureBuffer2((short*)&gMediaBlockInfo.Outptr,&gMediaBlockInfo.OutLength);

                    MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_DEC_OPEN_CMPL, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    MailBoxWriteB2AData((UINT32)&gMediaBlockInfo, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                }
                break;

            case AUDIO_DECODE_DECODE:
                {
                    pcb.audio_decode_status = AUDIO_DECODE_IDLE;

                     if(GetMsg(MSG_AUDIO_DECODE_FILL_BUFFER))
                     {
                         AudioFileInput2(pRawFileCache);
                     }

                    //Gpio_SetPinLevel(GPIO_CH2,GPIOPortA_Pin0,GPIO_HIGH);
                    if (1 != CodecDecode2())
                    {
                        bb_printf1("###AUDIO_DECODE error or decode end ###");
                        MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_DECODE_ERR, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                        MailBoxWriteB2AData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                        break;
                    }
                    //Gpio_SetPinLevel(GPIO_CH2,GPIOPortA_Pin0,GPIO_LOW);

#ifdef MP3_DEC_INCLUDE2
                    if (CurrentCodec2 == CODEC_MP3_DEC)
                    {
                        mp3_wait_synth();
                    }
#endif

                    CodecGetTime2(&gMediaBlockInfo.CurrentPlayTime);

                    //bb_printf1("codec decode over");
                    CodecGetCaptureBuffer2((short*)&gMediaBlockInfo.Outptr, &gMediaBlockInfo.OutLength);
                    //bb_printf1("codec get buffer ");

                    MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_DECODE_CMPL, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    MailBoxWriteB2AData((UINT32)&gMediaBlockInfo, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                }
                break;

            case AUDIO_DECODE_SEEK:
                {
                    pcb.audio_decode_status = AUDIO_DECODE_IDLE;

                    if ( 1 != CodecSeek2(pcb.audio_decode_param , 0))
                    {
                        bb_printf1("codec seek fail");
                        //TODO...
                    }

                    MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_DECODE_SEEK_CMPL, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    MailBoxWriteB2AData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                }
                break;

            case AUDIO_DECODE_GETBUFFER:
                {
//                CodecGetCaptureBuffer2((short*)&gMediaBlockInfo->Outptr,&gMediaBlockInfo->OutLength);
//
//                MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_DECODE_GETBUFFER_CMPL, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
//                MailBoxWriteB2AData((UINT32)gMediaBlockInfo, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                }
                break;

            case AUDIO_DECODE_GETTIME:
                {
                    /*
                    pcb.audio_decode_status = AUDIO_DECODE_IDLE;
                    CodecGetTime2(&gMediaBlockInfo.CurrentPlayTime);

                    MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_DECODE_GETTIME_CMPL, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    MailBoxWriteB2AData((UINT32)&gMediaBlockInfo,MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    */
                }
                break;

            case AUDIO_DECODE_CLOSE:
                {
                    pcb.audio_decode_status = AUDIO_DECODE_IDLE;
                    CodecClose2();

                    bb_printf1("AUDIO_DECODE_CLOSE gFileOpStatus=%d\n",gFileOpStatus);

                    MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_DECODE_CLOSE_CMPL, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    MailBoxWriteB2AData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                }
                break;


            case AUDIO_DECODE_FILE_OPEN:
                {
                    pcb.audio_decode_status = AUDIO_DECODE_IDLE;
                    AudioFileFuncInit2(pRawFileCache, HIFI_AUDIO_BUF_SIZE - 1024);

                    //RKFileFuncInit();
                }
                break;
//------------------------------------------------------------------------------
            case RECORD_ENCORD_INIT:
                {
#ifdef WAV_DEC_INCLUDE2
                    pcb.audio_decode_status = AUDIO_DECODE_IDLE;
                    bb_printf1(" --- adpcm init process --- ");
                    //tPCM_enc pPCM;
                    MemCpy2((uint8*)&PCM_source, (uint8*)pcb.audio_decode_param, sizeof(tPCM_enc));
                    RecordADPCMInit(&PCM_source);

                    bb_printf1(" --- usBytesPerBlock = %d--- ",PCM_source.usBytesPerBlock);
                    bb_printf1(" --- usSamplesPerBlock = %d--- ",PCM_source.usSamplesPerBlock);

                    RecordBufferInit();

                    gRecordBlockInfo.PCM_source = (UINT32)&PCM_source;

                    pRecordEncodeInputBuffer2 = pRecordPCMInputBuffer2;
                    gRecordBlockInfo.unenc_bufptr = (UINT32)pRecordEncodeInputBuffer2;

                    MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_ENCODE_INIT_CMPL, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    MailBoxWriteB2AData((UINT32)&gRecordBlockInfo,MAILBOX_ID_0, MAILBOX_CHANNEL_1);
#endif
                }
                break;

//            case RECORD_ENCORD_GETDATA:
//                {
//#ifdef WAV_DEC_INCLUDE2
//                    pcb.audio_decode_status = AUDIO_DECODE_IDLE;
//                    //RecordBufferInit();
//                    //bb_printf1(" record buffer init OK ");
//                    RecordInputBufIndex   = 1 - RecordInputBufIndex;
//                    pRecordPCMInputBuffer2    = pRecordInputBuffer2 + RecordInputBufIndex * WAV_AD_PIPO_BUFFER_SIZE;  //switch buffer
//
//                    pRecordEncodeInputBuffer2 = pRecordPCMInputBuffer2;
//                    gRecordBlockInfo.bufptr = (UINT32)pRecordEncodeInputBuffer2;
//
//                    MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_ENCODE_DATA_REQ, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
//                    MailBoxWriteB2AData((UINT32)&gRecordBlockInfo, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
//                    //bb_printf1(" send cmd to A core: request data ");
//
//                    //MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_ENCODE_DATA_REQ, MAILBOX_ID_0, MAILBOX_CHANNEL_2);
//                    //MailBoxWriteB2AData((UINT32)&gRecordBlockInfo, MAILBOX_ID_0, MAILBOX_CHANNEL_2);
//#endif
//                }
//
//                break;

             case RECORD_ENCORD_ENCODE:
#ifdef WAV_DEC_INCLUDE2
                {
                    pcb.audio_decode_status = AUDIO_DECODE_IDLE;
                    //record data encode process...
                    #if(RECORD_TYPE_CONFIG == RECORD_PCM)
                        //other data process to inputbuffer add here
                        //......
                        gRecordBlockInfo.enc_bufptr = (UINT32)pRecordEncodeInputBuffer2;

                        gRecordBlockInfo.length = pcb.audio_decode_param;

                        RecordInputBufIndex   = 1 - RecordInputBufIndex;
                        pRecordPCMInputBuffer2    = pRecordInputBuffer2 + RecordInputBufIndex * WAV_AD_PIPO_BUFFER_SIZE;  //switch buffer

                        pRecordEncodeInputBuffer2 = pRecordPCMInputBuffer2;
                        gRecordBlockInfo.unenc_bufptr = (UINT32)pRecordEncodeInputBuffer2;

                        MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_ENCODE_CMPL, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                        MailBoxWriteB2AData((UINT32)&gRecordBlockInfo, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                    #else
                    //ohter record type
                    //TODO:
                    //...
                    #endif
                }
#endif
                break;

            default :
                break;
        }

        if(GetMsg(MSG_AUDIO_DECODE_FILL_BUFFER))
        {
            AudioFileInput2(pRawFileCache);
        }


        //process hifi file operate
        switch (gFileOpStatus)
        {
            case AUDIO_FILE_OPT_IDLE:

               #if 1
               IntMasterDisable2();
               if((pcb.audio_decode_status == AUDIO_DECODE_IDLE) && (gFileOpStatus == AUDIO_FILE_OPT_IDLE))
               {
                   __WFI2();
               }
               IntMasterEnable2();
               #endif

                break;

            case AUDIO_FILE_OPEN_CMPL:
                gFileOpStatus = AUDIO_FILE_OPT_IDLE;

                //bb_printf1("AUDIO file func init!");

                AudioIntAndDmaInit2();
                AudioHWInit2();

                AudioCodecGetBufferSize2(CurrentCodec2,CodecFS_44100Hz);
                AudioFileFuncInit2(pRawFileCache, HIFI_AUDIO_BUF_SIZE - 1024);

#if 1
#ifdef AAC_DEC_INCLUDE
                if (CurrentCodec2 == CODEC_AAC_DEC)
                {
                    ClearMsg(MSG_AUDIO_DECODE_FILL_BUFFER);
                    RKFileFuncInit2();
                    HifiFileSeek(0, SEEK_SET, pRawFileCache);
                }
#endif
#ifdef HIFI_AlAC_DECODE
                if (CurrentCodec2 == CODEC_HIFI_ALAC_DEC)
                {
                    ClearMsg(MSG_AUDIO_DECODE_FILL_BUFFER);
                    RKFileFuncInit2();
                    HifiFileSeek(0, SEEK_SET, pRawFileCache);
                }
#endif
#endif

                MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_FILE_OPEN_HANDSHK, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                MailBoxWriteB2AData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                //bb_printf1("file open handshake!");

                break;

            case AUDIO_FILE_SEEK_CMPL:
                gFileOpStatus = AUDIO_FILE_OPT_IDLE;
                break;

            case AUDIO_FILE_READ_CMPL:
                gFileOpStatus = AUDIO_FILE_OPT_IDLE;
                break;

            case AUDIO_FILE_WRITE_CMPL:
                gFileOpStatus = AUDIO_FILE_OPT_IDLE;
                break;

            case AUDIO_FILE_TELL_CMPL:
                gFileOpStatus = AUDIO_FILE_OPT_IDLE;
                break;

            case AUDIO_FILE_GET_LEN_CMPL:
                gFileOpStatus = AUDIO_FILE_OPT_IDLE;
                break;

            case AUDIO_FILE_CLOSE_CMPL:
            {
                bb_printf1("AUDIO_FILE_CLOSE_CMPL\n");
                gFileOpStatus = AUDIO_FILE_OPT_IDLE;
                AudioIntAndDmaDeInit2();
                AudioHWDeInit2();

                bb_printf1("file close cmpl\n");
                //must MAILBOX_CHANNEL_2 for stremtask give semph
                //MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_FILE_CLOSE_HANDSHK, MAILBOX_ID_0, MAILBOX_CHANNEL_2);
                //MailBoxWriteB2AData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_2);
                MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_FILE_CLOSE_HANDSHK, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                MailBoxWriteB2AData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_1);
            }
                break;
//------------------------------------------------------
            case RECORD_FILE_CREATE_CMPL:    //used for record
                gFileOpStatus = AUDIO_FILE_OPT_IDLE;

                RKFileFuncInit2();

                MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_FILE_CREATE_HANDSHK, MAILBOX_ID_0, MAILBOX_CHANNEL_2);
                MailBoxWriteB2AData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_2);
                break;

            case RECORD_FILE_CLOSE_CMPL:
                gFileOpStatus = AUDIO_FILE_OPT_IDLE;

                MailBoxWriteB2ACmd(MEDIA_MSGBOX_CMD_REC_FILE_CLOSE_HANDSHK, MAILBOX_ID_0, MAILBOX_CHANNEL_2);
                MailBoxWriteB2AData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_2);
                break;
        }

    }
}


