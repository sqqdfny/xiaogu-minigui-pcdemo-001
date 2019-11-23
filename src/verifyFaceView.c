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
#include "verifyFaceView.h"
#include "numKeypadView.h"
#include "showMessageBox.h"

static mDialogBox* dlg =NULL;
static eVERIFY_FACE_TYPE verify_face_type;

static BITMAP bgBmp;
static BITMAP tipBgBmp;
static BITMAP btBgBmp;
static BITMAP returnBtnBmp;
static BITMAP numBtnBmp;

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
static BOOL OnNumButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    //GuiDbgPrintf("VerifyAdmin OnNumButtonClicked\n");
    DialogNumKeypadViewCreate(self->hwnd, eNUM_KEYPAD_TYPE_VERIFY_ADMIN);
    return FALSE;
}

static BOOL OnReturnButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    SendMessage (self->hwnd, MSG_CLOSE, 0, 0);
    return FALSE;
}

static BOOL RegisterCtrlHanders (mWidget* self, DWORD addData)
{
	GuiDbgPrintf("VerifyAdmin handers start!\n");

    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_VERIFY_FACE_NUMKEY_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_VERIFY_FACE_RETURN_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnReturnButtonClicked, NCSN_WIDGET_CLICKED);
	return TRUE;
}

//======================================================================================
//windos events
static BOOL OnCreate(mWidget* self, DWORD add_data)
{//TODO : initialize
    GuiDbgPrintf("VerifyFace OnCreate %lu\n", add_data);

    SetWindowFont(GetDlgItem(self->hwnd, IDC_VERIFY_FACE_STATE_BT_STATIC), logfont_unifont_default);

    if (LoadBitmapFromFile(HDC_SCREEN, &bgBmp, IMAGE_FILE_PATH_VERIFY_FACE_TIP_BG) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_VERIFY_FACE_TIP_BG);
    }
    set_image_info(self, IDC_VERIFY_FACE_TIP_BG_IMAGE, &bgBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &bgBmp, IMAGE_FILE_PATH_VERIFY_FACE_BG) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_VERIFY_FACE_BG);
    }
    set_image_info(self, IDC_VERIFY_FACE_BG_IMAGE, &bgBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &btBgBmp, IMAGE_FILE_PATH_VERIFY_FACE_BT_BG) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_VERIFY_FACE_BT_BG);
    }
    set_image_info(self, IDC_VERIFY_FACE_BT_BG_IMAGE, &btBgBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);


    if (LoadBitmapFromFile(HDC_SCREEN, &returnBtnBmp, IMAGE_FILE_PATH_VERIFY_FACE_RETURN_BTN) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_VERIFY_FACE_RETURN_BTN);
    }
    set_button_info(self, IDC_VERIFY_FACE_RETURN_BTN, &returnBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numBtnBmp, IMAGE_FILE_PATH_VERIFY_FACE_NUMKEY_BTN) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_VERIFY_FACE_NUMKEY_BTN);
    }
    set_button_info(self, IDC_VERIFY_FACE_NUMKEY_BTN, &numBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    RegisterCtrlHanders(self, add_data);

    if(eVERIFY_FACE_TYPE_ADMIN == verify_face_type)
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_VERIFY_FACE_STATE_BT_STATIC), "验证管理员");
    }
    else
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_VERIFY_FACE_STATE_BT_STATIC), "验证用户");
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
static BOOL OnDestroy(mWidget* self, int message)
{
    DestroyAllControls(self->hwnd);
    UnloadBitmap(&tipBgBmp);
    UnloadBitmap(&bgBmp);
    UnloadBitmap(&btBgBmp);
    UnloadBitmap(&returnBtnBmp);
    UnloadBitmap(&numBtnBmp);
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
static NCS_PROP_ENTRY static_password_props [] =
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

// START_OF_TEMPLATE
static NCS_WND_TEMPLATE _ctrl_templ[] =
{
    {
        NCSCTRL_IMAGE,
        IDC_VERIFY_FACE_TIP_BG_IMAGE,
        0, 0, 480, 75,
        WS_VISIBLE,
        WS_EX_NONE,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_IMAGE,
        IDC_VERIFY_FACE_BG_IMAGE,
        0, 75, 480, 694,
        WS_VISIBLE,
        WS_EX_NONE,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_IMAGE,
        IDC_VERIFY_FACE_BT_BG_IMAGE,
        0, 769, 480, 191,
        WS_VISIBLE,
        WS_EX_NONE,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_VERIFY_FACE_RETURN_BTN,
        0, 0, 75, 75,
        WS_VISIBLE | NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_VERIFY_FACE_NUMKEY_BTN,
    	380, 0, 100, 75,
    	WS_VISIBLE | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"",
    	NULL, //props,
    	btn_rdr_num_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_VERIFY_FACE_STATE_BT_STATIC,
        50, 830, 380, 80,
        WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "",
        static_password_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
};

static NCS_MNWND_TEMPLATE mywnd_templ = {
    NCSCTRL_DIALOGBOX,
    IDC_VERIFY_FACE_DIALOGBOX,
    0, 0, 480, 960,
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
    0,                     //font_name
    0,                      //hIcon
    0,                      //hMenu
    0,                      //notify_info
    0                       //special_id
};
// END_OF_TEMPLATE

mDialogBox* DialogVerifyFaceViewCreate(HWND parent_hwnd, eVERIFY_FACE_TYPE type)
{
    verify_face_type = type;
    dlg = (mDialogBox *)ncsCreateMainWindowIndirect (&mywnd_templ, parent_hwnd);
    return dlg;
}
//==============================================================================================
//end files

