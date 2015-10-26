/*
************************************************************************************************************************
*
*  Copyright (C),2006, Fuzhou Rockchip Co.,Ltd.
*
*  File name :     drmpd.h
*  Author:         hwg, zsx, zym
*  Description:
*  Remark:
*
*  History:
*           <author>      <time>     <version>       <desc>
*                         06/8/21       1.0
*
************************************************************************************************************************
*/
#ifndef __DRMPD_H__
#define __DRMPD_H__

#include "..\wmaInclude\predefine.h"
#ifndef WMAAPI_NO_DRM


#include "string.h"
//#include "memory.h"
//#include "assert.h"
#include "drmtype.h"

#if 1//def rk_nano_board
#define DRM_INTI_TEXT     __attribute__((section("WmaOpenCodecCode")))//__attribute__((section("WmaDrm9InitText")))
#define DRM_DECRYPT_TEXT  __attribute__((section("WmaHighLowCommonCode")))//__attribute__((section("WmaDrm9DecryptText")))
#define DRM_DECRYPT_BSS   __attribute__((section("WmaDecoderBss")))//__attribute__((section("WmaDrm9DecryptBss")))

#else
#define DRM_INTI_TEXT
#define DRM_DECRYPT_TEXT
#define DRM_DECRYPT_BSS

#endif
/*
 *     DEFINES
 */

#define IN
#define OUT

#define SIXTEEN_BIT_ADDRESSING 0

//#define NULL    ((void *)0)

#define DRM_SUCCESS        0
#define BUFFER_TOO_SMALL   1
#define DRM_NOTINITIALIZED 3


#define RC4_TABLESIZE      256
#define DES_KEYSIZE		     8
#define SHA_BLOCK_SIZE     64	/*Sha operates on 64 BYTE blocks */
#define SHA_DIGEST_LEN     20
#define DES_BLOCKLEN	     8
#define DES_DECRYPT        0

//#define COPYRIGHT_STRING_OFFSET 6
//#define COPYRIGHT_STRING_CHUNK_LEN 5


/*
 *     DATA
 */

//extern const BYTE PMSECRET_SEG0__[5];
//extern const BYTE pszCopyrightString[22];

extern UWORD32 unknown2[32];
extern const DRM_DWORD _DRM_Spbox[8][64];
extern const DRM_DWORD _DRM_Sel[8][64];



/*
 *     MACROS
 */
#if SIXTEEN_BIT_ADDRESSING
#define __CB_DECL(x) (x/2)


#else
#define __CB_DECL(x) (x)
//#define UWORD32_TO_UWORD64(x) (UWORD64)(x)
//#define SIZEOF(x) sizeof(x)
#define STRLEN(x) strlen(x)
#define MEMCMP(x, y, l) memcmp(x, y, l)
#define MEMCPY(to, from, l) memcpy(to, from, l)
#define GET_BYTE(pb,ib)             (pb)[(ib)]
#define PUT_BYTE(pb,ib,b)           (pb)[(ib)]=(b)
#define DRM_BYT_CopyBytes(to,tooffset,from,fromoffset,count)   \
                      memcpy(&((to)[(tooffset)]),&((from)[(fromoffset)]),(count))
#define BYTE2LITTLEENDIAN(dword, byte)                         \
                      DRM_BYT_CopyBytes((BYTE*)&(dword),0,(byte),0,__CB_DECL(4))
#define BYTES_TO_DWORD BYTE2LITTLEENDIAN
#define DWORD2LITTLEENDIAN( byte, dword )                      \
                      DRM_BYT_CopyBytes((byte),0,(DRM_BYTE*)&(dword),0,__CB_DECL(4))
#define DWORD_TO_BYTES DWORD2LITTLEENDIAN
#define WORDSWAP(d) ( ( (d) >> 16 ) + ( (d) << 16 ) )


#endif
/* pairwise independent function and summing step */
#define MP_C_STEP_P(pbData,L1, L2, L3, L4, L5, L6, t, sum) \
{\
    UWORD32   dwTemp = 0;\
    BYTES_TO_DWORD( dwTemp, pbData );\
    pbData += __CB_DECL(4);\
    t += dwTemp; \
    t *= L1; \
    t = WORDSWAP(t); \
    t *= L2; \
    t = WORDSWAP(t); \
    t *= L3; \
    t = WORDSWAP(t); \
    t *= L4; \
    t = WORDSWAP(t); \
    t *= L5; \
    t += L6; \
    sum += t; \
}

#define MP_C_STEP(Data,L1, L2, L3, L4, L5, L6, t, sum) \
    t += Data; \
    t *= L1; \
    t = WORDSWAP(t); \
    t *= L2; \
    t = WORDSWAP(t); \
    t *= L3; \
    t = WORDSWAP(t); \
    t *= L4; \
    t = WORDSWAP(t); \
    t *= L5; \
    t += L6; \
    sum += t;

/* step to reverse action of multiply-&-swap rounds */
#define INV_STEP_C(iL1, iL2, iL3, iL4, iL5) \
    tmp *= iL5; \
    tmp = WORDSWAP(tmp); \
    tmp *= iL4; \
    tmp = WORDSWAP(tmp); \
    tmp *= iL3; \
    tmp = WORDSWAP(tmp); \
    tmp *= iL2; \
    tmp = WORDSWAP(tmp); \
    tmp *= iL1;

//void memcpy(
//       OUT   void  *dst,
//    IN const void  *src,
//    IN       UWORD32  count )
//{
//    while( count > 0 )
//    {
//        *(BYTE*)dst = *(BYTE*)src;
//        dst = (char *)dst + 1;
//        src = (char *)src + 1;
//        count--;
//    }
//}

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define ROTATE_RIGHT(x, n) (((x) >> (n)) | ((x) << (32-(n))))
#ifdef WMA_TABLE_ROOM_VERIFY
#define F(l,r,S) { \
    work = ((DRM_DWORD *)pTable)[S  ]; \
    zork = ((DRM_DWORD *)pTable)[S+1];  \
    work ^= r; \
    zork ^= r; \
    zork = ROTATE_RIGHT(zork, 4); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)p__DRM_Spbox + __CB_DECL((work      & 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)p__DRM_Spbox + 1*64*4 + __CB_DECL((zork      & 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)p__DRM_Spbox + 2*64*4 + __CB_DECL(((work>> 8)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)p__DRM_Spbox + 3*64*4 + __CB_DECL(((zork>> 8)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)p__DRM_Spbox + 4*64*4 + __CB_DECL(((work>>16)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)p__DRM_Spbox + 5*64*4 + __CB_DECL(((zork>>16)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)p__DRM_Spbox + 6*64*4 + __CB_DECL(((work>>24)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)p__DRM_Spbox + 7*64*4 + __CB_DECL(((zork>>24)& 0xfc))); }
#else
#define F(l,r,S) { \
    work = ((DRM_DWORD *)pTable)[S  ]; \
    zork = ((DRM_DWORD *)pTable)[S+1];  \
    work ^= r; \
    zork ^= r; \
    zork = ROTATE_RIGHT(zork, 4); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[0] + __CB_DECL((work      & 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[1] + __CB_DECL((zork      & 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[2] + __CB_DECL(((work>> 8)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[3] + __CB_DECL(((zork>> 8)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[4] + __CB_DECL(((work>>16)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[5] + __CB_DECL(((zork>>16)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[6] + __CB_DECL(((work>>24)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[7] + __CB_DECL(((zork>>24)& 0xfc))); }
#endif
#define PERM_OP(a,b,t,n,m) ((t)=((((a)>>(n))^(b))&(m)),\
    (b)^=(t),\
    (a)^=((t)<<(n)))

#define HPERM_OP(a,t,n,m) ((t)=((((a)<<(16-(n)))^(a))&(m)),\
    (a)=(a)^(t)^(t>>(16-(n))))


/*
 *     STRUCTURES
 */

typedef struct _CBCKey
{
    UWORD32 a1, b1, c1, d1, e1, f1, a2, b2, c2, d2, e2, f2;
} DRM_CBCKey;		// 24 WORD16

typedef struct __tagCBCState
{
    UWORD32 sum,t;
    BYTE buf[__CB_DECL(8)];
    LENGTH_TYPE dwBufLen;
} DRM_CBCState;		// 10 WORD16

typedef struct tag_RC4_KEYSTRUCT
{
	BYTE S[__CB_DECL(256)];     /* State table */
	BYTE i, j;        /* Indices */
} RC4_KEYSTRUCT;	// 129 WORD16

typedef struct _destable
{
	UWORD32 keytab[16][2];		// 64 WORD16
	BYTE mem2[__CB_DECL(16)];				// 8 WORD16
} DESTable;			// 72 WORD16

typedef struct __tagSHA_CONTEXT
{
	BYTE id[__CB_DECL(24)];				// 12 WORD16
	UWORD32 ABCDE[5];			// 10 WORD16
	UWORD32 dwHighByteCount;	// 2 WORD16
	UWORD32 dwLowByteCount;		// 2 WORD16
	BYTE  bTempBuffer[__CB_DECL(64)];		// 32 WORD16
} SHA_CONTEXT;		// 58 WORD16

typedef struct tag_LIC_Data
{
	char KID[__CB_DECL(25)];    			// 25 byte
	BYTE PMContentKey[__CB_DECL(16)];       // 16 byte
	char rights[__CB_DECL(4)]; 			// 2 WORD16
	char appSec[__CB_DECL(4)];				// 2 WORD16
	char expiryDate[__CB_DECL(4)];			// 2 WORD16
	BYTE mem4;            		// 11 byte	好像没用
	UWORD16 mem5;		// 好像没用
	UWORD16 cbContentKey;
	UWORD16 mem7;		// 好像没用
	UWORD16 mem8;
	UWORD16 mem9;
} LIC_Data;		// 32 WORD16

typedef struct tag_CIPHER_CONTEXT
{	// 与DRM_CIPHER_CONTEXT类似
	DRM_CBCKey m_mackey,	// 24 WORD16
               m_invmackey;	// 24 WORD16	#0x30
    DRM_CBCState m_cbcstate;// 10 WORD16	#0x60
	UWORD32 m_rc4key[2];	// 4 WORD16		#0x74
	BYTE m_shaOut[__CB_DECL(20)];		// 10 WORD16	#0x7c
	BYTE m_fInited;			// 1 Byte		#0x90
	BYTE m_fDecryptInited;	// 1 Byte		#0x91
//	WORD16 mem31;			// 1 WORD16		可能没用
	LENGTH_TYPE m_cbPacket;		// 2 WORD16		#0x94
	LENGTH_TYPE m_iPacket;		// 2 WORD16		#0x98
	UWORD32 m_desS1[2],		// 4 WORD16		#0x9c
			m_desS2[2];		// 4 WORD16		#0xa4
	RC4_KEYSTRUCT m_rc4ks;	// 129 WORD16	#0xac
//	WORD16 mem341;   		// 1 WORD16		可能没用
	DESTable m_destable;    // 72 WORD16	#0x1b0
//	UWORD32 mem342;			// 2 WORD16		#0x240	可能没用
	SHA_CONTEXT m_shadata;	// 58 WORD16	#0x244
	UWORD32 mem4[24];		// 48 WORD16	#0x2b8	to generate mackey
//	WORD16 mem5[116];		// 116 WORD16	#0x318	好像没用
} CIPHER_CONTEXT;		// 512 WORD16 - 116 WORD16

typedef struct tag_structX_unknown3
{
	BYTE mem11;				// 1 BYTE		errorResult
//	BYTE mem12;				// 1 BYTE		可能没用
//	WORD16 mem13;			// 1 WORD16		可能没用
	LIC_Data ld;			// 32 WORD16	#0x4
	BYTE *pbPMID;			// 2 WORD16		#0x44
	LENGTH_TYPE cbPMID;			// 2 WORD16		#0x48
	UWORD32 mem5;			// 2 WORD16		#0x4c lic中无信息位数54
	UWORD32 mem6;			// 2 WORD16		#0x50 lic counter
	UWORD32 mem7;			// 2 WORD16		#0x54
	UWORD32 mem8;			// 2 WORD16		#0x58
	UWORD32 mem9;			// 2 WORD16		#0x5c
} structX_unknown3;		// 48 WORD16

typedef struct tag_structX_unknown2
{
//	WORD16 mem1[34];		// 34 WORD16	可能没用
	structX_unknown3 mem2;	// 48 WORD16
} structX_unknown2;		// 82 WORD16

typedef struct tag_struct_DRM_state
{
	BYTE m_DecryptInited;		// 1 BYTE
//	BYTE mem12;					// 1 BYTE	可能没用
//	WORD16 mem13;				// 1 WORD16	可能没用
	CIPHER_CONTEXT mem2;		// 512 WORD16	#0x4
	structX_unknown2 mem3;		// 82 WORD16	#0x404
	BYTE* mem4;					// 2 WORD16		#0x4a8
} struct_DRM_state;

typedef struct tag_PMLICENSE
{
	UWORD32 mem1;		// 2 WORD16
	UWORD32 mem2;		// 2 WORD16
	LIC_Data ld;		// 32 WORD16
} PMLICENSE;		// 36 WORD16

//typedef struct tagLD
//{
//	BYTE KID[32];
//	BYTE member1[9];
//	BYTE rights[4];
//	BYTE appSec[4];
//	BYTE expiryDate[4];
//	BYTE member2[11];
//} LD;
//
//typedef struct tagPMLICENSE
//{
//	BYTE member1[8];
//	LD ld;
//} PMLICENSE;



/*
 *     FUNCTIONS
 */
void memcpyDWORD(UWORD32* pto, UWORD32* pfrom, LENGTH_TYPE len);
LENGTH_TYPE ZSP_strlen(BYTE *string);
UWORD16 ZSP_memcmp(BYTE* p1, BYTE* p2, LENGTH_TYPE len);
void ZSP_memcpy(BYTE* pto, BYTE* pfrom, LENGTH_TYPE len);

//// drmpd
DRM_RESULT  CDrmPD_Init__(void* pDRM_state);
DRM_RESULT  CDrmPD_InitDecrypt__( void* pDRM_state,
                            PMLICENSE* pmlic,
                            BYTE* pPMID,
                            LENGTH_TYPE cbPMID,
                            BYTE* pLicData,
                            UWORD32* pOffset,
                            UWORD32* pActual);

DRM_RESULT  CDrmPD_Decrypt__ (void* pDRM_state, BYTE* pbBuffer, LENGTH_TYPE cbBuffer);
DRM_RESULT  CDrmPD_InitPacket__(void* pDRM_state, BYTE* pLast15, UWORD16 cbPayloadSize);

//// pdlicstr
//void    SetSecret_1of7__(struct_DRM_state* pin);
//DRM_RESULT  SetSecret_2of7__(struct_DRM_state* pin);
//DRM_RESULT  SetSecret_3of7__(struct_DRM_state* pin);
//DRM_RESULT  SetSecret_4of7__(struct_DRM_state* pin);
//DRM_RESULT  SetSecret_5of7__(struct_DRM_state* pin);
//DRM_RESULT  SetSecret_6of7__(struct_DRM_state* pin);
//DRM_RESULT  SetSecret_7of7__(struct_DRM_state* pin);
UWORD32 hash__(BYTE* pin);

DRM_RESULT  CPDLicStore_Init__(structX_unknown3* pin);
//DRM_RESULT MatchLicense__(LIC_Data* pld,
//					BYTE* pLicData);
DRM_RESULT PMIDToPMKey__(struct_DRM_state* pDRMState,
//				   UWORD32 temp1,
				   UWORD16 temp,
				   SHA_CONTEXT* pContextSHA,
				   BYTE* pPMID,
				   LENGTH_TYPE cbPMID,
				   BYTE* pLicData,
				   BYTE* pbPMKey,
				   LENGTH_TYPE cbPMKey);
void FlipBits__(BYTE* pbIn, LENGTH_TYPE cbIn);
DRM_RESULT PMContentKeyToContentKey__(struct_DRM_state* pDRMState,
							    BYTE* pbPMKey,
							    LENGTH_TYPE cbKey,
							    BYTE* pbKey);

DRM_RESULT  CPDLicStore_Lookup__(
              				structX_unknown3* pin,
							struct_DRM_state* pDRMState,
							BYTE* pPMID,
							LENGTH_TYPE cbPMID,
							LIC_Data* pld,
							PMLICENSE* pmlic,
							BYTE* pLicData,
							UWORD32* pOffset,
							UWORD32* pActual,
							BYTE* pbKey);
//// dess
void  desSkey__(
    IN OUT DESTable      *pTable,
    IN OUT DRM_BYTE      *p,
    IN     DRM_UINT      c );
void desSkey_LS__(struct_DRM_state* pDRMState,
				DESTable* pdestable,
				BYTE* pbKey,
				LENGTH_TYPE cbKey);
void desS__(
    DRM_BYTE*      rgbOut,//8byte
    DRM_BYTE*      p,//可能为8byte
    DESTable*      pTable,
    DRM_INT        op );
//// xsum
DRM_RESULT checksum2__(struct_DRM_state* pDRMState,
			     BYTE* pbPMKey,
			     LENGTH_TYPE cbPMKey,
			     BYTE* pLicData,
			     LENGTH_TYPE licDataLen,	// 实际上固定为64
			     BYTE* pbCheckSum);

//// so1
//BYTE    SO1_GetB0__(void);
//BYTE    SO1_GetB1__(void);
//BYTE    SO1_GetB2__(void);
//BYTE    SO1_GetB3__(void);
//BYTE    SO1_GetB4__(void);

//// so2
//UWORD32 SO2_BiggerAction__(UWORD32* pin);
//UWORD32 SO2_Action__(UWORD32 in);

//// so3
//COUNT_TYPE  SO3_CheapRec__(COUNT_TYPE in);
//COUNT_TYPE  SO3_ErsatzRec__(COUNT_TYPE in);


//// so4
//BYTE    SO4_Random__(UWORD32* pin);
//UWORD16 SO4_scp_rand__(UWORD32* pin);

//// soalter
//void    SOAlter_Init__(BYTE* pin);
//void    SOAlter_Init2__(BYTE* pin, BYTE a1, BYTE a2);
//BYTE    SOAlter_Alter__(BYTE* pin, BYTE a1);

//// sobuf
//void    SOBuf_Init__(DRM_BYTE** ppin);
//DRM_RESULT  SOBuf_SetByte__(DRM_BYTE** ppin, COUNT_TYPE index, DRM_BYTE val);
//DRM_RESULT  SOBuf_GetByte__(DRM_BYTE** ppin, COUNT_TYPE index, DRM_BYTE* out);


///// blackbox
void    DRMInit__(CIPHER_CONTEXT* pin);
DRM_RESULT  DRMDecrypt__(CIPHER_CONTEXT* pin, LENGTH_TYPE cbBuffer, BYTE* pbBuffer);
DRM_RESULT  DRMInitPacket__(CIPHER_CONTEXT* pin, UWORD16 cbPayloadSize, BYTE* pLast15);
UWORD32 AllignedDWORD__(BYTE* pin);
DRM_RESULT DRMKeySetup__(CIPHER_CONTEXT* pContext, BYTE cbData, BYTE* pbData);

//// cbc64ws4
void CBC64WS4_asm__(UWORD32 *pbData,
				  LENGTH_TYPE cBlocks,
				  UWORD32 rgdwKeys[2],
				  DRM_CBCKey *pCBCKey);
//void egcd__(UWORD64 a, UWORD64 b, UWORD64* p1, UWORD64* p2, UWORD64* p3);
void CBC64Init__( DRM_CBCKey *cbckey, DRM_CBCState *cbcstate, UWORD32 *pKey );
void CBC64InvKey__( DRM_CBCKey *cbckey, DRM_CBCKey *cbcInvKey );
void   CBC64Update__(
     IN      DRM_CBCKey *key,
     IN  OUT DRM_CBCState *cbcstate,
     IN      LENGTH_TYPE cbData,
     IN      BYTE *pbData );

UWORD32 CBC64Finalize__( DRM_CBCKey *key, DRM_CBCState *cbcstate, UWORD32 *pKey2 );
UWORD32 inv32__( UWORD32 n );
UWORD32 CBC64Invert__( DRM_CBCKey *key, DRM_CBCKey *ikey,
                      UWORD32 MacA1,   UWORD32 MacA2,
                      UWORD32 MacB1,   UWORD32 MacB2,    UWORD32 *pInvKey2 );
void    CBC64InitState__( DRM_CBCState *cbcstate );

//// desport
void  des__(
          DRM_BYTE  rgbOut[__CB_DECL(DES_BLOCKLEN)],
    const DRM_BYTE  rgbIn [__CB_DECL(DES_BLOCKLEN)],
          DESTable *pTable,
          DRM_INT   op );

//// deskey
void deskey__(
    OUT       DESTable *pTable,
    IN  const DRM_BYTE  rgbKey[__CB_DECL(DES_KEYSIZE)] );

//// msrc4
void  rc4__(
    IN OUT RC4_KEYSTRUCT *pKS,
    IN     LENGTH_TYPE       cbBuffer,
    IN OUT BYTE      	 *pbBuffer );

void  rc4_key__(
        OUT   RC4_KEYSTRUCT  *pKS,
    IN        LENGTH_TYPE       cbKey,
    IN  const DRM_BYTE       *pbKey );

//// sha
void  A_SHAInit__(
    IN OUT SHA_CONTEXT* pShaContext );

void  A_SHAUpdate__(
    IN OUT   SHA_CONTEXT *pShaContext,
    IN const DRM_BYTE    *pbData,
    IN       DRM_DWORD    cbData );

void  SHA_UpdateOffset__(
    IN const DRM_BYTE    *pbData,
    IN       DRM_DWORD    ibData,
    IN       DRM_DWORD    cbData,
    IN OUT   SHA_CONTEXT *pShaContext );

static void SHATransform__(
    IN       DRM_DWORD ABCDE[5],
    IN const DRM_BYTE  bInput[__CB_DECL(SHA_BLOCK_SIZE)] );

static void _GetDRMDWORD(
    OUT      UWORD32 *dwTo,
    IN       LENGTH_TYPE  dwCount,
    IN const BYTE  *bIn );

void  _PackDRMDWORD(
    IN const DRM_DWORD *dwFrom,
    IN       LENGTH_TYPE  dwCount,
    OUT      DRM_BYTE  *bOut);

static WORD32 _ft(WORD32 b, WORD32 c, WORD32 d, COUNT_TYPE t);

static UWORD32 _K(COUNT_TYPE t);

void A_SHAFinal__(
    IN  SHA_CONTEXT *pShaContext,
    OUT DRM_BYTE     rgbDigest[__CB_DECL(SHA_DIGEST_LEN)] );




extern struct_DRM_state DRM_state;
#endif
#endif
