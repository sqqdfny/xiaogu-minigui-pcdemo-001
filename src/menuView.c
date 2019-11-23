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
#include "menuView.h"
#include "editUserView.h"
#include "userManageView.h"
#include "unlockRecordManageView.h"


static mDialogBox* dlg =NULL;

static BITMAP returnBtnBmp;
static BITMAP addUserBtnBmp;
static BITMAP userManageBtnBmp;
static BITMAP recordManageBtnBmp;
static BITMAP netSetBtnBmp;
static BITMAP systemSetBtnBmp;
static BITMAP systemInfoBtnBmp;

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

static BOOL OnAddUserButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    DialogEditUserViewCreate(self->hwnd, eEDIT_USER_TYPE_ADD, 0);
    return FALSE;
}
static BOOL OnUserManageButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    DialogUserManageViewCreate(self->hwnd);
    return FALSE;
}
static BOOL OnRecordManageButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    GuiDbgPrintf("Menu OnRecordManageButtonClicked\n");
	DialogUnlockRecordManageViewCreate(self->hwnd);
    return FALSE;
}
static BOOL OnNetSetButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    GuiDbgPrintf("Menu OnNetSetButtonClicked\n");
    return FALSE;
}
static BOOL OnSystemSetButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    GuiDbgPrintf("Menu OnSystemSetButtonClicked\n");
    return FALSE;
}
static BOOL OnSystemInfoButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    GuiDbgPrintf("Menu OnSystemInfoButtonClicked\n");
    return FALSE;
}
static BOOL RegisterCtrlHanders (mWidget* self, DWORD addData)
{
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_MENU_RETURN_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnReturnButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_MENU_ADDUSER_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnAddUserButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_MENU_USERMANAGE_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnUserManageButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_MENU_RECORDMANAGE_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnRecordManageButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_MENU_NETSET_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNetSetButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_MENU_SYSTEMSET_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnSystemSetButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_MENU_SYSTEMINFO_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnSystemInfoButtonClicked, NCSN_WIDGET_CLICKED);
	return TRUE;
}

//======================================================================================
//windos events
static BOOL OnCreate(mWidget* self, DWORD add_data)
{//TODO : initialize
    SetWindowFont(GetDlgItem(self->hwnd, IDC_MENU_TIP_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_MENU_ADDUSER_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_MENU_USERMANAGE_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_MENU_RECORDMANAGE_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_MENU_NETSET_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_MENU_SYSTEMSET_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_MENU_SYSTEMINFO_STC), logfont_unifont_default);

    if (LoadBitmapFromFile(HDC_SCREEN, &returnBtnBmp, IMAGE_FILE_PATH_MENU_RETURN_BTN) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MENU_RETURN_BTN);
    }
    set_button_info(self, IDC_MENU_RETURN_BTN, &returnBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &addUserBtnBmp, IMAGE_FILE_PATH_MENU_ADDUSER_BTN) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MENU_ADDUSER_BTN);
    }
    set_button_info(self, IDC_MENU_ADDUSER_BTN, &addUserBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &userManageBtnBmp, IMAGE_FILE_PATH_MENU_USERMANAGE_BTN) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MENU_USERMANAGE_BTN);
    }
    set_button_info(self, IDC_MENU_USERMANAGE_BTN, &userManageBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &recordManageBtnBmp, IMAGE_FILE_PATH_MENU_RECORDMANAGE_BTN) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MENU_RECORDMANAGE_BTN);
    }
    set_button_info(self, IDC_MENU_RECORDMANAGE_BTN, &recordManageBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &netSetBtnBmp, IMAGE_FILE_PATH_MENU_NETSET_BTN) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MENU_NETSET_BTN);
    }
    set_button_info(self, IDC_MENU_NETSET_BTN, &netSetBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &systemSetBtnBmp, IMAGE_FILE_PATH_MENU_SYSTEMSET_BTN) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MENU_SYSTEMSET_BTN);
    }
    set_button_info(self, IDC_MENU_SYSTEMSET_BTN, &systemSetBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &systemInfoBtnBmp, IMAGE_FILE_PATH_MENU_SYSTEMINFO_BTN) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MENU_SYSTEMINFO_BTN);
    }
    set_button_info(self, IDC_MENU_SYSTEMINFO_BTN, &systemInfoBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    RegisterCtrlHanders(self, add_data);
    return NCSR_CONTINUE_MSG;
}


static void OnPaint (mWidget *self, HDC hdc, const CLIPRGN* inv)
{
    SetBkMode (hdc, BM_TRANSPARENT);
}

static BOOL OnClose(mWidget* self, int message)
{
    GuiDbgPrintf("menu OnClose\n");

	DestroyMainWindow(self->hwnd);
    MainWindowCleanup(self->hwnd);


    return NCSR_STOP_MSG;
}

static BOOL OnDestroy(mWidget* self, int message, WPARAM wParam, LPARAM lParam)
{
    GuiDbgPrintf("menu OnDestroy\n");

    DestroyAllControls(self->hwnd);

    UnloadBitmap(&returnBtnBmp);
    UnloadBitmap(&addUserBtnBmp);
    UnloadBitmap(&userManageBtnBmp);
    UnloadBitmap(&recordManageBtnBmp);
    UnloadBitmap(&netSetBtnBmp);
    UnloadBitmap(&systemSetBtnBmp);
    UnloadBitmap(&systemInfoBtnBmp);

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
static NCS_RDR_INFO btn_rdr_info[] =
{
    {"flat", "flat", NULL},
};

static NCS_RDR_ELEMENT stc_rdr_elements[] =
{
	{NCS_FGC_WINDOW , 0XFFFFFFFF},
	{ -1, 0 }
};
static NCS_RDR_INFO static_rdr_info[] = {
	{ "flat","flat", stc_rdr_elements }
};
static NCS_PROP_ENTRY static_props [] =
{
    { NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER },  //Align mode
    { NCSP_STATIC_VALIGN, NCS_VALIGN_CENTER },//Align mode
    {0, 0}
};
/*static NCS_PROP_ENTRY btn_adduser_props [] =
{
    //{ NCSP_BUTTON_IMAGE, (DWORD)&addUserBtnBmp },
    //{ NCSP_BUTTON_ALIGN, NCS_VALIGN_CENTER },
    //{ NCSP_BUTTON_VALIGN, NCS_VALIGN_CENTER },
    { NCSP_BUTTON_IMAGE_SIZE_PERCENT, 80 },
    //{ NCSP_BUTTON_, 80 },
    {0, 0}
};*/
// START_OF_TEMPLATE
static NCS_WND_TEMPLATE _ctrl_templ[] =
{
    {
        NCSCTRL_STATIC,
        IDC_MENU_TIP_STC,
        150, 0, 180, 75,
        WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "菜单",
        static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_MENU_RETURN_BTN,
    	0, 0, 75, 75,
    	WS_VISIBLE | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_MENU_ADDUSER_BTN,
    	60, 160, 140, 140,
    	WS_VISIBLE | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"新增用户",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_MENU_ADDUSER_STC,
        60, 300, 140, 40,
        WS_VISIBLE ,
        WS_EX_TRANSPARENT,
        "新增用户",
        static_props,//static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_MENU_USERMANAGE_BTN,
    	280, 160, 140, 140,
    	WS_VISIBLE | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"用户管理",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_MENU_USERMANAGE_STC,
        280, 300, 140, 40,
        WS_VISIBLE ,
        WS_EX_TRANSPARENT,
        "用户管理",
        static_props,//static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_MENU_RECORDMANAGE_BTN,
    	60, 430, 140, 140,
    	WS_VISIBLE | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"记录管理",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_MENU_RECORDMANAGE_STC,
        60, 570, 140, 40,
        WS_VISIBLE ,
        WS_EX_TRANSPARENT,
        "记录管理",
        static_props,//static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_MENU_NETSET_BTN,
    	280, 430, 140, 140,
    	WS_VISIBLE | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"网络设置",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_MENU_NETSET_STC,
        280, 570, 140, 40,
        WS_VISIBLE ,
        WS_EX_TRANSPARENT,
        "网络设置",
        static_props,//static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_MENU_SYSTEMSET_BTN,
    	60, 700, 140, 140,
    	WS_VISIBLE | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"系统设置",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_MENU_SYSTEMSET_STC,
        60, 840, 140, 40,
        WS_VISIBLE ,
        WS_EX_TRANSPARENT,
        "系统设置",
        static_props,//static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_MENU_SYSTEMINFO_BTN,
    	280, 700, 140, 140,
    	WS_VISIBLE | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"系统信息",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_MENU_SYSTEMINFO_STC,
        280, 840, 140, 40,
        WS_VISIBLE ,
        WS_EX_TRANSPARENT,
        "系统信息",
        static_props,//static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
};

static NCS_MNWND_TEMPLATE mywnd_templ = {
    NCSCTRL_DIALOGBOX,
    IDC_MENU_DIALOGBOX,
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

mDialogBox* DialogMenuViewCreate(HWND parent_hwnd)
{
    dlg = (mDialogBox *)ncsCreateMainWindowIndirect (&mywnd_templ, parent_hwnd);
    return dlg;
}
//==============================================================================================
//end files

