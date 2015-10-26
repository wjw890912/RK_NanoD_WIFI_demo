#ifndef APE_ANTIPREDICTOR_H
#define APE_ANTIPREDICTOR_H

#ifdef __cplusplus
	extern "C" {
#endif
typedef struct
{
	void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 );
}CAntiPredictor;

//CAntiPredictor * CreateAntiPredictor(ape_int32 nCompressionLevel, ape_int32 nVersion);

typedef struct //stCAntiPredictorOffset
{
	void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 , ape_int32 , ape_int32 );
}CAntiPredictorOffset;

//void CAntiPredictorOffset_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements, ape_int32 Offset, ape_int32 DeltaM);
#ifdef __cplusplus
	}
#endif

#ifdef ENABLE_COMPRESSION_MODE_FAST
#ifdef __cplusplus
	extern "C" {
#endif
typedef struct{
    void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 );
}CAntiPredictorFast0000To3320;
//void CAntiPredictorFast0000To3320_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements);


typedef struct
{
    void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 );
}CAntiPredictorFast3320ToCurrent;
//void CAntiPredictorFast3320ToCurrent_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements);
#ifdef __cplusplus
	}
#endif
#endif // #ifdef ENABLE_COMPRESSION_MODE_FAST

#ifdef ENABLE_COMPRESSION_MODE_NORMAL


#ifdef __cplusplus
	extern "C" {
#endif
/*****************************************************************************************
Normal anti-predictor
*****************************************************************************************/
typedef struct
{
   void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 );
}CAntiPredictorNormal0000To3320;
//void CAntiPredictorNormal0000To3320_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements);


typedef struct
{
	void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 );
}CAntiPredictorNormal3320To3800;
//void CAntiPredictorNormal3320To3800_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements);

typedef struct
{
	void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 );
}CAntiPredictorNormal3800ToCurrent;
//void CAntiPredictorNormal3800ToCurrent_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements);


#endif // #ifdef ENABLE_COMPRESSION_MODE_NORMAL

#ifdef ENABLE_COMPRESSION_MODE_HIGH

/*****************************************************************************************
High anti-predictor
*****************************************************************************************/
typedef struct
{
    void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 );
}CAntiPredictorHigh0000To3320;
//void CAntiPredictorHigh0000To3320_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements);

typedef struct
{
    void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 );
}CAntiPredictorHigh3320To3600;
//void CAntiPredictorHigh3320To3600_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements);


typedef struct
{
	void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 );
}CAntiPredictorHigh3600To3700;
//void CAntiPredictorHigh3600To3700_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements);

typedef struct
{
	void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 );
}CAntiPredictorHigh3700To3800;
//void CAntiPredictorHigh3700To3800_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements);

typedef struct
{
    void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 );
}CAntiPredictorHigh3800ToCurrent;
//void CAntiPredictorHigh3800ToCurrent_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements);

#ifdef __cplusplus
	}
#endif

#endif // #ifdef ENABLE_COMPRESSION_MODE_HIGH

#ifdef ENABLE_COMPRESSION_MODE_EXTRA_HIGH
#ifdef __cplusplus
	extern "C" {
#endif
/*****************************************************************************************
Extra high helper
*****************************************************************************************/
typedef struct
{
    ape_int32 (*ConventionalDotProduct)(ape_int16 *, ape_int16 *, ape_int16 *, ape_int32 , ape_int32 );
}CAntiPredictorExtraHighHelper;
//ape_int32 CAntiPredictorExtraHighHelper_ConventionalDotProduct(ape_int16 *bip, ape_int16 *bbm, ape_int16 *pIPAdaptFactor, ape_int32 op, ape_int32 nNumberOfIterations);


typedef struct
{
	void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 , ape_int32 , ape_uint32 *, ape_uint32 *);
	void (*AntiPredictorOffset)(ape_int32* , ape_int32* , ape_int32 , ape_int32 , ape_int32 , ape_int32 );
}CAntiPredictorExtraHigh0000To3320;
//void CAntiPredictorExtraHigh0000To3320_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements, ape_int32 Iterations, ape_uint32 *pOffsetValueArrayA, ape_uint32 *pOffsetValueArrayB);
//void CAntiPredictorExtraHigh0000To3320_AntiPredictorOffset(ape_int32* Input_Array, ape_int32* Output_Array, ape_int32 Number_of_Elements, ape_int32 g, ape_int32 dm, ape_int32 Max_Order);

typedef struct
{
    void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 , ape_int32 , ape_uint32 *, ape_uint32 *);
    void (*AntiPredictorOffset)(ape_int32* , ape_int32* , ape_int32 , ape_int32 , ape_int32 , ape_int32 );
}CAntiPredictorExtraHigh3320To3600;
//void CAntiPredictorExtraHigh3320To3600_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements, ape_int32 Iterations, ape_uint32 *pOffsetValueArrayA, ape_uint32 *pOffsetValueArrayB);
//void CAntiPredictorExtraHigh3320To3600_AntiPredictorOffset(ape_int32* Input_Array, ape_int32* Output_Array, ape_int32 Number_of_Elements, ape_int32 g, ape_int32 dm, ape_int32 Max_Order);

typedef struct  {
    void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 , ape_int32 , ape_uint32 *, ape_uint32 *);
    void (*AntiPredictorOffset)(ape_int32* , ape_int32* , ape_int32 , ape_int32 , ape_int32 , ape_int32 );
}CAntiPredictorExtraHigh3600To3700;
//void CAntiPredictorExtraHigh3600To3700_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements, ape_int32 Iterations, ape_uint32 *pOffsetValueArrayA, ape_uint32 *pOffsetValueArrayB);
//void CAntiPredictorExtraHigh3600To3700_AntiPredictorOffset(ape_int32* Input_Array, ape_int32* Output_Array, ape_int32 Number_of_Elements, ape_int32 g1, ape_int32 g2, ape_int32 Max_Order);

typedef struct {
	void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 , ape_int32 , ape_uint32 *, ape_uint32 *);
    void (*AntiPredictorOffset)(ape_int32* , ape_int32* , ape_int32 , ape_int32 , ape_int32 , ape_int32 );
}CAntiPredictorExtraHigh3700To3800;
//void CAntiPredictorExtraHigh3700To3800_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements, ape_int32 Iterations, ape_uint32 *pOffsetValueArrayA, ape_uint32 *pOffsetValueArrayB);
//void CAntiPredictorExtraHigh3700To3800_AntiPredictorOffset(ape_int32* Input_Array, ape_int32* Output_Array, ape_int32 Number_of_Elements, ape_int32 g1, ape_int32 g2, ape_int32 Max_Order);

typedef struct {
	void (*AntiPredict)(ape_int32 *, ape_int32 *, ape_int32 , ape_BOOL, ape_int32 , ape_int32 );
}CAntiPredictorExtraHigh3800ToCurrent;
//void CAntiPredictorExtraHigh3800ToCurrent_AntiPredict(ape_int32 *pInputArray, ape_int32 *pOutputArray, ape_int32 NumberOfElements, ape_BOOL bMMXAvailable, ape_int32 CPULoadBalancingFactor, ape_int32 nVersion);

#ifdef __cplusplus
	}
#endif

#endif // #ifdef ENABLE_COMPRESSION_MODE_EXTRA_HIGH

#endif // #ifndef APE_ANTIPREDICTOR_H
