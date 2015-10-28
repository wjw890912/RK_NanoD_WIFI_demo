#ifndef APE_GLOBALVAR_DEFINE_H
#define APE_GLOBALVAR_DEFINE_H



#include "ape_all.h"
#include "ape_globalvardeclaration.h"
#include "ape_decompress.h"
#include "ape_unbitarray.h"
#include "ape_unmac.h"
#include "ape_newpredictor.h"
#include "ape_rollbuffer.h"
/****************************************************************
--Note-- 20070620 Huangxudong
Ape_前缀:表示Ape解码的模块前缀;
g 前缀:   表示全局变量
p 前缀:   表示全局指针变量
t 前缀:   表示表格
****************************************************************/
#pragma arm section code = "ApeDecCode", rodata = "ApeDecCode", rwdata = "ApeDecData", zidata = "ApeDecBss"



#if 1
ape_char    Ape_gReadBuffer[512];
#else
ape_char    Ape_gReadBuffer_dummy[512];
#endif
/////Macro control////////////////////
//#define DECODE_BLOCK_SIZE        4096  //this macro control extended blocks of outbuffer,one block includes 16bits R,L samples.
//#define BLOCKS_PER_DECODE_CUSTOM_HANDLE 1152 //means that outbuffer blocks is 1152.
//#define BLOCKALIGN               4
//#define BLOCKS_PER_DECODE               1152//the macro represents length of block which each is decoded and written to wav file.

//the above two macros decide that length of outbuffer is (2304+4096)*4+1+256 bytes.

//#define BLOCKS_PER_BITSTREAMREAD  1080 //the macro represents that length of inbuffer is (1080+16)*4 bytes.

#if 1
////////allocate Global Space//////////////////////////////
//ape_uint16  Ape_gOutBuffer[2][BLOCKS_PER_DECODE * BLOCKALIGN];//Ape解码模块的放置pcm数据的输出双缓冲区
ape_uint32  Ape_gOutBufferOffset0;//该变量表示Ape_gOutBuffer的填充pcm数据的偏移量
ape_uint32  Ape_gOutBufferOffset1;//该变量表示Ape_gOutBuffer的填充pcm数据的偏移量
ape_uint16  Ape_gDualBufferIndex;//该变量表示控制双缓冲区的索引
ape_uint16* Ape_pOutBufferLeft;//该指针指向tAPE结构体的psLeft空间
ape_uint16* Ape_pOutBufferRight;//该指针指向tAPE结构体的psRight空间
//ape_uint32  Ape_gInBuffer[BLOCKS_PER_BITSTREAMREAD + 16];//存放Ape比特流的输入缓冲区
ape_uint32* Ape_pInBuffer;//该指针指向tAPE结构体里的pcEncodedData空间
ape_int16   Ape_gBeginDecodeFrameFlag;//表示开始ape解码的帧标记
ape_int16   Ape_gForwardBackIndex;//表示快进快退寻找最匹配的帧索引
ape_int16   Ape_gForwardBackFlag;//1表示启动快进快退,0表示取消快进快退;
ape_uint32  Ape_gForwardBackOffset;//进行快进快退的相对文件开始位置的偏移量;
ape_uint16  Ape_gDecodeErrorFlag;//解码过程异常标记,0表示成功,非0表示失败
ape_int32   Ape_gTotalBlocks;//表示ape文件的总blocks数目
ape_int32   Ape_gBlocksLeft;//表示未解码的blocks数目
ape_int32   Ape_gYValue;//20071124
ape_int32   Ape_gnYValue;//20071126

//---global variable in Maclib.c--------------------------------------
//ape_int16 Ape_gIApeDecompressMalloc[(sizeof(CAPEDecompress)+1)/2];//sizeof(CAPEDecompress)
CAPEDecompress Ape_gIApeDecompressMalloc;//[11];//sizeof(CAPEDecompress)

//---global variable in APEDecompress.c--------------------------------------
ape_int32 Ape_gBlockAlign;
ape_int32 Ape_gCurrentFrame;

ape_int32 Ape_gStartBlock;
ape_int32 Ape_gFinishBlock;
ape_int32 Ape_gCurrentBlock;
ape_BOOL  Ape_gIsRanged;
ape_BOOL  Ape_gDecompressorInitial;

// static variable used during decoding
CPrepare Ape_gPrepare;
ape_uint32 Ape_gCRC;
ape_uint32 Ape_gStoredCRC;
ape_int32 Ape_gSpecialCodes;
CAPEInfo* Ape_pApeInfo;
CUnBitArrayBase* Ape_pUnBitArray;
#if 1
struct UNBIT_ARRAY_STATE Ape_gBitArrayStateX;//开辟X熵编码结构体
struct UNBIT_ARRAY_STATE Ape_gBitArrayStateY;//开辟Y熵编码结构体
#endif
#endif

WAVEFORMATEX Ape_gWfeInput;

#if 1
IPredictorDecompress* Ape_pNewPredictorX;//定义X预测器指针
IPredictorDecompress* Ape_pNewPredictorY;//定义Y预测器指针
#endif
CPredictorDecompress3950toCurrent Ape_gPredDecomMallocX;//开辟X预测器结构体
CPredictorDecompress3950toCurrent Ape_gPredDecomMallocY;//开辟Y预测器结构体

ape_int32 Ape_gLastXValue;
ape_int16 Ape_gBytesToSkipMalloc[16];//need to verify the space

ape_BOOL Ape_gErrorDecodingCurrentFrame;
ape_int32 Ape_gCurrentFrameBufferBlock;
ape_int32 Ape_gFrameBufferFinishedBlocks;
CCircleBuffer Ape_gCbFrameBuffer;//CireBuffer变量

//-----------------------------------------

/////the following global variables are used in UnBitArray.c/////////////////////////////////
//add by Wei.Hisung 2007.03.06
ape_uint32 Ape_gElements;
ape_uint32 Ape_gBytesNum;
ape_uint32 Ape_gBitsNum;
ape_int32 Ape_gVersion;
CIO * Ape_pIO;
ape_uint32 Ape_gCurrentBitIndex;
ape_uint32* Ape_pBitArray;
//----------------------------------
ape_int32 Ape_gFlushCounter;
ape_int32 Ape_gFinalizeCounter;
//struct RANGE_CODER_STRUCT_DECOMPRESS    Ape_gRangeCoderInfo;
ape_uint32 Ape_gBufferValue;
ape_uint32 Ape_gLowValue;
ape_uint32 Ape_gRangeValue;
ape_uint32 Ape_gRefillBitThreshold;
//-----------------------------------
#ifdef APE_USE_TABLE_ROOM
ape_uint32 *Ape_gtPowersOfTwoMinusOne;
ape_uint32 *Ape_gtKSumMinBoundary;
ape_uint32 *Ape_gtRangeTotalOne;
ape_uint32 *Ape_gtRangeWidthOne;
ape_uint32 *Ape_gtRangeTotalTwo;
ape_uint32 *Ape_gtRangeWidthTwo;
#else
ape_uint32 Ape_gtPowersOfTwoMinusOne[33];
ape_uint32 Ape_gtKSumMinBoundary[32];
ape_uint32 Ape_gtRangeTotalOne[65];
ape_uint32 Ape_gtRangeWidthOne[64];
ape_uint32 Ape_gtRangeTotalTwo[65];
ape_uint32 Ape_gtRangeWidthTwo[64];
#endif

#ifdef APE_CHECK_TABLE_ROOM
ape_uint32 Ape_gtPowersOfTwoMinusOne_s[33] =
{
 0ul,1ul,3ul,7ul,
 15ul,31ul,63ul,127ul,
 255ul,511ul,1023ul,2047ul,
 4095ul,8191ul,16383ul,32767ul,
 65535ul,131071ul,262143ul,524287ul,
 1048575ul,2097151ul,4194303ul,8388607ul,
 16777215ul,33554431ul,67108863ul,134217727ul,
 268435455ul,536870911ul,1073741823ul,2147483647ul,
 4294967295ul
 };
//ape_uint32 Ape_gtPowersOfTwoMinusOneReversed[33] = {4294967295,2147483647,1073741823,536870911,268435455,134217727,67108863,33554431,16777215,8388607,4194303,2097151,1048575,524287,262143,131071,65535,32767,16383,8191,4095,2047,1023,511,255,127,63,31,15,7,3,1,0};
ape_uint32 Ape_gtKSumMinBoundary_s[32] =
{
 0ul,32ul,64ul,128ul,
 256ul,512ul,1024ul,2048ul,
 4096ul,8192ul,16384ul,32768ul,
 65536ul,131072ul,262144ul,524288ul,
 1048576ul,2097152ul,4194304ul,8388608ul,
 16777216ul,33554432ul,67108864ul,134217728ul,
 268435456ul,536870912ul,1073741824ul,2147483648ul,
 0ul,0ul,0ul,0ul
 };

ape_uint32 Ape_gtRangeTotalOne_s[65] =
{
 0ul,14824ul,28224ul,39348ul,
 47855ul,53994ul,58171ul,60926ul,
 62682ul,63786ul,64463ul,64878ul,
 65126ul,65276ul,65365ul,65419ul,
 65450ul,65469ul,65480ul,65487ul,
 65491ul,65493ul,65494ul,65495ul,
 65496ul,65497ul,65498ul,65499ul,
 65500ul,65501ul,65502ul,65503ul,
 65504ul,65505ul,65506ul,65507ul,
 65508ul,65509ul,65510ul,65511ul,
 65512ul,65513ul,65514ul,65515ul,
 65516ul,65517ul,65518ul,65519ul,
 65520ul,65521ul,65522ul,65523ul,
 65524ul,65525ul,65526ul,65527ul,
 65528ul,65529ul,65530ul,65531ul,
 65532ul,65533ul,65534ul,65535ul,
 65536ul
 };
ape_uint32 Ape_gtRangeWidthOne_s[64] =
{
 14824ul,13400ul,11124ul,8507ul,
 6139ul,4177ul,2755ul,1756ul,
 1104ul,677ul,415ul,248ul,
 150ul,89ul,54ul,31ul,
 19ul,11ul,7ul,4ul,
 2ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul
 };

ape_uint32 Ape_gtRangeTotalTwo_s[65] =
{
 0ul,19578ul,36160ul,48417ul,
 56323ul,60899ul,63265ul,64435ul,
 64971ul,65232ul,65351ul,65416ul,
 65447ul,65466ul,65476ul,65482ul,
 65485ul,65488ul,65490ul,65491ul,
 65492ul,65493ul,65494ul,65495ul,
 65496ul,65497ul,65498ul,65499ul,
 65500ul,65501ul,65502ul,65503ul,
 65504ul,65505ul,65506ul,65507ul,
 65508ul,65509ul,65510ul,65511ul,
 65512ul,65513ul,65514ul,65515ul,
 65516ul,65517ul,65518ul,65519ul,
 65520ul,65521ul,65522ul,65523ul,
 65524ul,65525ul,65526ul,65527ul,
 65528ul,65529ul,65530ul,65531ul,
 65532ul,65533ul,65534ul,65535ul,
 65536ul
 };
ape_uint32 Ape_gtRangeWidthTwo_s[64] =
{
 19578ul,16582ul,12257ul,7906ul,
 4576ul,2366ul,1170ul,536ul,
 261ul,119ul,65ul,31ul,
 19ul,10ul,6ul,3ul,
 3ul,2ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul,
 1ul,1ul,1ul,1ul
 };

ape_uint32 Ape_gtCRC32_s[256] =
{0ul,        1996959894ul,3993919788ul,2567524794ul,
 124634137ul,1886057615ul,3915621685ul,2657392035ul,
 249268274ul,2044508324ul,3772115230ul,2547177864ul,
 162941995ul,2125561021ul,3887607047ul,2428444049ul,
 498536548ul,1789927666ul,4089016648ul,2227061214ul,
 450548861ul,1843258603ul,4107580753ul,2211677639ul,
 325883990ul,1684777152ul,4251122042ul,2321926636ul,
 335633487ul,1661365465ul,4195302755ul,2366115317ul,
 997073096ul,1281953886ul,3579855332ul,2724688242ul,
 1006888145ul,1258607687ul,3524101629ul,2768942443ul,
 901097722ul,1119000684ul,3686517206ul,2898065728ul,
 853044451ul,1172266101ul,3705015759ul,2882616665ul,
 651767980ul,1373503546ul,3369554304ul,3218104598ul,
 565507253ul,1454621731ul,3485111705ul,3099436303ul,
 671266974ul,1594198024ul,3322730930ul,2970347812ul,
 795835527ul,1483230225ul,3244367275ul,3060149565ul,
 1994146192ul,31158534ul,2563907772ul,4023717930ul,
 1907459465ul,112637215ul,2680153253ul,3904427059ul,
 2013776290ul,251722036ul,2517215374ul,3775830040ul,
 2137656763ul,141376813ul,2439277719ul,3865271297ul,
 1802195444ul,476864866ul,2238001368ul,4066508878ul,
 1812370925ul,453092731ul,2181625025ul,4111451223ul,
 1706088902ul,314042704ul,2344532202ul,4240017532ul,
 1658658271ul,366619977ul,2362670323ul,4224994405ul,
 1303535960ul,984961486ul,2747007092ul,3569037538ul,
 1256170817ul,1037604311ul,2765210733ul,3554079995ul,
 1131014506ul,879679996ul,2909243462ul,3663771856ul,
 1141124467ul,855842277ul,2852801631ul,3708648649ul,
 1342533948ul,654459306ul,3188396048ul,3373015174ul,
 1466479909ul,544179635ul,3110523913ul,3462522015ul,
 1591671054ul,702138776ul,2966460450ul,3352799412ul,
 1504918807ul,783551873ul,3082640443ul,3233442989ul,
 3988292384ul,2596254646ul,62317068ul,1957810842ul,
 3939845945ul,2647816111ul,81470997ul,1943803523ul,
 3814918930ul,2489596804ul,225274430ul,2053790376ul,
 3826175755ul,2466906013ul,167816743ul,2097651377ul,
 4027552580ul,2265490386ul,503444072ul,1762050814ul,
 4150417245ul,2154129355ul,426522225ul,1852507879ul,
 4275313526ul,2312317920ul,282753626ul,1742555852ul,
 4189708143ul,2394877945ul,397917763ul,1622183637ul,
 3604390888ul,2714866558ul,953729732ul,1340076626ul,
 3518719985ul,2797360999ul,1068828381ul,1219638859ul,
 3624741850ul,2936675148ul,906185462ul,1090812512ul,
 3747672003ul,2825379669ul,829329135ul,1181335161ul,
 3412177804ul,3160834842ul,628085408ul,1382605366ul,
 3423369109ul,3138078467ul,570562233ul,1426400815ul,
 3317316542ul,2998733608ul,733239954ul,1555261956ul,
 3268935591ul,3050360625ul,752459403ul,1541320221ul,
 2607071920ul,3965973030ul,1969922972ul,40735498ul,
 2617837225ul,3943577151ul,1913087877ul,83908371ul,
 2512341634ul,3803740692ul,2075208622ul,213261112ul,
 2463272603ul,3855990285ul,2094854071ul,198958881ul,
 2262029012ul,4057260610ul,1759359992ul,534414190ul,
 2176718541ul,4139329115ul,1873836001ul,414664567ul,
 2282248934ul,4279200368ul,1711684554ul,285281116ul,
 2405801727ul,4167216745ul,1634467795ul,376229701ul,
 2685067896ul,3608007406ul,1308918612ul,956543938ul,
 2808555105ul,3495958263ul,1231636301ul,1047427035ul,
 2932959818ul,3654703836ul,1088359270ul,936918000ul,
 2847714899ul,3736837829ul,1202900863ul,817233897ul,
 3183342108ul,3401237130ul,1404277552ul,615818150ul,
 3134207493ul,3453421203ul,1423857449ul,601450431ul,
 3009837614ul,3294710456ul,1567103746ul,711928724ul,
 3020668471ul,3272380065ul,1510334235ul,755167117ul
 };

#endif

/////the following global variables are used in UnBitArrayBase.c/////////////////////////////////
//static const ape_uint32 Ape_gtPowersOfTwoMinusOne[33] = {0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535,131071,262143,524287,1048575,2097151,4194303,8388607,16777215,33554431,67108863,134217727,268435455,536870911,1073741823,2147483647,4294967295};
//add by Wei.Hisung 2007.03.06
ape_uint32 Ape_gElementsBase;
ape_uint32 Ape_gBytesValueBase;
ape_uint32 Ape_gBitsValueBase;
ape_int32 Ape_gVersionBase;
CIO* Ape_pIOBase;
ape_uint32 Ape_gCurrentBitIndexBase;
ape_uint32 * Ape_pBitArrayBase;
//ape_int16 Ape_gUnBitArrayBaseMalloc[sizeof(CUnBitArray)/2];//(CUnBitArrayBase * )malloc(sizeof(CUnBitArray))or(CUnBitArrayBase * )malloc(sizeof(CUnBitArrayOld))
CUnBitArray Ape_gUnBitArrayBaseMalloc;//[12];//(CUnBitArrayBase * )malloc(sizeof(CUnBitArray))or(CUnBitArrayBase * )malloc(sizeof(CUnBitArrayOld))
//----------------------------------

/*

Ape_gtPowersOfTwoMinusOne                    = (ape_uint32 *)(pTableBase + TBL_OFFSET_Ape_gtPowersOfTwoMinusOne);
	Ape_gtKSumMinBoundary                        = (ape_uint32 *)(pTableBase + TBL_OFFSET_Ape_gtKSumMinBoundary    );
	Ape_gtRangeTotalOne                          = (ape_uint32 *)(pTableBase + TBL_OFFSET_Ape_gtRangeTotalOne      );
	Ape_gtRangeWidthOne                          = (ape_uint32 *)(pTableBase + TBL_OFFSET_Ape_gtRangeWidthOne      );
	Ape_gtRangeTotalTwo                          = (ape_uint32 *)(pTableBase + TBL_OFFSET_Ape_gtRangeTotalTwo      );
	Ape_gtRangeWidthTwo                          = (ape_uint32 *)(pTableBase + TBL_OFFSET_Ape_gtRangeWidthTwo      );
	Ape_gtCRC32                                  = (ape_uint32 *)(pTableBase + TBL_OFFSET_Ape_gtCRC32              );





*/










#ifdef APE_USE_TABLE_ROOM
ape_uint32 *Ape_gtCRC32;
#else
/////the following global variables are used in Prepare.c/////////////////////////////////
ape_uint32 Ape_gtCRC32[256] =
{0ul,        1996959894ul,3993919788ul,2567524794ul,
 124634137ul,1886057615ul,3915621685ul,2657392035ul,
 249268274ul,2044508324ul,3772115230ul,2547177864ul,
 162941995ul,2125561021ul,3887607047ul,2428444049ul,
 498536548ul,1789927666ul,4089016648ul,2227061214ul,
 450548861ul,1843258603ul,4107580753ul,2211677639ul,
 325883990ul,1684777152ul,4251122042ul,2321926636ul,
 335633487ul,1661365465ul,4195302755ul,2366115317ul,
 997073096ul,1281953886ul,3579855332ul,2724688242ul,
 1006888145ul,1258607687ul,3524101629ul,2768942443ul,
 901097722ul,1119000684ul,3686517206ul,2898065728ul,
 853044451ul,1172266101ul,3705015759ul,2882616665ul,
 651767980ul,1373503546ul,3369554304ul,3218104598ul,
 565507253ul,1454621731ul,3485111705ul,3099436303ul,
 671266974ul,1594198024ul,3322730930ul,2970347812ul,
 795835527ul,1483230225ul,3244367275ul,3060149565ul,
 1994146192ul,31158534ul,2563907772ul,4023717930ul,
 1907459465ul,112637215ul,2680153253ul,3904427059ul,
 2013776290ul,251722036ul,2517215374ul,3775830040ul,
 2137656763ul,141376813ul,2439277719ul,3865271297ul,
 1802195444ul,476864866ul,2238001368ul,4066508878ul,
 1812370925ul,453092731ul,2181625025ul,4111451223ul,
 1706088902ul,314042704ul,2344532202ul,4240017532ul,
 1658658271ul,366619977ul,2362670323ul,4224994405ul,
 1303535960ul,984961486ul,2747007092ul,3569037538ul,
 1256170817ul,1037604311ul,2765210733ul,3554079995ul,
 1131014506ul,879679996ul,2909243462ul,3663771856ul,
 1141124467ul,855842277ul,2852801631ul,3708648649ul,
 1342533948ul,654459306ul,3188396048ul,3373015174ul,
 1466479909ul,544179635ul,3110523913ul,3462522015ul,
 1591671054ul,702138776ul,2966460450ul,3352799412ul,
 1504918807ul,783551873ul,3082640443ul,3233442989ul,
 3988292384ul,2596254646ul,62317068ul,1957810842ul,
 3939845945ul,2647816111ul,81470997ul,1943803523ul,
 3814918930ul,2489596804ul,225274430ul,2053790376ul,
 3826175755ul,2466906013ul,167816743ul,2097651377ul,
 4027552580ul,2265490386ul,503444072ul,1762050814ul,
 4150417245ul,2154129355ul,426522225ul,1852507879ul,
 4275313526ul,2312317920ul,282753626ul,1742555852ul,
 4189708143ul,2394877945ul,397917763ul,1622183637ul,
 3604390888ul,2714866558ul,953729732ul,1340076626ul,
 3518719985ul,2797360999ul,1068828381ul,1219638859ul,
 3624741850ul,2936675148ul,906185462ul,1090812512ul,
 3747672003ul,2825379669ul,829329135ul,1181335161ul,
 3412177804ul,3160834842ul,628085408ul,1382605366ul,
 3423369109ul,3138078467ul,570562233ul,1426400815ul,
 3317316542ul,2998733608ul,733239954ul,1555261956ul,
 3268935591ul,3050360625ul,752459403ul,1541320221ul,
 2607071920ul,3965973030ul,1969922972ul,40735498ul,
 2617837225ul,3943577151ul,1913087877ul,83908371ul,
 2512341634ul,3803740692ul,2075208622ul,213261112ul,
 2463272603ul,3855990285ul,2094854071ul,198958881ul,
 2262029012ul,4057260610ul,1759359992ul,534414190ul,
 2176718541ul,4139329115ul,1873836001ul,414664567ul,
 2282248934ul,4279200368ul,1711684554ul,285281116ul,
 2405801727ul,4167216745ul,1634467795ul,376229701ul,
 2685067896ul,3608007406ul,1308918612ul,956543938ul,
 2808555105ul,3495958263ul,1231636301ul,1047427035ul,
 2932959818ul,3654703836ul,1088359270ul,936918000ul,
 2847714899ul,3736837829ul,1202900863ul,817233897ul,
 3183342108ul,3401237130ul,1404277552ul,615818150ul,
 3134207493ul,3453421203ul,1423857449ul,601450431ul,
 3009837614ul,3294710456ul,1567103746ul,711928724ul,
 3020668471ul,3272380065ul,1510334235ul,755167117ul
 };
#endif

/////the following global variables are used in APEDecompressOld.c/////////////////////////////////
//----------------------------------
ape_char* Ape_pBuffer;
ape_int32 Ape_gBufferTail;
ape_int32 Ape_gBlockAlignOld;
ape_int32 Ape_gCurrentFrameOld;
ape_int32 Ape_gStartBlockOld;
ape_int32 Ape_gFinishBlockOld;
ape_int32 Ape_gCurrentBlockOld;
ape_BOOL  Ape_gIsRangedOld;
CUnMAC    Ape_gUnMAC;
CAPEInfo* Ape_pAPEInfoOld;
ape_BOOL  Ape_gDecompressorInitialOld;
//----------------------------------

/////the following global variables are used in UnBitArrayOld.c/////////////////////////////////
//add by Wei.Hisung 2007.03.06
ape_uint32 Ape_gElementsOld;
ape_uint32 Ape_gBytesValueOld;
ape_uint32 Ape_gBitsValueOld;
ape_int32  Ape_gVersionOld;
CIO*       Ape_pIOOld;
ape_uint32 Ape_gCurrentBitIndexOld;
ape_uint32* Ape_pBitArrayOld;
//----------------------------------
ape_uint32 Ape_gKBitArrayOld;
ape_uint32 Ape_gKSum;
ape_uint32 Ape_gRefillBitThresholdOld;
//----------------------------------

//const ape_uint32 Ape_gtPowersOfTwoMinusOne[33] = {0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535,131071,262143,524287,1048575,2097151,4194303,8388607,16777215,33554431,67108863,134217727,268435455,536870911,1073741823,2147483647,4294967295};

ape_uint32 Ape_gtKSumMinBoundaryOld[32] =
{
 0ul,128ul,256ul,512ul,
 1024ul,2048ul,4096ul,8192ul,
 16384ul,32768ul,65536ul,131072ul,
 262144ul,524288ul,1048576ul,2097152ul,
 4194304ul,8388608ul,16777216ul,33554432ul,
 67108864ul,134217728ul,268435456ul,536870912ul,
 1073741824ul,2147483648ul,0ul,0ul,
 0ul,0ul,0ul,0ul
 };
ape_uint32 Ape_gtKSumMaxBoundaryOld[32] =
{
 128ul,256ul,512ul,1024ul,
 2048ul,4096ul,8192ul,16384ul,
 32768ul,65536ul,131072ul,262144ul,
 524288ul,1048576ul,2097152ul,4194304ul,
 8388608ul,16777216ul,33554432ul,67108864ul,
 134217728ul,268435456ul,536870912ul,1073741824ul,
 2147483648ul,0ul,0ul,0ul,
 0ul,0ul,0ul,0ul
 };
ape_uint32 Ape_gtPowersOfTwo[32] =
{
 1ul,2ul,4ul,8ul,
 16ul,32ul,64ul,128ul,
 256ul,512ul,1024ul,2048ul,
 4096ul,8192ul,16384ul,32768ul,
 65536ul,131072ul,262144ul,524288ul,
 1048576ul,2097152ul,4194304ul,8388608ul,
 16777216ul,33554432ul,67108864ul,134217728ul,
 268435456ul,536870912ul,1073741824ul,2147483648ul
 };
ape_uint32 Ape_gtPowersOfTwoReversed[32] =
{
 2147483648ul,1073741824ul,536870912ul,268435456ul,
 134217728ul,67108864ul,33554432ul,16777216ul,
 8388608ul,4194304ul,2097152ul,1048576ul,
 524288ul,262144ul,131072ul,65536ul,
 32768ul,16384ul,8192ul,4096ul,
 2048ul,1024ul,512ul,256ul,
 128ul,64ul,32ul,16ul,
 8ul,4ul,2ul,1ul
 };

/*
ape_uint32 Ape_gtPowersOfTwoMinusOne[33] =
{
 0ul,1ul,3ul,7ul,
 15ul,31ul,63ul,127ul,
 255ul,511ul,1023ul,2047ul,
 4095ul,8191ul,16383ul,32767ul,
 65535ul,131071ul,262143ul,524287ul,
 1048575ul,2097151ul,4194303ul,8388607ul,
 16777215ul,33554431ul,67108863ul,134217727ul,
 268435455ul,536870911ul,1073741823ul,2147483647ul,
 4294967295ul
 };
 */

ape_uint32 Ape_gtPowersOfTwoMinusOneReversed[33] =
{
 4294967295ul,2147483647ul,1073741823ul,536870911ul,
 268435455ul,134217727ul,67108863ul,33554431ul,
 16777215ul,8388607ul,4194303ul,2097151ul,
 1048575ul,524287ul,262143ul,131071ul,
 65535ul,32767ul,16383ul,8191ul,
 4095ul,2047ul,1023ul,511ul,
 255ul,127ul,63ul,31ul,
 15ul,7ul,3ul,1ul,
 0ul
 };

//const ape_uint32 Ape_gtKSumMinBoundary[32] = {0,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,0,0,0,0};
ape_uint32 Ape_gtKSumMaxBoundary[32] =
{
 32ul,64ul,128ul,256ul,
 512ul,1024ul,2048ul,4096ul,
 8192ul,16384ul,32768ul,65536ul,
 131072ul,262144ul,524288ul,1048576ul,
 2097152ul,4194304ul,8388608ul,16777216ul,
 33554432ul,67108864ul,134217728ul,268435456ul,
 536870912ul,1073741824ul,2147483648ul,0ul,
 0ul,0ul,0ul,0ul
 };

/////the following global variables are used in RollBuffer.c/////////////////////////////////
ape_int32 Ape_gPredABufferFastMalloc[2][(WINDOW_ELEMENTS + HISTORY_ELEMENTS)];
ape_int32 Ape_gPredBBufferFastMalloc[2][(WINDOW_ELEMENTS + HISTORY_ELEMENTS)];

ape_int32 Ape_gAdaptABufferFastMalloc[2][(WINDOW_ELEMENTS + HISTORY_ELEMENTS)];
ape_int32 Ape_gAdaptBBufferFastMalloc[2][(WINDOW_ELEMENTS + HISTORY_ELEMENTS)];

#if 0
ape_int32 Ape_gDeltaMCreateMalloc[3][2][(NN_WINDOW_ELEMENTS + NORDER)];
ape_int32 Ape_gInputCreateMalloc[1/*3*/][2][(NN_WINDOW_ELEMENTS + NORDER)];
#endif



/////the following global variables are used in NNFilter.c/////////////////////////////////
//ape_int32 Ape_gParyMMalloc[3][2][NORDER];//malloc(sizeof(short)*(aNNF->m_nOrder));//assume aNNF->m_nOrder is 16,but must test aNNF->m_nOrder using other ape file.


/////the following global variables are used in NewPredictor.c/////////////////////////////////
//ape_int32 m_pBuffer_malloc[2][HISTORY_ELEMENTS + WINDOW_BLOCKS];
#if 1
CNNFilter Ape_gCNNFilterMalloc[2];//[36];//malloc(sizeof( CNNFilter));
#endif
CNNFilter Ape_gCNNFilter1Malloc[2];//[36];//malloc(sizeof( CNNFilter));
CNNFilter Ape_gCNNFilter2Malloc[2];//[36];//malloc(sizeof( CNNFilter));


//----------maclib.c----ApeDecompressCreate()-----------------
CAPEInfo Ape_gDecompressInfoCreate;
//----------APEHeader.c---ApeHeaderAnalyzeCurrent()----------
struct APE_DESCRIPTOR Ape_gHeaderDescriptor;
//#define SEEKBYTE_TABLE_MAX 512
//#define WAVE_HEADER_MAX 50
ape_uint32 Ape_gHeaderSeekByteTableOffset;
ape_uint32 Ape_gHeaderSeekByteTable[SEEKBYTE_TABLE_MAX];
ape_uchar Ape_gHeaderSeekBitTable[SEEKBYTE_TABLE_MAX];
ape_uchar Ape_gWavHeaderData[WAVE_HEADER_MAX];
//----------APEInfo.c-----ApeInfoFile()------
CIO Ape_gInfoIO;
CAPETag Ape_gInfoApeTag;
//----------APETag.c-----ApeTagAnalyze()------
//ape_char CAPETag_RawTag[512];  -->Ape_gReadBuffer
//----------APETag.c-----ApeTagLoadField()----
//#define APETAG_FIELDS_NUM_MAX    64
//ape_char Ape_pApeTagAryFields[APETAG_FIELDS_NUM_MAX][32];
ape_char* Ape_pApeTagAryFields;
//----------APETag.c-----ApeTagFileInitialize()------
CIO Ape_gApeTagIO;
struct ID3_TAG Ape_gId3Tag;
///////////////////the following global variable in APESimple.c/////////
CIO* Ape_pIOOutput;
IAPEDecompress* Ape_pApeDecompress;
CIO Ape_gCIODecompressCore; //for write file

ape_int16 Ape_gTitleIndex;
ape_int16 Ape_gArtistIndex;
ape_int16 Ape_gAlbumIndex;

#pragma arm section code


#endif // #ifndef APE_GLOBALVAR_DEFINE_H
