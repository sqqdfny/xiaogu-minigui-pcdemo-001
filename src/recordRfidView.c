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
#include "recordRfidView.h"

static mDialogBox* dlg =NULL;
static eRECORD_RFID_TYPE this_type;
static BITMAP fgBmp;
static BITMAP returnBtnBmp;
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
static BOOL OnReturnButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    SendMessage (self->hwnd, MSG_CLOSE, 0, 0);
    return FALSE;
}

static BOOL RegisterCtrlHanders (mWidget* self, DWORD addData)
{
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_RECORD_RFID_RETURN_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnReturnButtonClicked, NCSN_WIDGET_CLICKED);
	return TRUE;
}

//======================================================================================
//windos events
static BOOL OnCreate(mWidget* self, DWORD add_data)
{//TODO : initialize
    SetWindowFont(GetDlgItem(self->hwnd, IDC_RECORD_RFID_TIP_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_RECORD_RFID_STATE_STC), logfont_unifont_default);

    if (LoadBitmapFromFile(HDC_SCREEN, &fgBmp, IMAGE_FILE_PATH_RECORD_RFID_FG) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_RECORD_RFID_FG);
    }
    set_image_info(self, IDC_RECORD_RFID_FG_IMAGE, &fgBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &returnBtnBmp, IMAGE_FILE_PATH_RECORD_RFID_RETURN_BTN) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_RECORD_RFID_RETURN_BTN);
    }
    set_button_info(self, IDC_RECORD_RFID_RETURN_BTN, &returnBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    RegisterCtrlHanders(self, add_data);

    if(eRECORD_RFID_TYPE_ADD == this_type)
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_RECORD_RFID_TIP_STC), "新增用户");
    }
    else
    if(eRECORD_RFID_TYPE_MODIFY == this_type)
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_RECORD_RFID_TIP_STC), "修改用户");
    }
    else
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_RECORD_RFID_TIP_STC), "查询用户");
    }
    return NCSR_CONTINUE_MSG;
}


static void OnPaint (mWidget *self, HDC hdc, const CLIPRGN* inv)
{
    SetBkMode (hdc, BM_TRANSPARENT);
}

static BOOL OnClose(mWidget* self, int message)
{
    DestroyMainWindow(self->hwnd);
    MainWindowCleanup(self->hwnd);
    return NCSR_STOP_MSG;
}
static BOOL OnDestroy(mWidget* self, int message, WPARAM wParam, LPARAM lParam)
{
    DestroyAllControls(self->hwnd);

    UnloadBitmap(&fgBmp);
    UnloadBitmap(&returnBtnBmp);


    return NCSR_STOP_MSG;
}

static NCS_EVENT_HANDLER handlers[] =
{
    {MSG_CREATE,    OnCreate    },
    {MSG_PAINT,     OnPaint     },
    {MSG_CLOSE,     OnClose     },
    {MSG_DESTROY,   OnDestroy   },
    {0,             NULL        }
};

//======================================================================================
static NCS_RDR_INFO btn_rdr_num_info[] =
{
    {"flat", "flat", NULL},
};
static NCS_PROP_ENTRY static_props [] =
{
    { NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER },  //Align mode
    { NCSP_STATIC_VALIGN, NCS_VALIGN_CENTER },//Align mode
    {0, 0}
};
static NCS_RDR_ELEMENT stc_rdr_elements[] =
{
    {NCS_FGC_WINDOW , 0XFFFFFFFF},
    { -1, 0 }
};
static NCS_RDR_INFO static_rdr_info[] = {
    { "flat","flat", stc_rdr_elements }
};

static NCS_RDR_ELEMENT state_stc_rdr_elements[] =
{
    {NCS_FGC_WINDOW , 0XFFB2AA89},
    { -1, 0 }
};
static NCS_RDR_INFO state_static_rdr_info[] = {
    { "flat","flat", state_stc_rdr_elements }
};

// START_OF_TEMPLATE
static NCS_WND_TEMPLATE _ctrl_templ[] =
{
    {
        NCSCTRL_IMAGE,
        IDC_RECORD_RFID_FG_IMAGE,
        90, 350, 300, 110,
        WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_RECORD_RFID_RETURN_BTN,
        0, 0, 75, 75,
        WS_VISIBLE | NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_RECORD_RFID_TIP_STC,
        50, 0, 380, 75,
        WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "",
        static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_RECORD_RFID_STATE_STC,
        0, 500, 480, 50,
        WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "请将卡片放置到刷卡区",
        static_props,
        state_static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
};

static NCS_MNWND_TEMPLATE mywnd_templ = {
    NCSCTRL_DIALOGBOX,
    IDC_RECORD_RFID_DIALOGBOX,
    0, 0, 480, 960,
    WS_VISIBLE | WS_CHILD,
    WS_EX_AUTOSECONDARYDC,
    "",
    NULL,
    NULL,
    handlers,
    _ctrl_templ,
    sizeof(_ctrl_templ)/sizeof(NCS_WND_TEMPLATE),
    0,                      //user_data
    MakeRGBA(0x00, 0x00, 0x00, 0xFF),    //bk_color
    0,                     //font_name
    0,                      //hIcon
    0,                      //hMenu
    0,                      //notify_info
    0                       //special_id
};
// END_OF_TEMPLATE

mDialogBox* DialogRecordRfidViewCreate(HWND parent_hwnd, eRECORD_RFID_TYPE type)
{
    this_type = type;
    dlg = (mDialogBox *)ncsCreateMainWindowIndirect (&mywnd_templ, parent_hwnd);
    return dlg;
}
//==============================================================================================
//end files

