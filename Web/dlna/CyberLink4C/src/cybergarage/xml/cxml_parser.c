/******************************************************************
*
*    CyberXML for C
*
*    Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*    File: cxml_parser.c
*
*    Revision:
*
*    02/07/05
*        - first revision
*
******************************************************************/
#include "source_macro.h"
#ifdef __WIFI_UPNP_C__
#include <cybergarage/xml/cxml.h>
#include <cybergarage/http/chttp.h>
#include <cybergarage/util/clog.h>
#include <rk_heap.h>
#include "dlna.h"

//#pragma arm section code = "upnpCode", rodata = "upnpCode", rwdata = "upnpdata", zidata = "upnpdata"
extern unsigned *namingBitmap;
extern unsigned char *nmstrtPages;
extern unsigned char *namePages;

/****************************************
* cg_xml_parser_new
****************************************/

CgXmlParser *cg_xml_parser_new()
{
    CgXmlParser *parser;
    uint32 len = 0;

    cg_log_debug_l4("Entering...\n");

    if( Cg_Get_Xml(XML_DMR_NAMINGBITMAP_DATA, (char**)&(namingBitmap), &len) == -1)
    {
        rk_printf("dmr_namingbitmap_data get fail\n");
        return NULL;
    }

    if(Cg_Get_Xml(XML_DMR_NMSTRTPAGES_DATA, &nmstrtPages, &len) == -1)
    {
        free(namingBitmap);
        rk_printf("dmr_nmstrtpages_data get fail\n");
        return NULL;
    }

    if(Cg_Get_Xml(XML_DMR_NAMEPAGES_DATA, &namePages, &len) == -1)
    {
        free(nmstrtPages);
        free(namingBitmap);
        rk_printf("dmr_namepages_data get fail\n");
        return NULL;
    }

    parser = (CgXmlParser *)malloc(sizeof(CgXmlParser));

    if ( NULL != parser )
    {
        parser->parseResult = FALSE;
    }
    return parser;

    cg_log_debug_l4("Leaving...\n");
}

/****************************************
* cg_xml_parser_delete
****************************************/

void cg_xml_parser_delete(CgXmlParser *parser)
{
    cg_log_debug_l4("Entering...\n");

    free(parser);

    if(namePages != NULL)
        free(namePages);
    namePages = NULL;

    if(nmstrtPages != NULL)
        free(nmstrtPages);
    nmstrtPages = NULL;

    if(namingBitmap != NULL)
        free(namingBitmap);
    namingBitmap = NULL;

    cg_log_debug_l4("Leaving...\n");
}

//#pragma arm section code
#endif
