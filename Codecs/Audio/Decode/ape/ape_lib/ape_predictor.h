#ifndef APE_PREDICTOR_H
#define APE_PREDICTOR_H

/*************************************************************************************************
IPredictorCompress - the interface for compressing (predicting) data
*************************************************************************************************/
/*Mod by Wei.Hisung 2007.03.05*/


/*************************************************************************************************
IPredictorDecompress - the interface for decompressing (un-predicting) data
*************************************************************************************************/

typedef struct IPredictorDecompress_t
{
  ape_int32 (*cIPredictorDecompress)(void * ,ape_int32 , ape_int32 ,ape_int16 );
  //void (*dIPredictorDecompress)(void*);
  ape_int32 (*DecompressValue)(void*,ape_int32 nA, ape_int32 nB) ;
  ape_int32 (*Flush)(void*);
}IPredictorDecompress;

#endif // #ifndef APE_PREDICTOR_H
