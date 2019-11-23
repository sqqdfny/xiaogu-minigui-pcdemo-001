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
#include "editUserView.h"
#include "menuView.h"
#include "showMessageBox.h"
#include "showMessageBoxQuery.h"
#include "recordFaceView.h"
#include "recordPasswordView.h"
#include "recordRfidView.h"

//======================================================================================
typedef struct
{
    eEDIT_USER_TYPE type;
    uint32_t id;
}sEDIT_USER_INFO;

static mDialogBox* dlg =NULL;

static sEDIT_USER_INFO edit_user_info;
static sUSER_INFO user_info;

static BITMAP returnBtnBmp;
static BITMAP enterBtnBmp;

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
static BOOL OnRecordFaceButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    if(eEDIT_USER_TYPE_ADD == edit_user_info.type)
    {
        DialogRecordFaceViewCreate(self->hwnd, eRECORD_FACE_TYPE_ADD);
    }
    else
    if(eEDIT_USER_TYPE_MODIFY == edit_user_info.type)
    {
        DialogRecordFaceViewCreate(self->hwnd, eRECORD_FACE_TYPE_MODIFY);
    }
    else
    {
        DialogRecordFaceViewCreate(self->hwnd, eRECORD_FACE_TYPE_QUERY);
    }
    return FALSE;
}
static BOOL OnRecordPasswordButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    if(eEDIT_USER_TYPE_ADD == edit_user_info.type)
    {
        DialogRecordPasswordViewCreate(self->hwnd, eRECORD_PASSWORD_TYPE_ADD);
    }
    else
    if(eEDIT_USER_TYPE_MODIFY == edit_user_info.type)
    {
        DialogRecordPasswordViewCreate(self->hwnd, eRECORD_PASSWORD_TYPE_MODIFY);
    }
    else
    {
        DialogRecordPasswordViewCreate(self->hwnd, eRECORD_PASSWORD_TYPE_QUERY);
    }
    return FALSE;
}
static BOOL OnRecordRfidButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    if(eEDIT_USER_TYPE_ADD == edit_user_info.type)
    {
        DialogRecordRfidViewCreate(self->hwnd, eRECORD_RFID_TYPE_ADD);
    }
    else
    if(eEDIT_USER_TYPE_MODIFY == edit_user_info.type)
    {
        DialogRecordRfidViewCreate(self->hwnd, eRECORD_RFID_TYPE_MODIFY);
    }
    else
    {
        DialogRecordRfidViewCreate(self->hwnd, eRECORD_RFID_TYPE_QUERY);
    }
    return FALSE;
}
static BOOL OnUserAuthButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    if(0 == GuiGetAdminCount())
    {
        GuiDbgPrintf("EditUserView no valid users\n");
        ShowMessageBox(self->hwnd, ICON_FILE_PATH_ICON_WARNING_NORMAL, "当前没有管理员\n请先设置一个管理员");
        return FALSE;
    }
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_RETURN_BTN), FALSE);
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDFACE_BTN), FALSE);
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDPASSWORD_BTN), FALSE);
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDRFID_BTN), FALSE);
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_BTN), FALSE);

    ShowWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_SAVE_BTN), SW_HIDE);
    ShowWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_ADMIN_BTN), SW_SHOW);
    ShowWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_CUSTOMER_BTN), SW_SHOW);
    return FALSE;
}
static BOOL OnUserAuthAdminButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_RETURN_BTN), TRUE);
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDFACE_BTN), TRUE);
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_BTN), TRUE);
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDPASSWORD_BTN), TRUE);
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDRFID_BTN), TRUE);

    ShowWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_SAVE_BTN), SW_SHOW);
    ShowWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_ADMIN_BTN), SW_HIDE);
    ShowWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_CUSTOMER_BTN), SW_HIDE);

    user_info.type = eUSER_TYPE_ADMIN;
    SetWindowText (GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_STC_RIGHT), (eUSER_TYPE_ADMIN == user_info.type) ? "管理员" : "普通用户");
    return FALSE;
}
static BOOL OnUserAuthCustomerButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_RETURN_BTN), TRUE);
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDFACE_BTN), TRUE);
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_BTN), TRUE);
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDPASSWORD_BTN), TRUE);
    EnableWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDRFID_BTN), TRUE);
    ShowWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_ADMIN_BTN), SW_HIDE);
    ShowWindow(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_CUSTOMER_BTN), SW_HIDE);

    user_info.type = eUSER_TYPE_CUSTOMER;
    SetWindowText (GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_STC_RIGHT), (eUSER_TYPE_ADMIN == user_info.type) ? "管理员" : "普通用户");
    return FALSE;
}
static BOOL OnUserSaveButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    GuiDbgPrintf("EditUserView OnUserSaveButtonClicked start\n");
    ShowMessageBoxQuery(self->hwnd, ICON_FILE_PATH_ICON_WARNING_NORMAL, "已有录入信息\n是否覆盖当前验证?");
    GuiDbgPrintf("EditUserView OnUserSaveButtonClicked end\n");
    return FALSE;
}

static BOOL RegisterCtrlHanders (mWidget* self, DWORD addData)
{
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_EDIT_USER_RETURN_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnReturnButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_EDIT_USER_RECORDFACE_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnRecordFaceButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_EDIT_USER_RECORDPASSWORD_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnRecordPasswordButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_EDIT_USER_RECORDRFID_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnRecordRfidButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_EDIT_USER_USERAUTH_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnUserAuthButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_EDIT_USER_USERAUTH_ADMIN_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnUserAuthAdminButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_EDIT_USER_USERAUTH_CUSTOMER_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnUserAuthCustomerButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_EDIT_USER_SAVE_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnUserSaveButtonClicked, NCSN_WIDGET_CLICKED);
	return TRUE;
}

//======================================================================================
//windos events
static BOOL OnCreate(mWidget* self, DWORD add_data)
{//TODO : initialize
    char str[32];
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_TIP_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERID_STC_LEFT), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERID_STC_RIGHT), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERNAME_STC_LEFT), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERNAME_STC_RIGHT), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDFACE_STC_LEFT), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDFACE_STC_RIGHT), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDPASSWORD_STC_LEFT), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDPASSWORD_STC_RIGHT), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDRFID_STC_LEFT), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_RECORDRFID_STC_RIGHT), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_STC_LEFT), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_STC_RIGHT), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_ADMIN_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_CUSTOMER_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_EDIT_USER_SAVE_BTN), logfont_unifont_default);

    if (LoadBitmapFromFile(HDC_SCREEN, &returnBtnBmp, IMAGE_FILE_PATH_EDIT_USER_RETURN_BTN) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_EDIT_USER_RETURN_BTN);
    }
    set_button_info(self, IDC_EDIT_USER_RETURN_BTN, &returnBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &enterBtnBmp, IMAGE_FILE_PATH_EDIT_USER_ENTER_BTN) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_EDIT_USER_ENTER_BTN);
    }
    set_button_info(self, IDC_EDIT_USER_RECORDFACE_BTN, &enterBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
    set_button_info(self, IDC_EDIT_USER_RECORDPASSWORD_BTN, &enterBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
    set_button_info(self, IDC_EDIT_USER_RECORDRFID_BTN, &enterBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
    set_button_info(self, IDC_EDIT_USER_USERAUTH_BTN, &enterBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    RegisterCtrlHanders(self, add_data);
//---------------------------------------------------------------------------------------------------------
    if(eEDIT_USER_TYPE_ADD == edit_user_info.type)
    {// if add new user
        user_info.isValid = FALSE;
        user_info.id = GuiApplyUserInfoId();
        user_info.type = (0 == GuiGetAdminCount()) ? eUSER_TYPE_ADMIN : eUSER_TYPE_CUSTOMER;
        sprintf(user_info.name, "xiaogu %u", user_info.id);
        user_info.password[0] = '\0';
        user_info.pic = NULL;
        user_info.face = NULL;
        user_info.rfid = NULL;
    }
    else
    {
        if(FALSE == GuiReadUserInfo(edit_user_info.id, &user_info))
        {
            //user_info.id =
        }
    }

    if(eEDIT_USER_TYPE_ADD == edit_user_info.type)
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_EDIT_USER_TIP_STC), "新增用户");
    }
    else
    if(eEDIT_USER_TYPE_MODIFY == edit_user_info.type)
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_EDIT_USER_TIP_STC), "修改用户");
    }
    else
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_EDIT_USER_TIP_STC), "查询用户信息");
    }

    sprintf(str, "%010u", user_info.id);
    SetWindowText (GetDlgItem(self->hwnd, IDC_EDIT_USER_USERID_STC_RIGHT), str);
    SetWindowText (GetDlgItem(self->hwnd, IDC_EDIT_USER_USERNAME_STC_RIGHT), user_info.name);
    SetWindowText (GetDlgItem(self->hwnd, IDC_EDIT_USER_USERAUTH_STC_RIGHT), (eUSER_TYPE_ADMIN == user_info.type) ? "管理员" : "普通用户");
//---------------------------------------------------------------------------------------------------------
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
static BOOL OnEraseBkgnd(mWidget* self, HDC hdc, const PRECT clip)
{ //NCS_CB_ONERASEBKGND
    //RECT rcTemp;
    //GuiDbgPrintf("AddUser OnEraseBkgnd\n");

    //SetWindowBkColor(self->hwnd, MakeRGBA(0x00, 0x00, 0x00, 0xFF));
    //FillBox(hdc, 0, 0, 480, 200);
    //SetBrushColor (self->hwnd, PIXEL_blue);
    //SetWindowBkColor(self->hwnd, MakeRGBA(0x00, 0x00, 0xFF, 0xFF));
    //FillBox(hdc, 0, 200, 480, 50);
    //SetWindowBkColor(self->hwnd, MakeRGBA(0x00, 0x00, 0x00, 0xFF));
    return NCSR_CONTINUE_MSG;
}
static BOOL OnDestroy(mWidget* self, int message)
{
    DestroyAllControls(self->hwnd);
    UnloadBitmap(&returnBtnBmp);
    return NCSR_STOP_MSG;
}

static NCS_EVENT_HANDLER handlers[] =
{
    {MSG_CREATE,        OnCreate    },
    {MSG_PAINT,         OnPaint     },
    {MSG_CLOSE,         OnClose     },
    {MSG_ERASEBKGND,    OnEraseBkgnd},
    {MSG_DESTROY,       OnDestroy   },
    {0,                 NULL        }
};

//======================================================================================
static NCS_RDR_INFO btn_rdr_info[] =
{
    {"flat", "flat", NULL},
};

static NCS_RDR_ELEMENT user_auth_select_btn_rdr_elements[] =
{
    {NCS_FGC_3DBODY , 0xFF31221c},
    {NCS_BGC_3DBODY , 0xFF31221c},
    //{NCS_METRICS_3DBODY_ROUNDX, 25},
    {NCS_METRICS_3DBODY_ROUNDY, 5},
	{NCS_FGC_WINDOW , 0XFFFFFFFF},
	{ -1, 0 }
};
static NCS_RDR_INFO user_auth_select_btn_rdr_info[] =
{
    {"flat", "flat", user_auth_select_btn_rdr_elements},
};

static NCS_RDR_ELEMENT save_btn_rdr_elements[] =
{
    {NCS_FGC_3DBODY , 0xFFFF0000},
    {NCS_BGC_3DBODY , 0xFFFF0000},
    {NCS_METRICS_3DBODY_ROUNDY, 35},
    {NCS_FGC_WINDOW , 0XFFFFFFFF},
	{ -1, 0 }
};
static NCS_RDR_INFO save_btn_rdr_info[] =
{
    {"flat", "flat", save_btn_rdr_elements},
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
static NCS_PROP_ENTRY left_static_props [] =
{
    { NCSP_STATIC_ALIGN, NCS_ALIGN_LEFT },  //Align mode
    { NCSP_STATIC_VALIGN, NCS_VALIGN_CENTER },//Align mode
    {0, 0}
};
static NCS_PROP_ENTRY right_static_props [] =
{
    { NCSP_STATIC_ALIGN, NCS_ALIGN_RIGHT },  //Align mode
    { NCSP_STATIC_VALIGN, NCS_VALIGN_CENTER },//Align mode
    {0, 0}
};

static NCS_PROP_ENTRY rect_props [] = {
    {NCSP_RECTANGLE_BORDERSIZE, 0},
    {NCSP_RECTANGLE_BORDERCOLOR, 0xFF000000},
    {NCSP_RECTANGLE_FILLCOLOR, 0xFF31221c},
    {0, 0}
};

static NCS_WND_TEMPLATE _ctrl_templ[] =
{
    {
        NCSCTRL_STATIC,
        IDC_EDIT_USER_TIP_STC,
        100, 0, 280, 75,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "",
        static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_EDIT_USER_RETURN_BTN,
    	0, 0, 75, 75,
    	WS_VISIBLE | WS_CHILD | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
      NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_RECTANGLE,
        IDC_EDIT_USER_USERID_RECT,
        0, 100, 480, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "",
        rect_props, //props
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_EDIT_USER_USERID_STC_LEFT,
        20, 100, 150, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "用户编号",
        left_static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_EDIT_USER_USERID_STC_RIGHT,
        170, 100, 230, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "",
        right_static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_RECTANGLE,
        IDC_EDIT_USER_USERNAME_RECT,
        0, 200, 480, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "",
        rect_props, //props
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_EDIT_USER_USERNAME_STC_LEFT,
        20, 200, 150, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "用户名称",
        left_static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_EDIT_USER_USERNAME_STC_RIGHT,
        170, 200, 230, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "",
        right_static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_RECTANGLE,
        IDC_EDIT_USER_RECORDFACE_RECT,
        0, 300, 480, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "",
        rect_props, //props
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_EDIT_USER_RECORDFACE_STC_LEFT,
        20, 300, 150, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "登记人脸",
        left_static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_EDIT_USER_RECORDFACE_BTN,
    	400, 300, 80, 80,
    	WS_VISIBLE | WS_CHILD | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
      NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_RECTANGLE,
        IDC_EDIT_USER_RECORDPASSWORD_RECT,
        0, 400, 480, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "",
        rect_props, //props
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_EDIT_USER_RECORDPASSWORD_STC_LEFT,
        20, 400, 150, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "登记密码",
        left_static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_EDIT_USER_RECORDPASSWORD_BTN,
    	400, 400, 80, 80,
    	WS_VISIBLE | WS_CHILD | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
      NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_RECTANGLE,
        IDC_EDIT_USER_RECORDRFID_RECT,
        0, 500, 480, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "",
        rect_props, //props
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_EDIT_USER_RECORDRFID_STC_LEFT,
        20, 500, 150, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "登记卡片",
        left_static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_EDIT_USER_RECORDRFID_BTN,
    	400, 500, 80, 80,
    	WS_VISIBLE | WS_CHILD | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
      NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_RECTANGLE,
        IDC_EDIT_USER_USERAUTH_RECT,
        0, 600, 480, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "",
        rect_props, //props
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_EDIT_USER_USERAUTH_STC_LEFT,
        20, 600, 150, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "用户权限",
        left_static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_EDIT_USER_USERAUTH_STC_RIGHT,
        170, 600, 230, 80,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "",
        right_static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_EDIT_USER_USERAUTH_BTN,
    	400, 600, 80, 80,
    	WS_VISIBLE | WS_CHILD | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
      NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_EDIT_USER_SAVE_BTN,
    	40, 760, 400, 80,
    	WS_VISIBLE | WS_CHILD | WS_BORDER,
    	WS_EX_NONE, //TRANSPARENT,
    	"保存",
    	NULL, //props,
    	save_btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_EDIT_USER_USERAUTH_ADMIN_BTN,
    	0, 700, 480, 80,
    	WS_CHILD | WS_BORDER,
    	WS_EX_NONE, //TRANSPARENT,
    	"管理员",
    	NULL, //props,
    	user_auth_select_btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_EDIT_USER_USERAUTH_CUSTOMER_BTN,
    	0, 780, 480, 80,
    	WS_CHILD | WS_BORDER,
    	WS_EX_NONE, //TRANSPARENT,
    	"普通用户",
    	NULL, //props,
    	user_auth_select_btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
};

static NCS_MNWND_TEMPLATE mywnd_templ = {
    NCSCTRL_DIALOGBOX,
    IDC_EDIT_USER_DIALOGBOX,
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

mDialogBox* DialogEditUserViewCreate(HWND parent_hwnd, eEDIT_USER_TYPE type, uint32_t id)
{
    edit_user_info.type = type;
    edit_user_info.id = id;
    dlg = (mDialogBox *)ncsCreateMainWindowIndirect (&mywnd_templ, parent_hwnd);
    return dlg;
}
//==============================================================================================
//end files

