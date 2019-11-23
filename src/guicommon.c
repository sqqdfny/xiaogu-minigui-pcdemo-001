#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <mgncs/mgncs.h>
#include "guicommon.h"

//===============================================================================
LOGFONT* logfont_unifont_160_50 = NULL;
static DEVFONT* devfont_unifont_160_50 = NULL;
#define FONTNAME_UNIFONT_160_50                   "upf-unifont-rrncnn-16-16-UTF-8"


LOGFONT* logfont_unifont_SZSimHei_UTF_8_32_32 = NULL;
static DEVFONT* devfont_unifont_SZSimHei_UTF_8_32_32 = NULL;
#define FONTNAME_UNIFONT_SZSimHei_UTF_8_32_32     "upf-SZSimHei-rrncnn-32-32-UTF-8"

//===============================================================================
//Load global resources when minigui init.
void LoadIncoreData()
{
    devfont_unifont_SZSimHei_UTF_8_32_32 = LoadDevFontFromFile (FONTNAME_UNIFONT_SZSimHei_UTF_8_32_32, FONT_UPF_FILE_PATH_SZSimHei_32_32);
    if (NULL == devfont_unifont_SZSimHei_UTF_8_32_32)
    {
        _ERR_PRINTF("Failed to load incore UPF font.\n");
        exit (2);
    }
    logfont_unifont_SZSimHei_UTF_8_32_32 = CreateLogFontByName(FONTNAME_UNIFONT_SZSimHei_UTF_8_32_32);
    if (logfont_unifont_SZSimHei_UTF_8_32_32 == NULL)
    {
        _ERR_PRINTF("Failed to create logfont.\n");
        exit (2);
    }

    devfont_unifont_160_50 = LoadDevFontFromFile (FONTNAME_UNIFONT_160_50, FONT_UPF_FILE_PATH_160_50);
    if (NULL == devfont_unifont_160_50)
    {
       _ERR_PRINTF("Failed to load incore UPF font.\n");
       exit (2);
    }

    logfont_unifont_160_50 = CreateLogFontByName(FONTNAME_UNIFONT_160_50);
    if (logfont_unifont_160_50 == NULL)
    {
        _ERR_PRINTF("Failed to create logfont.\n");
        exit (2);
    }
    //g_SysLogFont[SYSLOGFONT_CONTROL] = logfont_unifont_160_50;
}

//Unload global resources when minigui deinit.
void UnloadIncoreData()
{
    DestroyLogFont(logfont_unifont_SZSimHei_UTF_8_32_32);
    DestroyDynamicDevFont(&devfont_unifont_SZSimHei_UTF_8_32_32);

    DestroyLogFont(logfont_unifont_160_50);
    DestroyDynamicDevFont(&devfont_unifont_160_50);
}
//===============================================================================

//===============================================================================
BOOL GuiVerifyPassword(char* str, eVERIFY_TYPE type)
{
    int ret;
    switch(type)
    {
        case eVERIFY_TYPE_OPEN_DOOR:
            ret = strcmp(str, "1234567") & strcmp(str, "123456");
            break;
        case eVERIFY_TYPE_CUSTOMER:
            ret = strcmp(str, "123456");
            break;
        default:
            ret = strcmp(str, "1234567");
    }

    if(0 == ret)
    {
        return TRUE ;
    }
    else
    {
        return FALSE;
    }
}

BOOL GuiVerifyFace(void * face, eVERIFY_TYPE type)
{
    return FALSE;
}

BOOL GuiVerifyRfid(void * rfid, eVERIFY_TYPE type)
{
    return FALSE;
}
//===============================================================================
//return 0 means no Admin
uint32_t GuiGetAdminCount()
{
    return 0;
}

//return 0 means no customer
uint32_t GuiGetCustomerCount()
{
    return 0;
}

uint32_t GuiApplyUserInfoId(void)
{
    return (0);
}

BOOL GuiIsUserInfoValid(uint32_t id)
{
    return FALSE;
}

//return FALSE means no valid info
BOOL GuiReadUserInfo(uint32_t id, sUSER_INFO * pUserInfoOut)
{
    return FALSE;
}

//===============================================================================
//end files

