#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <mgeff/mgeff-common.h>
#include <mgncs/mgncs.h>

#include "guicommon.h"

#include "mainWindow.h"
#include "numKeypadView.h"
#include "verifyFaceView.h"
#include "menuView.h"

static mDialogBox* dlg =NULL;
static BITMAP bgBmp;
static BITMAP batteryBmp[MAIN_BATT_MAX];
static BITMAP wifiBmp[MAIN_WIFI_MAX];
static BITMAP menuBtnBmp;
static BITMAP numBtnBmp;
static int batt_level;
static int wifi_status;
//==================================================================================
HWND GetMainWindowHwnd()
{
  return dlg->hwnd;
}
//==================================================================================
extern void flat_draw_3dbox(HDC hdc, const RECT *rc, int flag ,int state, int check_state, mWidget *owner);

static void transparent_buttonbox_paint(mButtonBoxPiece *self, HDC hdc, mWidget * owner, DWORD add_data)
{
   /* 如果有透明直接返回 */
    if(GetWindowExStyle(owner->hwnd) & WS_EX_TRANSPARENT)return;

    RECT rc;
    if(!_c(self)->getRect(self, &rc))
        return ;

    flat_draw_3dbox(hdc, &rc, NCSRF_FILL, add_data&NCS_PIECE_PAINT_STATE_MASK, NCS_PIECE_PAINT_GET_CHECK(add_data), owner);
}

//init boxpiece
void transparent_init_boxpiece_renderer(void)
{
    NCS_RDR_ENTRY entries [] =
    {
        {Class(mButtonBoxPiece).typeName, (mWidgetRenderer*)(void*)transparent_buttonbox_paint},
    };
    // 注册控件的渲染器函数,修改flat风格的button渲染器函擿
    // 可以用这个函数同时替换其他风枿`classic`,`fashion`,`skin`,`flat`)的对应函数
    ncsRegisterCtrlRDRs("flat",     entries, sizeof(entries)/sizeof(NCS_RDR_ENTRY));
    ncsRegisterCtrlRDRs("classic",  entries, sizeof(entries)/sizeof(NCS_RDR_ENTRY));
    ncsRegisterCtrlRDRs("fashion",  entries, sizeof(entries)/sizeof(NCS_RDR_ENTRY));
    ncsRegisterCtrlRDRs("skin",     entries, sizeof(entries)/sizeof(NCS_RDR_ENTRY));
}

//==================================================================================
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
//==================================================================================
//==================================================================================
//controls events
static BOOL OnMenuButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    //if(0 == GuiGetAdminCount())
    {
        DialogMenuViewCreate(self->hwnd);
    }
    //else
    //{
    //    DialogVerifyFaceViewCreate(self->hwnd, eVERIFY_FACE_TYPE_ADMIN);
    //}
    return FALSE;
}

static BOOL OnNumButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    DialogNumKeypadViewCreate(self->hwnd, eNUM_KEYPAD_TYPE_OPEN_DOOR);
    return FALSE;
}

static BOOL RegisterCtrlHanders (mWidget* self, DWORD addData)
{
	ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_MAIN_MENU_BTN)),
                        (mObject*)self, (NCS_CB_ONOBJEVENT)OnMenuButtonClicked, NCSN_WIDGET_CLICKED);

	ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_MAIN_NUM_BTN)),
                        (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
	return TRUE;
}
//==================================================================================
//windos events
static BOOL OnCreate(mWidget* self, DWORD add_data)
{//TODO : initialize
    batt_level = MAIN_BATT_0;
    wifi_status = MAIN_WIFI_0;

    SetWindowFont(GetDlgItem(self->hwnd, IDC_MAIN_WELLCOM_STC), logfont_unifont_default);
    //SetWindowFont(((mStatic*)_c(self)->getChild (self, IDC_MAIN_WELLCOM_STC))->hwnd, logfont_unifont_default);

    if (LoadBitmapFromFile(HDC_SCREEN, &bgBmp, IMAGE_FILE_PATH_MAIN_BG) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MAIN_BG);
    }
    set_image_info(self, IDC_MAIN_BG_IMAGE, &bgBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &batteryBmp[MAIN_BATT_0], IMAGE_FILE_PATH_MAIN_BATT_0) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MAIN_BATT_0);
    }
    if (LoadBitmapFromFile(HDC_SCREEN, &batteryBmp[MAIN_BATT_1], IMAGE_FILE_PATH_MAIN_BATT_1) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MAIN_BATT_1);
    }
    if (LoadBitmapFromFile(HDC_SCREEN, &batteryBmp[MAIN_BATT_2], IMAGE_FILE_PATH_MAIN_BATT_2) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MAIN_BATT_2);
    }
    if (LoadBitmapFromFile(HDC_SCREEN, &batteryBmp[MAIN_BATT_3], IMAGE_FILE_PATH_MAIN_BATT_3) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MAIN_BATT_3);
    }
    set_image_info(self, IDC_MAIN_BATTERY_IMAGE, &batteryBmp[batt_level], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &wifiBmp[MAIN_WIFI_0], IMAGE_FILE_PATH_MAIN_WIFI_0) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MAIN_WIFI_0);
    }
    if (LoadBitmapFromFile(HDC_SCREEN, &wifiBmp[MAIN_WIFI_1], IMAGE_FILE_PATH_MAIN_WIFI_1) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MAIN_WIFI_1);
    }
    if (LoadBitmapFromFile(HDC_SCREEN, &wifiBmp[MAIN_WIFI_2], IMAGE_FILE_PATH_MAIN_WIFI_2) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MAIN_WIFI_2);
    }
    if (LoadBitmapFromFile(HDC_SCREEN, &wifiBmp[MAIN_WIFI_3], IMAGE_FILE_PATH_MAIN_WIFI_3) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MAIN_WIFI_3);
    }
    set_image_info(self, IDC_MAIN_WIFI_IMAGE, &wifiBmp[wifi_status], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &menuBtnBmp, IMAGE_FILE_PATH_MAIN_MENU_BUTTON) != 0 )
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MAIN_MENU_BUTTON);
    }
    set_button_info(self, IDC_MAIN_MENU_BTN, &menuBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (LoadBitmapFromFile(HDC_SCREEN, &numBtnBmp, IMAGE_FILE_PATH_MAIN_NUM_BUTTON) != 0)
	{
		GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_MAIN_NUM_BUTTON);
	}
	set_button_info(self, IDC_MAIN_NUM_BTN, &numBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    RegisterCtrlHanders(self, add_data);

    SetTimer (self->hwnd, IDC_MAIN_TIMER, 200);
    return NCSR_CONTINUE_MSG;
}

static void OnClose(mWidget* self, int message)
{
    KillTimer (self->hwnd, IDC_MAIN_TIMER);

    UnloadBitmap(&bgBmp);
    UnloadBitmap(&batteryBmp[MAIN_BATT_0]);
    UnloadBitmap(&batteryBmp[MAIN_BATT_1]);
    UnloadBitmap(&batteryBmp[MAIN_BATT_2]);
    UnloadBitmap(&batteryBmp[MAIN_BATT_3]);
    UnloadBitmap(&wifiBmp[MAIN_WIFI_0]);
    UnloadBitmap(&wifiBmp[MAIN_WIFI_1]);
    UnloadBitmap(&wifiBmp[MAIN_WIFI_2]);
    UnloadBitmap(&wifiBmp[MAIN_WIFI_3]);
    UnloadBitmap(&menuBtnBmp);
    UnloadBitmap(&numBtnBmp);

	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}

static void OnPaint (mWidget *self, HDC hdc, const CLIPRGN* inv)
{
    //GuiDbgPrintf("main window OnPaint\n");
    SetBkMode (hdc, BM_TRANSPARENT);
}
static void OnTimer (mWidget *self, HDC hdc, const CLIPRGN* inv)
{
    //GuiDbgPrintf("main window OnTimer\n");
    if(++batt_level >= MAIN_BATT_MAX)
    {
        batt_level = 0;
    }
    if(++wifi_status >= MAIN_WIFI_MAX)
    {
        wifi_status = 0;
    }
    set_image_info(self, IDC_MAIN_BATTERY_IMAGE, &batteryBmp[batt_level], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
    set_image_info(self, IDC_MAIN_WIFI_IMAGE, &wifiBmp[wifi_status], NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

}
static void OnChildReturn(mWidget* self, int message, WPARAM wParam, LPARAM lParam)
{
    GuiDbgPrintf("numkeypad  OnChildReturn message=%d wpara=%lu lpara=%lu\n", message, (DWORD)wParam, (DWORD)lParam);
}

static NCS_EVENT_HANDLER handlers[] =
{
    {MSG_CREATE,    OnCreate    },
    {MSG_PAINT,     OnPaint     },
    {MSG_CLOSE,     OnClose     },
    {MSG_TIMER,     OnTimer     },
    {MSG_USR_CHILD_RETURN,  OnChildReturn },
    {0,             NULL        }
};

//==============================================================================================
// rdr_info
static NCS_RDR_ELEMENT stc_rdr_elements[] =
{
	{NCS_FGC_WINDOW , 0XFFFFFFFF},
	{ -1, 0 }
};

static NCS_RDR_INFO static_rdr_info[] = {
	{ "flat","flat", stc_rdr_elements }
};

static NCS_RDR_INFO btn_rdr_info[] = {
	{ "flat","flat", NULL }
};
//===========================================================================================
//Controls
static NCS_WND_TEMPLATE _ctrl_templ[] =
{
    {
        NCSCTRL_IMAGE,
        IDC_MAIN_BG_IMAGE,
        0, 75, 480, 885,
        WS_VISIBLE,
        WS_EX_NONE,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_MAIN_WELLCOM_STC,
        100, 340, 280, 60,
        WS_VISIBLE | SS_CENTER,
        WS_EX_TRANSPARENT,
        "星期三,欢迎回家",
        NULL,
        static_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_IMAGE,
        IDC_MAIN_BATTERY_IMAGE,
        10, 23, 56, 29,
        WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_IMAGE,
        IDC_MAIN_WIFI_IMAGE,
        75, 19, 42, 36,
        WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_MAIN_MENU_BTN,
    	400, 0, 75, 75,
    	WS_VISIBLE | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_MAIN_NUM_BTN,
    	190, 750, 100, 100,
    	NCSS_BUTTON_IMAGE |WS_VISIBLE,
		WS_EX_TRANSPARENT,
		"",
		NULL, //props,
		btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
};

//define the main window template
static NCS_MNWND_TEMPLATE main_templ =
{
    NCSCTRL_DIALOGBOX,
    IDC_MAIN_DIALOGBOX,
    0, 0, 480, 960,
    WS_VISIBLE,
    WS_EX_AUTOSECONDARYDC,
    "",
    NULL,
    static_rdr_info,
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

int MiniGUIMain(int argc, const char* argv[])
{
    MSG Msg;

    ncsInitialize();
    //ncs4TouchInitialize();
    transparent_init_boxpiece_renderer();
    LoadIncoreData();

	//mDialogBox  mMainWnd
	dlg = (mDialogBox *)ncsCreateMainWindowIndirect(&main_templ, HWND_DESKTOP);

    mGEffInit();

	// START_OF_MSGLOOP
	while (GetMessage (&Msg, dlg->hwnd))
	{
		TranslateMessage (&Msg);
		DispatchMessage (&Msg);
	}
	// END_OF_MSGLOOP

    MainWindowThreadCleanup(dlg->hwnd);
    UnloadIncoreData();

    mGEffDeinit();

    //ncs4TouchUninitialize();
    ncsUninitialize ();
	return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
//========================================================================================
//end files 