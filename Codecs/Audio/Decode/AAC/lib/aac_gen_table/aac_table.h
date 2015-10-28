
#ifdef NANOC_DECODE
#define base ((unsigned char *)0x02000000)
#else
extern const unsigned char AAC_TABLE_ROM[];
#define base ((unsigned char *)AAC_TABLE_ROM)
#endif
#define aac_table_test
#define cos4sin4tabOffset_offset  0
#define cos4sin4tab_offset  8
#define cos1sin1tab_offset  4616
#define sinWindowOffset_offset  6672
#define sinWindow_offset  6680
#define kbdWindowOffset_offset  11288
#define kbdWindow_offset  11296
#define bitrevtabOffset_offset  15904
#define bitrevtab_offset	15912
#define uniqueIDTab_offset  16060
#define twidTabOdd_offset  16068
#define twidTabEven_offset  20100
#define invQuant3_offset  22116
#define invQuant4_offset  22180
#define pow14_stproc_offset  22244
#define pow14_pns_offset  22276
#define gainBits_noiseless_offset  22292
#define huffTabSpec_offset  22304
#define huffTabScaleFact_offset  24788
#define huffTabSpecInfo_offset  25032
#define huffTabScaleFactInfo_offset  25340
#define nfftTab_fft_offset	25368
#define nfftlog2Tab_fft_offset  25376
#define pow14_deq_offset  25384
#define pow43_14_deq_offset  25400
#define pow43_deq_offset  25656
#define poly43lo_deq_offset  25848
#define poly43hi_deq_offset  25868
#define pow2exp_deq_offset  25888
#define pow2frac_deq_offset  25920
#define nmdctTab_dct_offset  25952
#define postSkip_dct_offset  25960
#define sampRateTab_offset  25968
#define predSFBMax_offset  26016
#define channelMapTab_offset  26064
#define elementNumChans_offset  26096
#define sfBandTotalShort_offset  26128
#define sfBandTotalLong_offset  26140
#define sfBandTabShortOffset_offset  26152
#define sfBandTabShort_offset  26200
#define sfBandTabLongOffset_offset  26352
#define sfBandTabLong_offset  26400
#define tnsMaxBandsShortOffset_offset  27052
#define tnsMaxBandsShort_offset  27064
#define tnsMaxOrderShort_offset  27088
#define tnsMaxBandsLongOffset_offset  27092
#define tnsMaxBandsLong_offset  27104
#define tnsMaxOrderLong_offset  27128
#define _cos1sin1tab64_aacdec_offset  27132
#define _cos4sin4tab64_aacdec_offset  27268
#define _twidTabOdd32_aacdec_offset  27524
