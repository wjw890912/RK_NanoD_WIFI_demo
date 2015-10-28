#ifndef APE_APETAG_H
#define APE_APETAG_H
#include "ape_io1.h"
#include "ape_all.h"

//class CIO;

/*****************************************************************************************
APETag version history / supported formats

1.0 (1000) - Original APE tag spec.  Fully supported by this code.
2.0 (2000) - Refined APE tag spec (better streaming support, UTF encoding). Fully supported by this code.

Notes:
    - also supports reading of ID3v1.1 tags
    - all saving done in the APE Tag format using CURRENT_APE_TAG_VERSION
*****************************************************************************************/

/*****************************************************************************************
APETag layout

1) Header - APE_TAG_FOOTER (optional) (32 bytes)
2) Fields (array):
        Value Size (4 bytes)
        Flags (4 bytes)
        Field Name (? ANSI bytes -- requires NULL terminator -- in range of 0x20 (space) to 0x7E (tilde))
        Value ([Value Size] bytes)
3) Footer - APE_TAG_FOOTER (32 bytes)
*****************************************************************************************/

/*****************************************************************************************
Notes

-When saving images, store the filename (no directory -- i.e. Cover.jpg) in UTF-8 followed
by a null terminator, followed by the image data.
*****************************************************************************************/

/*****************************************************************************************
The version of the APE tag
*****************************************************************************************/
#define CURRENT_APE_TAG_VERSION                 2000

/*****************************************************************************************
"Standard" APE tag fields
*****************************************************************************************/
#define APE_TAG_FIELD_TITLE                     L"Title"
#define APE_TAG_FIELD_ARTIST                    L"Artist"
#define APE_TAG_FIELD_ALBUM                     L"Album"
#define APE_TAG_FIELD_COMMENT                   L"Comment"
#define APE_TAG_FIELD_YEAR                      L"Year"
#define APE_TAG_FIELD_TRACK                     L"Track"
#define APE_TAG_FIELD_GENRE                     L"Genre"
#define APE_TAG_FIELD_COVER_ART_FRONT           L"Cover Art (front)"
#define APE_TAG_FIELD_NOTES                     L"Notes"
#define APE_TAG_FIELD_LYRICS                    L"Lyrics"
#define APE_TAG_FIELD_COPYRIGHT                 L"Copyright"
#define APE_TAG_FIELD_BUY_URL                   L"Buy URL"
#define APE_TAG_FIELD_ARTIST_URL                L"Artist URL"
#define APE_TAG_FIELD_PUBLISHER_URL             L"Publisher URL"
#define APE_TAG_FIELD_FILE_URL                  L"File URL"
#define APE_TAG_FIELD_COPYRIGHT_URL             L"Copyright URL"
#define APE_TAG_FIELD_MJ_METADATA               L"Media Jukebox Metadata"
#define APE_TAG_FIELD_TOOL_NAME                 L"Tool Name"
#define APE_TAG_FIELD_TOOL_VERSION              L"Tool Version"
#define APE_TAG_FIELD_PEAK_LEVEL                L"Peak Level"
#define APE_TAG_FIELD_REPLAY_GAIN_RADIO         L"Replay Gain (radio)"
#define APE_TAG_FIELD_REPLAY_GAIN_ALBUM         L"Replay Gain (album)"
#define APE_TAG_FIELD_COMPOSER                  L"Composer"
#define APE_TAG_FIELD_KEYWORDS                  L"Keywords"

/*****************************************************************************************
Standard APE tag field values
*****************************************************************************************/
#define APE_TAG_GENRE_UNDEFINED                 L"Undefined"

/*****************************************************************************************
ID3 v1.1 tag
*****************************************************************************************/
#define ID3_TAG_BYTES    128
struct ID3_TAG
{
    ape_char Header[3];             // should equal 'TAG'
    ape_char Title[30];             // title
    ape_char Artist[30];            // artist
    ape_char Album[30];             // album
    ape_char Year[4];               // year
    ape_char Comment[29];           // comment
    ape_uchar Track;        // track
    ape_uchar Genre;        // genre
};

/*****************************************************************************************
Footer (and header) flags
*****************************************************************************************/
#define APE_TAG_FLAG_CONTAINS_HEADER            0x80000000//((ape_int32)1 << 31)
#define APE_TAG_FLAG_CONTAINS_FOOTER            0x40000000//((ape_int32)1 << 30)
#define APE_TAG_FLAG_IS_HEADER                  0x20000000//((ape_int32)1 << 29)

#define APE_TAG_FLAGS_DEFAULT                   (APE_TAG_FLAG_CONTAINS_FOOTER)

/*****************************************************************************************
Tag field flags
*****************************************************************************************/
#define TAG_FIELD_FLAG_READ_ONLY                (1 << 0)

#define TAG_FIELD_FLAG_DATA_TYPE_MASK           (6)
#define TAG_FIELD_FLAG_DATA_TYPE_TEXT_UTF8      (0 << 1)
#define TAG_FIELD_FLAG_DATA_TYPE_BINARY         (1 << 1)
#define TAG_FIELD_FLAG_DATA_TYPE_EXTERNAL_INFO  (2 << 1)
#define TAG_FIELD_FLAG_DATA_TYPE_RESERVED       (3 << 1)

/*****************************************************************************************
The footer at the end of APE tagged files (can also optionally be at the front of the tag)
*****************************************************************************************/
#define APE_TAG_FOOTER_BYTES    32

typedef struct
{
  ape_char m_cID[8];              // should equal 'APETAGEX'
  //ape_char m_cID[4];              // should equal 'APETAGEX'
  ape_int32 Ape_gVersion;             // equals CURRENT_APE_TAG_VERSION
  ape_int32 m_nSize;                // the complete size of the tag, including this footer (excludes header)
  ape_int32 m_nFields;              // the number of fields in the tag
  ape_int32 m_nFlags;               // the tag flags
  ape_char m_cReserved[8];        // reserved for later use (must be zero)
  //ape_char m_cReserved[4];        // reserved for later use (must be zero)
  ape_BOOL (*GetIsValid)(void *,ape_BOOL);
  ape_int32 (*GetVersion)(void *) ;
  ape_BOOL (*GetIsHeader)(void *) ;
  ape_BOOL (*GetHasHeader)(void *);
  ape_int32 (*GetNumberFields)(void *) ;
  ape_int32 (*GetFieldsOffset)(void *) ;
  ape_int32 (*GetFieldBytes)(void *);
  ape_int32 (*GetTotalTagBytes)(void *) ;
  void (*cAPE_TAG_FOOTER)(void *,ape_int32, ape_int32);
}APE_TAG_FOOTER;
  ape_BOOL  ApeTagFooterGetIsValid(APE_TAG_FOOTER *,ape_BOOL);
  ape_int32 ApeTagFooterGetVersion(APE_TAG_FOOTER *) ;
  ape_BOOL  ApeTagFooterGetIsHeader(APE_TAG_FOOTER *) ;
  ape_BOOL  ApeTagFooterGetHasHeader(APE_TAG_FOOTER *);
  ape_int32 ApeTagFooterGetNumberFields(APE_TAG_FOOTER *) ;
  ape_int32 ApeTagFooterGetFieldsOffset(APE_TAG_FOOTER *) ;
  ape_int32 ApeTagFooterGetFieldBytes(APE_TAG_FOOTER *);
  ape_int32 ApeTagFooterGetTotalTagBytes(APE_TAG_FOOTER *) ;
  void      ApeTagFooter(APE_TAG_FOOTER *,ape_int32, ape_int32);


/*****************************************************************************************
CAPETagField class (an APE tag is an array of these)
*****************************************************************************************/

typedef struct
{
  void (*cCAPETagIO)(void*,CIO * pIO, ape_BOOL bAnalyze);
  void (*cCAPETagFile)(void*,ape_char * pFilename, ape_BOOL bAnalyze);
  void (*dCAPETag)(void*);

  ape_int32 (*GetTagBytes)(void*);
  ape_int32 (*CreateID3Tag)(void*,struct ID3_TAG * pID3Tag);

  ape_int32 (*Analyze)(void*);
  ape_int32 (*GetTagFieldIndex)(void*,ape_char * pFieldName);
  ape_int32 (*WriteBufferToEndOfIO)(void*,void * pBuffer, ape_int32 nBytes);
  ape_int32 (*LoadField)(void*,ape_char * pBuffer, ape_int32 nMaximumBytes, ape_int32 * pBytes);
  ape_int32 (*SortFields)(void*);
  ape_int32 (*CompareFields)( void * pA,  void * pB);
  ape_BOOL (*GetHasID3Tag)(void*);
  ape_BOOL (*GetHasAPETag)(void*);
  ape_int32 (*GetAPETagVersion)(void*);
  CIO *m_spIO;
  ape_BOOL m_bAnalyzed;
  ape_int32 m_nTagBytes;
  ape_int32 m_nFields;
  ape_char * m_aryFields[256];
  ape_BOOL m_bHasAPETag;
  ape_int32 m_nAPETagVersion;
  ape_BOOL m_bHasID3Tag;
  ape_BOOL m_bIgnoreReadOnly;
}CAPETag;

#ifdef __cplusplus
extern "C" {
#endif
  void      ApeTagIo(CAPETag*,CIO * pIO, ape_BOOL bAnalyze);
  void      ApeTagFileInitialize(CAPETag*,ape_uint16 * pFilename, ape_BOOL bAnalyze);
  void      CAPETag_dCAPETag(CAPETag*);
  ape_int32 ApeTagGetTagBytes(CAPETag*);
  ape_int32 ApeTagAnalyze(CAPETag*);
  ape_int32 ApeTagLoadField(CAPETag*,ape_char * pBuffer, ape_int32 nMaximumBytes, ape_int32 * pBytes);
  ape_BOOL  ApeTagGetHasId3Tag(CAPETag*);
  ape_BOOL  ApeTagGetHasApeTag(CAPETag*);
  ape_int32 ApeTagGetApeTagVersion(CAPETag*);

#ifdef __cplusplus
}
#endif

#endif // #ifndef APE_APETAG_H

