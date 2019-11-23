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
#include "recordPasswordView.h"
#include "showMessageBox.h"
#include "softKeypadView.h"


static mDialogBox* dlg =NULL;
static mDialogBox* soft_keypad_dlg;
static eRECORD_PASSWORD_TYPE this_type;

static BITMAP tipBgBmp;
static BITMAP bgBmp;
static BITMAP sleditBgBmp;
static BITMAP returnBtnBmp;
static BITMAP enterBmp;
static BITMAP stateBmp;
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

static BOOL OnEditSetFocusClicked (mMainWnd *self, mSlEdit *sender, int id, DWORD param)
{
    GuiDbgPrintf("RecordPassword OnEditSetFocusClicked\n");
    if(!IsSoftKeypadRun())
    {
        DialogSoftKeypadViewCreate(self->hwnd, SOFT_KEYPAD_FUN_NUM, 0, 420);
    }
    return FALSE;
}
static BOOL OnEditKillFocusClicked (mMainWnd *self, mSlEdit *sender, int id, DWORD param)
{
    HWND hwnd = GetFocusChild(self->hwnd);

    GuiDbgPrintf("RecordPassword OnEditKillFocusClicked\n");

    if((hwnd !=  GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_PASSWORD_EDIT_1)) \
    && (hwnd !=  GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_PASSWORD_EDIT_2)) )
    {
        GuiDbgPrintf("RecordPassword CloseSoftKeypad\n");
        CloseSoftKeypad();
    }

    return FALSE;
}



static BOOL OnEditChangeClicked (mMainWnd *self, mSlEdit *sender, int id, DWORD param)
{
    int len[2];
    char str[2][50 + 1] = {"", ""};
    len[0] = GetDlgItemText(self->hwnd, IDC_RECORD_PASSWORD_PASSWORD_EDIT_1, str[0], 50);
    len[1] = GetDlgItemText(self->hwnd, IDC_RECORD_PASSWORD_PASSWORD_EDIT_2, str[1], 50);
    GuiDbgPrintf("RecordPassword OnEditChangeClicked len[0] = %d str[0] = %s\n",len[0], str[0]);
    GuiDbgPrintf("RecordPassword OnEditChangeClicked len[1] = %d str[1] = %s\n",len[1], str[1]);

    if(strcmp(str[0], str[1]))
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_STATE_STC), "两次输入不一致");
        ShowWindow(GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_STATE_IMAGE), SW_SHOW);
        ShowWindow(GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_STATE_STC), SW_SHOW);
    }
    else
    if(len[0] < 6)
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_STATE_STC), "密码最少为6位");
        ShowWindow(GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_STATE_IMAGE), SW_SHOW);
        ShowWindow(GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_STATE_STC), SW_SHOW);
    }
    else
    {
        ShowWindow(GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_STATE_IMAGE), SW_HIDE);
        ShowWindow(GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_STATE_STC), SW_HIDE);
    }
    return FALSE;
}

static BOOL OnEnterButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    GuiDbgPrintf("RecordPassword OnEnterButtonClicked\n");
    //SendMessage (self->hwnd, MSG_CLOSE, 0, 0);
    return FALSE;
}

static BOOL RegisterCtrlHanders (mWidget* self, DWORD addData)
{
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_RECORD_PASSWORD_RETURN_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnReturnButtonClicked, NCSN_WIDGET_CLICKED);

    ncsAddEventListener ((mObject*)((mSlEdit*)_c(self)->getChild (self, IDC_RECORD_PASSWORD_PASSWORD_EDIT_1)),
                                (mObject*)self, (NCS_CB_ONOBJEVENT)OnEditSetFocusClicked, NCSN_EDIT_SETFOCUS);
    ncsAddEventListener ((mObject*)((mSlEdit*)_c(self)->getChild (self, IDC_RECORD_PASSWORD_PASSWORD_EDIT_2)),
                                (mObject*)self, (NCS_CB_ONOBJEVENT)OnEditSetFocusClicked, NCSN_EDIT_SETFOCUS);

    ncsAddEventListener ((mObject*)((mSlEdit*)_c(self)->getChild (self, IDC_RECORD_PASSWORD_PASSWORD_EDIT_1)),
                                (mObject*)self, (NCS_CB_ONOBJEVENT)OnEditKillFocusClicked, NCSN_EDIT_KILLFOCUS);
    ncsAddEventListener ((mObject*)((mSlEdit*)_c(self)->getChild (self, IDC_RECORD_PASSWORD_PASSWORD_EDIT_2)),
                                (mObject*)self, (NCS_CB_ONOBJEVENT)OnEditKillFocusClicked, NCSN_EDIT_KILLFOCUS);

    ncsAddEventListener ((mObject*)((mSlEdit*)_c(self)->getChild (self, IDC_RECORD_PASSWORD_PASSWORD_EDIT_1)),
                                (mObject*)self, (NCS_CB_ONOBJEVENT)OnEditChangeClicked, NCSN_EDIT_CHANGE);
    ncsAddEventListener ((mObject*)((mSlEdit*)_c(self)->getChild (self, IDC_RECORD_PASSWORD_PASSWORD_EDIT_2)),
                                (mObject*)self, (NCS_CB_ONOBJEVENT)OnEditChangeClicked, NCSN_EDIT_CHANGE);

    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_RECORD_PASSWORD_ENTER_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnEnterButtonClicked, NCSN_WIDGET_CLICKED);

	return TRUE;
}

//======================================================================================
//windos events
static BOOL OnCreate(mWidget* self, DWORD add_data)
{//TODO : initialize
    SetWindowFont(GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_TIP_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_SLEDIT_HEAD_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_STATE_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_PASSWORD_EDIT_1), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_PASSWORD_EDIT_2), logfont_unifont_default);

    if (LoadBitmapFromFile(HDC_SCREEN, &tipBgBmp, IMAGE_FILE_PATH_RECORD_PASSWORD_TIP_BG) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_RECORD_PASSWORD_TIP_BG);
    }
    set_image_info(self, IDC_RECORD_PASSWORD_TIP_BG_IMAGE, &tipBgBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &bgBmp, IMAGE_FILE_PATH_RECORD_PASSWORD_BG) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_RECORD_PASSWORD_BG);
    }
    set_image_info(self, IDC_RECORD_PASSWORD_BG_IMAGE, &bgBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &sleditBgBmp, IMAGE_FILE_PATH_RECORD_PASSWORD_SLEDIT_BG) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_RECORD_PASSWORD_SLEDIT_BG);
    }
    set_image_info(self, IDC_RECORD_PASSWORD_SLEDIT_BG_IMAGE, &sleditBgBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &stateBmp, ICON_FILE_PATH_ICON_WARNING_SMALL) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", ICON_FILE_PATH_ICON_WARNING_SMALL);
    }
    set_image_info(self, IDC_RECORD_PASSWORD_STATE_IMAGE, &stateBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);


    if (LoadBitmapFromFile(HDC_SCREEN, &returnBtnBmp, IMAGE_FILE_PATH_RECORD_PASSWORD_RETURN_BTN) != 0)
    {
        printf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_RECORD_PASSWORD_RETURN_BTN);
    }
    set_button_info(self, IDC_RECORD_PASSWORD_RETURN_BTN, &returnBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &enterBmp, (HDC_SCREEN, &returnBtnBmp, IMAGE_FILE_PATH_RECORD_PASSWORD_ENTER_BTN)) != 0)
    {
        printf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_RECORD_PASSWORD_ENTER_BTN);
    }
    set_button_info(self, IDC_RECORD_PASSWORD_ENTER_BTN, &enterBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    RegisterCtrlHanders(self, add_data);

    if(eRECORD_PASSWORD_TYPE_ADD == this_type)
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_TIP_STC), "新增用户");
    }
    else
    if(eRECORD_PASSWORD_TYPE_MODIFY == this_type)
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_TIP_STC), "修改用户密码");
    }
    else
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_RECORD_PASSWORD_TIP_STC), "查询用户密码");
    }

    return NCSR_CONTINUE_MSG;
}


static void OnPaint (mWidget *self, HDC hdc, const CLIPRGN* inv)
{
    //GuiDbgPrintf("num keypad window OnPaint\n");
    SetBkMode (hdc, BM_TRANSPARENT);
}

static BOOL OnClose(mWidget* self, int message)
{
    DestroyMainWindow(self->hwnd);
    MainWindowCleanup(self->hwnd);
    return NCSR_STOP_MSG;
}

static BOOL OnCommand(mWidget* self, int id, int nc, HWND hCtrl)
{
    //GuiDbgPrintf("recordPassword OnCommand id=%d, nc=%d\n", id, nc);
    return NCSR_CONTINUE_MSG;
}
static BOOL OnDestroy(mWidget* self, int message)
{
    DestroyAllControls(self->hwnd);
    UnloadBitmap(&tipBgBmp);
    UnloadBitmap(&bgBmp);
    UnloadBitmap(&sleditBgBmp);
    UnloadBitmap(&returnBtnBmp);
    UnloadBitmap(&enterBmp);
    return NCSR_STOP_MSG;
}

static NCS_EVENT_HANDLER handlers[] =
{
    {MSG_CREATE,            OnCreate    },
    {MSG_PAINT,             OnPaint     },
    {MSG_CLOSE,             OnClose     },
    {MSG_COMMAND,           OnCommand   },
    {MSG_DESTROY,           OnDestroy   },
    {0,                     NULL        }
};
//=============================================================================================
//ctrls enents
static BOOL OnCharSledit(mWidget* self, int message, int scancode, DWORD key_status)
{
    GuiDbgPrintf("recordPassword OnCharSledit message=%d, scancode=%c state=%lu\n", message, scancode, key_status);
    if((scancode >= '0') && (scancode <= '9'))
    {
        return NCSR_CONTINUE_MSG;
    }
    return NCSR_STOP_MSG;
}

static NCS_EVENT_HANDLER sledit_handlers[] =
{
    {MSG_CHAR,           OnCharSledit},
    {0,                  NULL        }
};

//======================================================================================
static NCS_RDR_INFO btn_rdr_info[] =
{
    {"flat", "flat", NULL},
};
static NCS_RDR_ELEMENT ok_btn_rdr_elements[] =
{
    {NCS_FGC_3DBODY , 0xFFFF0000},
    {NCS_BGC_3DBODY , 0xFFFF0000},
    {NCS_METRICS_3DBODY_ROUNDY, 24},
    {NCS_FGC_WINDOW , 0XFFFFFFFF},
    { -1, 0 }
};
static NCS_RDR_INFO ok_btn_rdr_info[] = {
    { "flat","flat", ok_btn_rdr_elements }
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
    {NCS_FGC_WINDOW , 0XF0000FF},
    { -1, 0 }
};
static NCS_RDR_INFO state_static_rdr_info[] = {
    { "flat","flat", state_stc_rdr_elements }
};

static NCS_PROP_ENTRY static_props [] =
{
    { NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER },  //Align mode
    { NCSP_STATIC_VALIGN, NCS_VALIGN_CENTER },//Align mode
    {0, 0}
};
static NCS_PROP_ENTRY static_left_props [] =
{
    { NCSP_STATIC_ALIGN, NCS_ALIGN_LEFT },  //Align mode
    { NCSP_STATIC_VALIGN, NCS_VALIGN_CENTER },//Align mode
    {0, 0}
};

static NCS_RDR_ELEMENT sledit_rdr_info_elements[] = {
	{NCS_FGC_WINDOW , 0XFFFF0000},
	{NCS_BGC_WINDOW , 0XFFFF0000},
	{NCS_FGC_3DBODY           , 0xFFFF0101},
  {NCS_BGC_3DBODY           , 0xFFFF0101},
	{ -1, 0 }
};

static NCS_RDR_INFO sledit_rdr_ifno[] =
{
    {"flat", "flat", sledit_rdr_info_elements},
};

static NCS_PROP_ENTRY sledit_password_1_props [] =
{
    //{ NCSP_WIDGET_BKIMAGE, (DWORD)&numPasswordBmp },
    //{ NCSP_WIDGET_BKIMAGE_MODE, NCS_DM_NORMAL },
    { NCSP_EDIT_LIMITTEXT, MAX_PASSWORD_LEN },
    { NCSP_SLEDIT_TIPTEXT, (DWORD)"请输入密码" },
    { NCSP_SLEDIT_PWDCHAR, (DWORD)'*' },
    {0, 0}
};
static NCS_PROP_ENTRY sledit_password_2_props [] =
{
    //{ NCSP_WIDGET_BKIMAGE, (DWORD)&numPasswordBmp },
    //{ NCSP_WIDGET_BKIMAGE_MODE, NCS_DM_NORMAL },
    { NCSP_EDIT_LIMITTEXT, MAX_PASSWORD_LEN },
    { NCSP_SLEDIT_TIPTEXT, (DWORD)"请再次输入密码" },
    { NCSP_SLEDIT_PWDCHAR, (DWORD)'*' },
    {0, 0}
};


// START_OF_TEMPLATE
static NCS_WND_TEMPLATE _ctrl_templ[] =
{
    {
        NCSCTRL_IMAGE,
        IDC_RECORD_PASSWORD_TIP_BG_IMAGE,
        0, 0, 480, 75,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_IMAGE,
        IDC_RECORD_PASSWORD_BG_IMAGE,
        0, 75, 480, 885,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_RECORD_PASSWORD_RETURN_BTN,
    	0, 0, 75, 75,
    	WS_VISIBLE | WS_CHILD | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
      NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_RECORD_PASSWORD_TIP_STC,
        100, 0, 280, 75,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "",
        static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_RECORD_PASSWORD_SLEDIT_HEAD_STC,
        50, 150, 380, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "设置密码 sign up",
        static_left_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_IMAGE,
        IDC_RECORD_PASSWORD_SLEDIT_BG_IMAGE,
        40, 200, 400, 200,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_SLEDIT,
        IDC_RECORD_PASSWORD_PASSWORD_EDIT_1,
        50, 220, 380, 50,
        WS_VISIBLE | WS_BORDER | WS_THICKFRAME | NCSS_EDIT_LEFT | NCSS_NOTIFY | NCSS_EDIT_BASELINE,// | NCSS_SLEDIT_PASSWORD,
        WS_EX_NONE,
        "",
        sledit_password_1_props,
        sledit_rdr_ifno,  //rdr_info
        sledit_handlers,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_SLEDIT,
        IDC_RECORD_PASSWORD_PASSWORD_EDIT_2,
        50, 280, 380, 50,
        WS_VISIBLE | WS_CHILD | NCSS_EDIT_LEFT | NCSS_NOTIFY | NCSS_SLEDIT_TIP | NCSS_EDIT_BASELINE,// | NCSS_SLEDIT_PASSWORD,
        WS_EX_TRANSPARENT,
        "",
        sledit_password_2_props,
        sledit_rdr_ifno,  //rdr_info
        sledit_handlers,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_IMAGE,
        IDC_RECORD_PASSWORD_STATE_IMAGE,
        50, 340, 50, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_RECORD_PASSWORD_STATE_STC,
        100, 340, 330, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "密码最少为6位",
        static_left_props,
        state_static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_RECORD_PASSWORD_ENTER_BTN,
        168, 420, 143, 67,
        WS_VISIBLE | WS_CHILD | NCSS_BUTTON_IMAGE,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        ok_btn_rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
};

static NCS_MNWND_TEMPLATE mywnd_templ = {
    NCSCTRL_DIALOGBOX,
    IDC_RECORD_PASSWORD_DIALOGBOX,
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
    0,                      //font_name
    0,                      //hIcon
    0,                      //hMenu
    0,                      //notify_info
    0                       //special_id
};
// END_OF_TEMPLATE

mDialogBox* DialogRecordPasswordViewCreate(HWND parent_hwnd, eRECORD_PASSWORD_TYPE type)
{
    this_type = type;
    dlg = (mDialogBox *)ncsCreateMainWindowIndirect (&mywnd_templ, parent_hwnd);
    return dlg;
}
//==============================================================================================
//end files

