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
#include "numKeypadView.h"
#include "showMessageBox.h"


static mDialogBox* dlg =NULL;

static eNUM_KEYPAD_TYPE keypad_type;

static BITMAP bgBmp;
static BITMAP numBtnBmp[10];
static BITMAP backBtnBmp;
static BITMAP okBtnBmp;
static BITMAP numPasswordBmp;
static BITMAP batteryBmp[MAIN_BATT_MAX];
static BITMAP wifiBmp[MAIN_WIFI_MAX];
static BITMAP menuBtnBmp;
static BITMAP returnBtnBmp;

static int batt_level;
static int wifi_status;

static BOOL verify_result;


static char pwd_str[MAX_PASSWORD_LEN +1]={""};
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

static eVERIFY_TYPE GetVerifyType()
{
    eVERIFY_TYPE verify_type;
    switch(keypad_type)
    {
        case eNUM_KEYPAD_TYPE_OPEN_DOOR:
            verify_type = eVERIFY_TYPE_OPEN_DOOR;
            break;
        case eNUM_KEYPAD_TYPE_VERIFY_CUSTOMER:
            verify_type = eVERIFY_TYPE_CUSTOMER;
            break;
        default:
            verify_type = eVERIFY_TYPE_ADMIN;
    }
    return verify_type;
}
//======================================================================================
static BOOL OnNumButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    int id_btn, num;
    char text[MAX_PASSWORD_LEN +1]={""};
    char temp_char[2];

    id_btn = _c(sender)->getId(sender);
    //GuiDbgPrintf("Num button clicked %d\n", id_btn - IDC_NUM_KEYPAD_NUM_0_BTN);
    num = strlen(pwd_str);
    if(num >= MAX_PASSWORD_LEN)
    {
        GuiDbgPrintf("Password num is max!\n");
        return FALSE;
    }

    if(id_btn >= IDC_NUM_KEYPAD_NUM_0_BTN && id_btn <= IDC_NUM_KEYPAD_NUM_9_BTN)
    {
        temp_char[0] = '0' + (char)(id_btn - IDC_NUM_KEYPAD_NUM_0_BTN);
        temp_char[1] = '\0';

        strncat(pwd_str,temp_char,2);
        num = strlen(pwd_str);

        if (num >= 6)
        {
            ShowWindow(GetDlgItem(self->hwnd, IDC_NUM_KEYPAD_OK_BTN), SW_SHOW);
        }

        memset(text, '*', num);
        text[num] = '\0';
        SetWindowText (((mStatic *)ncsGetChildObj(self->hwnd, IDC_NUM_KEYPAD_PASSWORD_STC))->hwnd, text);

        GuiDbgPrintf("password:%s\n", pwd_str);
    }
    return FALSE;
}

static BOOL OnReturnButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    SendMessage (self->hwnd, MSG_CLOSE, 0, 0);
    return FALSE;
}

static BOOL OnBackButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    memset(pwd_str, 0, MAX_PASSWORD_LEN +1);
    ShowWindow(GetDlgItem (self->hwnd, IDC_NUM_KEYPAD_OK_BTN), SW_HIDE);
    SetWindowText (((mStatic *)ncsGetChildObj(self->hwnd, IDC_NUM_KEYPAD_PASSWORD_STC))->hwnd, "请输入密码");
    return FALSE;
}

static BOOL OnOkButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    verify_result = GuiVerifyPassword(pwd_str, GetVerifyType());
    //ShowVerifyResultMessageCreate(self->hwnd, verify_result);
    if(verify_result)
    {
        ShowMessageBox(self->hwnd, ICON_FILE_PATH_ICON_SUCC_NORMAL, "验证成功");
    }
    else
    {
        ShowMessageBox(self->hwnd, ICON_FILE_PATH_ICON_FAIL_NORMAL, "验证失败");
    }
    memset(pwd_str, 0, MAX_PASSWORD_LEN +1);
    ShowWindow(GetDlgItem (self->hwnd, IDC_NUM_KEYPAD_OK_BTN), SW_HIDE);
    SetWindowText (((mStatic *)ncsGetChildObj(self->hwnd, IDC_NUM_KEYPAD_PASSWORD_STC))->hwnd, "请输入密码");
    return FALSE;
}

static BOOL OnMenuButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    SendMessage (self->hwnd, MSG_CLOSE, 0, 0);
    return FALSE;
}

static BOOL RegisterCtrlHanders (mWidget* self, DWORD addData)
{
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_NUM_0_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_NUM_1_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_NUM_2_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_NUM_3_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_NUM_4_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_NUM_5_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_NUM_6_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_NUM_7_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_NUM_8_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_NUM_9_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_RETURN_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnReturnButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_BACK_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnBackButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_OK_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnOkButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_NUM_KEYPAD_MENU_BTN)),
                        (mObject*)self, (NCS_CB_ONOBJEVENT)OnMenuButtonClicked, NCSN_WIDGET_CLICKED);
	return TRUE;
}

//======================================================================================
//windos events
static BOOL OnCreate(mWidget* self, DWORD add_data)
{//TODO : initialize

    verify_result = FALSE;
    memset(pwd_str, 0, MAX_PASSWORD_LEN +1);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_NUM_KEYPAD_PASSWORD_STC), logfont_unifont_default);

    if (LoadBitmapFromFile(HDC_SCREEN, &bgBmp, IMAGE_FILE_PATH_NUM_KEYPAD_BG) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_BG);
    }
    set_image_info(self, IDC_NUM_KEYPAD_BG_IMAGE, &bgBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numBtnBmp[0], IMAGE_FILE_PATH_NUM_KEYPAD_NUM_0) != 0)
	{
		GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_NUM_0);
	}
	set_button_info(self, IDC_NUM_KEYPAD_NUM_0_BTN, &numBtnBmp[0], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numBtnBmp[1], IMAGE_FILE_PATH_NUM_KEYPAD_NUM_1) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_NUM_1);
    }
    set_button_info(self, IDC_NUM_KEYPAD_NUM_1_BTN, &numBtnBmp[1], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numBtnBmp[2], IMAGE_FILE_PATH_NUM_KEYPAD_NUM_2) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_NUM_2);
    }
    set_button_info(self, IDC_NUM_KEYPAD_NUM_2_BTN, &numBtnBmp[2], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numBtnBmp[3], IMAGE_FILE_PATH_NUM_KEYPAD_NUM_3) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_NUM_3);
    }
    set_button_info(self, IDC_NUM_KEYPAD_NUM_3_BTN, &numBtnBmp[3], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numBtnBmp[4], IMAGE_FILE_PATH_NUM_KEYPAD_NUM_4) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_NUM_4);
    }
    set_button_info(self, IDC_NUM_KEYPAD_NUM_4_BTN, &numBtnBmp[4], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numBtnBmp[5], IMAGE_FILE_PATH_NUM_KEYPAD_NUM_5) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_NUM_5);
    }
    set_button_info(self, IDC_NUM_KEYPAD_NUM_5_BTN, &numBtnBmp[5], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numBtnBmp[6], IMAGE_FILE_PATH_NUM_KEYPAD_NUM_6) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_NUM_6);
    }
    set_button_info(self, IDC_NUM_KEYPAD_NUM_6_BTN, &numBtnBmp[6], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numBtnBmp[7], IMAGE_FILE_PATH_NUM_KEYPAD_NUM_7) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_NUM_7);
    }
    set_button_info(self, IDC_NUM_KEYPAD_NUM_7_BTN, &numBtnBmp[7], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numBtnBmp[8], IMAGE_FILE_PATH_NUM_KEYPAD_NUM_8) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_NUM_8);
    }
    set_button_info(self, IDC_NUM_KEYPAD_NUM_8_BTN, &numBtnBmp[8], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numBtnBmp[9], IMAGE_FILE_PATH_NUM_KEYPAD_NUM_9) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_NUM_9);
    }
    set_button_info(self, IDC_NUM_KEYPAD_NUM_9_BTN, &numBtnBmp[9], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &backBtnBmp, IMAGE_FILE_PATH_NUM_KEYPAD_BACK) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_BACK);
    }
    set_button_info(self, IDC_NUM_KEYPAD_BACK_BTN, &backBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &returnBtnBmp, IMAGE_FILE_PATH_NUM_KEYPAD_RETURN) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_RETURN);
    }
    set_button_info(self, IDC_NUM_KEYPAD_RETURN_BTN, &returnBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);


    if (LoadBitmapFromFile(HDC_SCREEN, &okBtnBmp, IMAGE_FILE_PATH_NUM_KEYPAD_OK) != 0)
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_OK);
    }
    set_button_info(self, IDC_NUM_KEYPAD_OK_BTN, &okBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numPasswordBmp, IMAGE_FILE_PATH_NUM_KEYPAD_PASSWORD) != 0)
    {
        printf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_PASSWORD);
    }
    set_image_info(self, IDC_NUM_KEYPAD_PASSWORD_IMAGE, &numPasswordBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &batteryBmp[MAIN_BATT_0], IMAGE_FILE_PATH_NUM_KEYPAD_BATT_0) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_BATT_0);
    }
    if (LoadBitmapFromFile(HDC_SCREEN, &batteryBmp[MAIN_BATT_1], IMAGE_FILE_PATH_NUM_KEYPAD_BATT_1) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_BATT_1);
    }
    if (LoadBitmapFromFile(HDC_SCREEN, &batteryBmp[MAIN_BATT_2], IMAGE_FILE_PATH_NUM_KEYPAD_BATT_2) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_BATT_2);
    }
    if (LoadBitmapFromFile(HDC_SCREEN, &batteryBmp[MAIN_BATT_3], IMAGE_FILE_PATH_NUM_KEYPAD_BATT_3) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_BATT_3);
    }
    set_image_info(self, IDC_NUM_KEYPAD_BATTERY_IMAGE, &batteryBmp[batt_level], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &wifiBmp[MAIN_WIFI_0], IMAGE_FILE_PATH_NUM_KEYPAD_WIFI_0) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_WIFI_0);
    }
    if (LoadBitmapFromFile(HDC_SCREEN, &wifiBmp[MAIN_WIFI_1], IMAGE_FILE_PATH_NUM_KEYPAD_WIFI_1) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_WIFI_1);
    }
    if (LoadBitmapFromFile(HDC_SCREEN, &wifiBmp[MAIN_WIFI_2], IMAGE_FILE_PATH_NUM_KEYPAD_WIFI_2) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_WIFI_2);
    }
    if (LoadBitmapFromFile(HDC_SCREEN, &wifiBmp[MAIN_WIFI_3], IMAGE_FILE_PATH_NUM_KEYPAD_WIFI_3) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_WIFI_3);
    }
    set_image_info(self, IDC_NUM_KEYPAD_WIFI_IMAGE, &wifiBmp[wifi_status], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &menuBtnBmp, IMAGE_FILE_PATH_NUM_KEYPAD_MENU_BUTTON) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_NUM_KEYPAD_MENU_BUTTON);
    }
    set_button_info(self, IDC_NUM_KEYPAD_MENU_BTN, &menuBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    RegisterCtrlHanders(self, add_data);

    if(eNUM_KEYPAD_TYPE_OPEN_DOOR == keypad_type)
    {
        ShowWindow(GetDlgItem (self->hwnd, IDC_NUM_KEYPAD_BATTERY_IMAGE), SW_SHOW);
        ShowWindow(GetDlgItem (self->hwnd, IDC_NUM_KEYPAD_WIFI_IMAGE), SW_SHOW);
    }
    else
    {
        ShowWindow(GetDlgItem (self->hwnd, IDC_NUM_KEYPAD_RETURN_BTN), SW_SHOW);
    }
    SetTimer (self->hwnd, IDC_NUM_KEYPAD_TIMER, 200);
    return NCSR_CONTINUE_MSG;
}


static void OnPaint (mWidget *self, HDC hdc, const CLIPRGN* inv)
{
    SetBkMode (hdc, BM_TRANSPARENT);
}

static void OnTimer (mWidget *self, HDC hdc, const CLIPRGN* inv)
{
    if(eNUM_KEYPAD_TYPE_OPEN_DOOR == keypad_type)
    {
        GuiDbgPrintf("numkeypad OnTimer\n");
        if(++batt_level >= MAIN_BATT_MAX)
        {
            batt_level = 0;
        }
        if(++wifi_status >= MAIN_WIFI_MAX)
        {
            wifi_status = 0;
        }
        set_image_info(self, IDC_NUM_KEYPAD_BATTERY_IMAGE, &batteryBmp[batt_level], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
        set_image_info(self, IDC_NUM_KEYPAD_WIFI_IMAGE, &wifiBmp[wifi_status], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
    }
}

static void OnKillFocus(mWidget* self, int message)
{
    //SendMessage (self->hwnd, MSG_CLOSE, 0, 0);
}

static BOOL OnClose(mWidget* self, int message)
{
    KillTimer (self->hwnd, IDC_NUM_KEYPAD_TIMER);

    //SendMessage (GetParent(self->hwnd), MSG_USR_CHILD_RETURN, verify_result, 2);

    DestroyMainWindow(self->hwnd);
    MainWindowCleanup(self->hwnd);

    return NCSR_STOP_MSG;
}

static BOOL OnDestroy(mWidget* self, int message, WPARAM wParam, LPARAM lParam)
{
    DestroyAllControls(self->hwnd);

    UnloadBitmap(&bgBmp);
    UnloadBitmap(&numBtnBmp[0]);
    UnloadBitmap(&numBtnBmp[1]);
    UnloadBitmap(&numBtnBmp[2]);
    UnloadBitmap(&numBtnBmp[3]);
    UnloadBitmap(&numBtnBmp[4]);
    UnloadBitmap(&numBtnBmp[5]);
    UnloadBitmap(&numBtnBmp[6]);
    UnloadBitmap(&numBtnBmp[7]);
    UnloadBitmap(&numBtnBmp[8]);
    UnloadBitmap(&numBtnBmp[9]);
    UnloadBitmap(&returnBtnBmp);
    UnloadBitmap(&backBtnBmp);
    UnloadBitmap(&okBtnBmp);
    UnloadBitmap(&numPasswordBmp);
    UnloadBitmap(&batteryBmp[MAIN_BATT_0]);
    UnloadBitmap(&batteryBmp[MAIN_BATT_1]);
    UnloadBitmap(&batteryBmp[MAIN_BATT_2]);
    UnloadBitmap(&batteryBmp[MAIN_BATT_3]);
    UnloadBitmap(&wifiBmp[MAIN_WIFI_0]);
    UnloadBitmap(&wifiBmp[MAIN_WIFI_1]);
    UnloadBitmap(&wifiBmp[MAIN_WIFI_2]);
    UnloadBitmap(&wifiBmp[MAIN_WIFI_3]);
    UnloadBitmap(&menuBtnBmp);
    return NCSR_STOP_MSG;
}

static NCS_EVENT_HANDLER handlers[] =
{
    {MSG_CREATE,            OnCreate    },
    {MSG_PAINT,             OnPaint     },
    {MSG_CLOSE,             OnClose     },
    {MSG_TIMER,             OnTimer     },
    {MSG_KILLFOCUS,         OnKillFocus },
    {MSG_DESTROY,           OnDestroy   },
    {0,                     NULL        }
};

//======================================================================================
static NCS_RDR_INFO btn_rdr_num_info[] =
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
static NCS_PROP_ENTRY static_password_props [] =
{
    { NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER },  //Align mode
    { NCSP_STATIC_VALIGN, NCS_VALIGN_CENTER },//Align mode
    {0, 0}
};

// START_OF_TEMPLATE
static NCS_WND_TEMPLATE _ctrl_templ[] =
{
    {
        NCSCTRL_IMAGE,
        IDC_NUM_KEYPAD_BG_IMAGE,
        0, 75, 480, 885,
        WS_VISIBLE,
        WS_EX_NONE,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_NUM_1_BTN,
        45, 195, 90, 90,
        WS_VISIBLE | NCSS_BUTTON_IMAGE,
        WS_EX_TRANSPARENT,
        "1",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_NUM_2_BTN,
        195, 195, 90, 90,
        WS_VISIBLE  |NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "2",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_NUM_3_BTN,
        345, 195, 90, 90,
        WS_VISIBLE  |NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "3",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_NUM_4_BTN,
        45, 345, 90, 90,
        WS_VISIBLE  |NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "4",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_NUM_5_BTN,
        195, 345, 90, 90,
        WS_VISIBLE  |NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "5",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_NUM_6_BTN,
        345, 345, 90, 90,
        WS_VISIBLE  |NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "6",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_NUM_7_BTN,
        45, 495, 90, 90,
        WS_VISIBLE  |NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "7",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_NUM_8_BTN,
        195, 495, 90, 90,
        WS_VISIBLE  |NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "8",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_NUM_9_BTN,
        345, 495, 90, 90,
        WS_VISIBLE  |NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "9",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_NUM_0_BTN,
        195, 645, 90, 90,
        WS_VISIBLE | NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "0",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_OK_BTN,
        64, 800, 351, 70,
        NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "OK",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_NUM_KEYPAD_PASSWORD_STC,
        46, 100, 388, 52,
        WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "请输入密码",
        static_password_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_IMAGE,
        IDC_NUM_KEYPAD_PASSWORD_IMAGE,
        46, 100, 388, 52,
        WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_RETURN_BTN,
        0, 0, 75, 75,
        NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_NUM_KEYPAD_BACK_BTN,
        350, 101, 60, 50,
        WS_VISIBLE | NCSS_BUTTON_IMAGE ,
        WS_EX_TRANSPARENT,
        "0",
        NULL,
        btn_rdr_num_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_IMAGE,
        IDC_NUM_KEYPAD_BATTERY_IMAGE,
        10, 23, 56, 29,
        WS_NONE,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_IMAGE,
        IDC_NUM_KEYPAD_WIFI_IMAGE,
        75, 19, 42, 36,
        WS_NONE,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_NUM_KEYPAD_MENU_BTN,
    	400, 0, 75, 75,
    	WS_VISIBLE | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"MENU",
    	NULL, //props,
    	btn_rdr_num_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
};

static NCS_MNWND_TEMPLATE mywnd_templ = {
    NCSCTRL_DIALOGBOX,
    IDC_NUM_KEYPAD_DIALOGBOX,
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

mDialogBox* DialogNumKeypadViewCreate(HWND parent_hwnd, eNUM_KEYPAD_TYPE type)
{
    keypad_type = type;
    dlg = (mDialogBox *)ncsCreateMainWindowIndirect (&mywnd_templ, parent_hwnd);
    return dlg;
}
//==============================================================================================
//end files

