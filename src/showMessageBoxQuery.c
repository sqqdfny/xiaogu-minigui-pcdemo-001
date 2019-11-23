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
#include "showMessageBoxQuery.h"

static mDialogBox* dlg;
HWND this_parent_hwnd;
static char icon_path[100] = "";
static char string[100] = "";
static BOOL this_result;

static BITMAP bgBmp;
static BITMAP iconBmp;
static uint8_t dly_cnt;
//======================================================================================
//common function
static void set_image_info(mWidget* self, int id, PBITMAP pbmp, int align_id, int align)
{
        mImage *img;
        img = (mImage *)ncsGetChildObj(self->hwnd, id);

        if(img)
        {
                _c(img)->setProperty(img, NCSP_IMAGE_IMAGE, (DWORD)pbmp);
                _c(img)->setProperty(img, align_id, align);
        }
}
static void set_button_info(mWidget* self, int id, PBITMAP pbmp, int align_id, int align)
{
        mButton *btn;
        btn = (mButton *)ncsGetChildObj(self->hwnd, id);

        if(btn){
                _c(btn)->setProperty(btn, NCSP_BUTTON_IMAGE, (DWORD)pbmp);
                _c(btn)->setProperty(btn, align_id, align);
        }

}

//======================================================================================
//controls events
static BOOL OnYesButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
	//GuiDbgPrintf("ShowMessageQuery OnYesButtonClicked\n");
    this_result = TRUE;
    SendMessage (self->hwnd, MSG_CLOSE, 0, 0);
    return FALSE;
}
static BOOL OnNoButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
	//GuiDbgPrintf("ShowMessageQuery OnNoButtonClicked\n");
    this_result = FALSE;
    SendMessage (self->hwnd, MSG_CLOSE, 0, 0);
    return FALSE;
}
static BOOL RegisterCtrlHanders (mWidget* self, DWORD addData)
{
	ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SHOW_MESSAGE_BOX_QUERY_YES_BTN)),
                        (mObject*)self, (NCS_CB_ONOBJEVENT)OnYesButtonClicked, NCSN_WIDGET_CLICKED);
	ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SHOW_MESSAGE_BOX_QUERY_NO_BTN)),
                        (mObject*)self, (NCS_CB_ONOBJEVENT)OnNoButtonClicked, NCSN_WIDGET_CLICKED);
	return TRUE;
}

//======================================================================================
//windos events
static BOOL OnCreate(mWidget* self, DWORD add_data)
{//TODO : initialize
    //GuiDbgPrintf("ShowMessageQuery OnCreate parent = %lu\n", (DWORD)GetParent(self->hwnd));

    SetWindowFont(GetDlgItem(self->hwnd, IDC_SHOW_MESSAGE_BOX_QUERY_STR_STC), logfont_unifont_default);
    SetWindowText (GetDlgItem(self->hwnd, IDC_SHOW_MESSAGE_BOX_QUERY_STR_STC), string);

    SetWindowFont(GetDlgItem(self->hwnd, IDC_SHOW_MESSAGE_BOX_QUERY_YES_BTN), logfont_unifont_default);
    SetWindowText (GetDlgItem(self->hwnd, IDC_SHOW_MESSAGE_BOX_QUERY_YES_BTN), "是");

    SetWindowFont(GetDlgItem(self->hwnd, IDC_SHOW_MESSAGE_BOX_QUERY_NO_BTN), logfont_unifont_default);
    SetWindowText (GetDlgItem(self->hwnd, IDC_SHOW_MESSAGE_BOX_QUERY_NO_BTN), "否(3)");

    if (LoadBitmapFromFile(HDC_SCREEN, &bgBmp, IMAGE_FILE_PATH_SHOW_MESSSAGE_BOX_QUERY_BG) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_SHOW_MESSSAGE_BOX_QUERY_BG);
    }
    set_image_info(self, IDC_SHOW_MESSAGE_BOX_QUERY_BG_IMAGE, &bgBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &iconBmp, icon_path) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", icon_path);
    }
    set_image_info(self, IDC_SHOW_MESSAGE_BOX_QUERY_ICON_IMAGE, &iconBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);


    RegisterCtrlHanders(self, add_data);

	dly_cnt = 3;
	SetTimer (self->hwnd, IDC_SHOW_MESSAGE_BOX_QUERY_DIALOGBOX, 100);

    return NCSR_CONTINUE_MSG;
}


static void OnPaint (mWidget *self, HDC hdc, const CLIPRGN* inv)
{
    SetBkMode (hdc, BM_TRANSPARENT);
}

static BOOL OnClose(mWidget* self, int message, WPARAM wpara, LPARAM lpara)
{
    KillTimer (self->hwnd, IDC_SHOW_MESSAGE_BOX_QUERY_DIALOGBOX);
    UnloadBitmap(&bgBmp);
    UnloadBitmap(&iconBmp);

    DestroyAllControls(self->hwnd);
	DestroyMainWindow(self->hwnd);
    MainWindowThreadCleanup(self->hwnd);
    return NCSR_STOP_MSG;
}

static void OnKillFocus(mWidget* self, int message)
{
    //GuiDbgPrintf("Show MessageBox OnKillFocus\n");
    this_result = FALSE;
    SendMessage (self->hwnd, MSG_CLOSE, 0, 0);
}
static void OnTimer (mWidget *self, HDC hdc, const CLIPRGN* inv)
{
    char str[30];

    if(0 == (--dly_cnt))
    {
        this_result = FALSE;
        SendMessage (self->hwnd, MSG_CLOSE, 0, 0);
    }
    else
    {
        if(dly_cnt > 3) {dly_cnt = 3;}
        sprintf(str, "否(%u)", dly_cnt);
        SetWindowText (GetDlgItem(self->hwnd, IDC_SHOW_MESSAGE_BOX_QUERY_NO_BTN), str);
    }
}

static NCS_EVENT_HANDLER handlers[] =
{
    {MSG_CREATE,    OnCreate    },
    {MSG_PAINT,     OnPaint     },
    {MSG_CLOSE,     OnClose     },
    {MSG_KILLFOCUS, OnKillFocus },
    {MSG_TIMER,     OnTimer     },
    {0,             NULL        }
};
//======================================================================================
static NCS_RDR_ELEMENT yes_btn_rdr_elements[] =
{
    {NCS_FGC_3DBODY , 0xFF000000},
    {NCS_BGC_3DBODY , 0xFF000000},
    {NCS_METRICS_3DBODY_ROUNDY, 18},
    {NCS_FGC_WINDOW , 0XFFFFFFFF},
	{ -1, 0 }
};

static NCS_RDR_INFO yes_btn_rdr_info[] = {
	{ "flat","flat", yes_btn_rdr_elements }
};

static NCS_RDR_ELEMENT no_btn_rdr_elements[] =
{
    {NCS_FGC_3DBODY , 0xFF0000FF},
    {NCS_BGC_3DBODY , 0xFF0000FF},
    {NCS_METRICS_3DBODY_ROUNDY, 18},
    {NCS_FGC_WINDOW , 0XFFFFFFFF},
	{ -1, 0 }
};

static NCS_RDR_INFO no_btn_rdr_info[] = {
	{ "flat","flat", no_btn_rdr_elements }
};

static NCS_RDR_ELEMENT stc_rdr_elements[] =
{
	{NCS_FGC_WINDOW , 0XFF0000FF},
	{ -1, 0 }
};

static NCS_RDR_INFO static_rdr_info[] = {
	{ "flat","flat", stc_rdr_elements }
};
static NCS_PROP_ENTRY static_props [] =
{
    { NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER },
    { NCSP_STATIC_VALIGN, NCS_VALIGN_CENTER },
    { NCSP_STATIC_AUTOWRAP, 1 },
    {0, 0} };

// START_OF_TEMPLATE
static NCS_WND_TEMPLATE _ctrl_templ[] =
{
    {
        NCSCTRL_IMAGE,
        IDC_SHOW_MESSAGE_BOX_QUERY_BG_IMAGE,
        0, 0, 400, 300,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_IMAGE,
        IDC_SHOW_MESSAGE_BOX_QUERY_ICON_IMAGE,
        150, 10, 100, 100,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_SHOW_MESSAGE_BOX_QUERY_STR_STC,
        0, 110, 400, 130,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "",
        static_props,
        static_rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SHOW_MESSAGE_BOX_QUERY_YES_BTN,
        30, 240, 150, 40,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "",
        NULL,
        yes_btn_rdr_info,
        NULL,NULL,0,0, MakeRGBA(0xFF, 0xFF, 0xFF, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SHOW_MESSAGE_BOX_QUERY_NO_BTN,
        220, 240, 150, 40,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "",
        NULL,
        no_btn_rdr_info,
        NULL,NULL,0,0, MakeRGBA(0xFF, 0xFF, 0xFF, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
};

static NCS_MNWND_TEMPLATE mywnd_templ = {
    NCSCTRL_DIALOGBOX,
    IDC_SHOW_MESSAGE_BOX_QUERY_DIALOGBOX,
    40, 300, 400, 300,
    WS_VISIBLE | WS_CHILD,
    WS_EX_AUTOSECONDARYDC | WS_EX_TRANSPARENT,
    "",
    NULL,
    NULL,
    handlers,
    _ctrl_templ,
    sizeof(_ctrl_templ)/sizeof(NCS_WND_TEMPLATE),
    0,                      //user_data
    MakeRGBA(0x00, 0x00, 0x00, 0xFF),    //bk_color
    0,                      //font_name
    0,                      //hIcon
    0,                      //hMenu
    0,                      //notify_info
    0                       //special_id
};
// END_OF_TEMPLATE

BOOL ShowMessageBoxQuery(HWND parent_hwnd, char * icon, char * str)
{
    this_result = FALSE;
    int len = strlen(icon);
    //GuiDbgPrintf("icon = %d\n", len);
    if(len > 99) return FALSE;

    len = strlen(str);
    //GuiDbgPrintf("str = %d\n", len);
    if(len > 99) return FALSE;

    strcpy(icon_path, icon);
    strcpy(string, str);

    //GuiDbgPrintf("ShowMessageBoxQuery parent = %lu\n", (DWORD)parent_hwnd);

    this_parent_hwnd = parent_hwnd;

    //dlg = (mDialogBox *)ncsCreateWindowIndirect (&mywnd_templ, parent_hwnd);
    dlg = (mDialogBox *)ncsCreateMainWindowIndirect (&mywnd_templ, parent_hwnd);
    _c(dlg)->doModal(dlg, TRUE);
    return this_result;
}
//==============================================================================================
//end files

