/**
  ******************************************************************************
  * @file    httpd_cg_ssi.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   Webserver SSI and CGI handlers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "FreeRTOS.h"
#include "RKOS.h"
#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "main.h"

#include <string.h>
#include <stdlib.h>
#ifdef __WICE_HTTP_SERVER__
//#pragma arm section code = "lwipcode", rodata = "lwipcode", rwdata = "lwipdata", zidata = "lwipdata"

/* we will use character "t" as tag for CGI */
char const* TAGCHAR="t";
char const** TAGS=&TAGCHAR;

/* CGI handler for LED control */
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/* Html request for "/leds.cgi" will start LEDS_CGI_Handler */
const tCGI LEDS_CGI={"/leds.cgi", LEDS_CGI_Handler};

/* Cgi call table, only one CGI used */
tCGI CGI_TAB[1];

static u16_t
SSIHandler(int iIndex, char *pcInsert, int iInsertLen)
{
    uint8 num_bsscfg;
    int num_menu = 0;
    int num_index;
    int ret = 0;
    return ret;
}

/**
  * @brief  CGI handler for LEDs control
  */
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{


#if 1
  uint32 i=0;

  /* We have only one SSI handler iIndex = 0 */
  if (iIndex==0)
  {
    /* All leds off */
    //STM_EVAL_LEDOff(LED1);

    /* Check cgi parameter : example GET /leds.cgi?led=2&led=4 */
    printf("led all off\n");
    for (i=0; i<iNumParams; i++)
    {
      /* check parameter "led" */
      if (strcmp(pcParam[i] , "led")==0)
      {
        /* switch led1 ON if 1 */
        if(strcmp(pcValue[i], "1") ==0)
        {
          //STM_EVAL_LEDOn(LED1);
            printf("led 1 on\n");
        }
        /* switch led2 ON if 2 */
        else if(strcmp(pcValue[i], "2") ==0)
        {
          //STM_EVAL_LEDOn(LED1);
            printf("led 2 on\n");
        }
        /* switch led3 ON if 3 */
        else if(strcmp(pcValue[i], "3") ==0)
        {
          //STM_EVAL_LEDOn(LED1);
            printf("led 3 on\n");
        }
        /* switch led4 ON if 4 */
        else if(strcmp(pcValue[i], "4") ==0)
        {
          //STM_EVAL_LEDOn(LED1);
            printf("led 4 on\n");
        }
      }
    }
  }
  /* uri to send after cgi call*/
  return "/STM32F4x7LED.html";
#endif
return "/index.html";
}

/**
 * Initialize SSI handlers
 */
void httpd_ssi_init(void)
{
  /* configure SSI handlers (ADC page SSI) */
  http_set_ssi_handler(SSIHandler, (char const **)TAGS, 1);
}

/**
 * Initialize CGI handlers
 */
void httpd_cgi_init(void)
{
  /* configure CGI handlers (LEDs control CGI) */
  CGI_TAB[0] = LEDS_CGI;
  http_set_cgi_handlers(CGI_TAB, 1);
}
//#pragma arm section code
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
