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
#include "userManageView.h"
#include "showMessageBox.h"
#include "showMessageBoxQuery.h"

//#include "faceAuthCommon.h"
#define MAX_ITEM_PER_PAGE     12

static mDialogBox* dlg =NULL;

static BITMAP returnBtnBmp;
static BITMAP delAllBtnBmp;
static uint32_t this_total_page, this_cur_page;
//======================================================================================
static void SetListvBkColor(mListView* lstvObj)
{
    int color = 0xFFA0A000;
    _c(lstvObj)->setBackground(lstvObj, 0, 0, &color);
    _c(lstvObj)->setBackground(lstvObj, 0, 1, &color);
    _c(lstvObj)->setBackground(lstvObj, 0, 2, &color);
    _c(lstvObj)->setBackground(lstvObj, 2, 0, &color);
    _c(lstvObj)->setBackground(lstvObj, 2, 1, &color);
    _c(lstvObj)->setBackground(lstvObj, 2, 2, &color);
    _c(lstvObj)->setBackground(lstvObj, 4, 0, &color);
    _c(lstvObj)->setBackground(lstvObj, 4, 1, &color);
    _c(lstvObj)->setBackground(lstvObj, 4, 2, &color);
    _c(lstvObj)->setBackground(lstvObj, 6, 0, &color);
    _c(lstvObj)->setBackground(lstvObj, 6, 1, &color);
    _c(lstvObj)->setBackground(lstvObj, 6, 2, &color);
    _c(lstvObj)->setBackground(lstvObj, 8, 0, &color);
    _c(lstvObj)->setBackground(lstvObj, 8, 1, &color);
    _c(lstvObj)->setBackground(lstvObj, 8, 2, &color);
    _c(lstvObj)->setBackground(lstvObj, 10, 0, &color);
    _c(lstvObj)->setBackground(lstvObj, 10, 1, &color);
    _c(lstvObj)->setBackground(lstvObj, 10, 2, &color);
    color = 0xFF808000;
    _c(lstvObj)->setBackground(lstvObj, 1, 0, &color);
    _c(lstvObj)->setBackground(lstvObj, 1, 1, &color);
    _c(lstvObj)->setBackground(lstvObj, 1, 2, &color);
    _c(lstvObj)->setBackground(lstvObj, 3, 0, &color);
    _c(lstvObj)->setBackground(lstvObj, 3, 1, &color);
    _c(lstvObj)->setBackground(lstvObj, 3, 2, &color);
    _c(lstvObj)->setBackground(lstvObj, 5, 0, &color);
    _c(lstvObj)->setBackground(lstvObj, 5, 1, &color);
    _c(lstvObj)->setBackground(lstvObj, 5, 2, &color);
    _c(lstvObj)->setBackground(lstvObj, 7, 0, &color);
    _c(lstvObj)->setBackground(lstvObj, 7, 1, &color);
    _c(lstvObj)->setBackground(lstvObj, 7, 2, &color);
    _c(lstvObj)->setBackground(lstvObj, 9, 0, &color);
    _c(lstvObj)->setBackground(lstvObj, 9, 1, &color);
    _c(lstvObj)->setBackground(lstvObj, 9, 2, &color);
    _c(lstvObj)->setBackground(lstvObj, 11, 0, &color);
    _c(lstvObj)->setBackground(lstvObj, 11, 1, &color);
    _c(lstvObj)->setBackground(lstvObj, 11, 2, &color);
}

static void AddUserInfoToItem(mListView *self, int index, structUserInfo * user)
{
    NCS_LISTV_ITEMINFO info;
    NCS_LISTV_ITEMDATA subdata[3];

    char str_name[64];
    char str_id[20];

    subdata[0].flags = 0;
    subdata[0].image = 0;
    subdata[0].row = index;
    subdata[0].col = 0;
    sprintf(str_name, "小钴科技%d", this_cur_page * MAX_ITEM_PER_PAGE + index);
    subdata[0].text = str_name;
    subdata[0].textColor = 0xFFFFFFFF;

    subdata[1].flags = 0;
    subdata[1].image = 0;
    subdata[1].row = index;
    subdata[1].col = 1;
    sprintf(str_id, "%03d", this_cur_page * MAX_ITEM_PER_PAGE + index);
    subdata[1].text = str_id;
    subdata[1].textColor = 0xFFFFFFFF;

    subdata[2].flags = 0;
    subdata[2].image = 0;
    subdata[2].row = index;
    subdata[2].col = 2;
    subdata[2].text = (index & 0x1L) ? "普通用户" : "管理员";
    subdata[2].textColor = 0xFFFFFFFF;

    info.height = 64;
	info.flags = 0;
	info.foldIcon = 0;
	info.unfoldIcon = 0;
    info.parent = 0;
	info.index = index;
    info.dataSize = 3;
	info.data = subdata;
	_c(self)->addItem (self, &info);
}

static void AddItemForPage(mListView * lstvObj, uint32_t page)
{
    _c(lstvObj)->removeAll(lstvObj);
    AddUserInfoToItem(lstvObj, 0, NULL);
    AddUserInfoToItem(lstvObj, 1, NULL);
    AddUserInfoToItem(lstvObj, 2, NULL);
    AddUserInfoToItem(lstvObj, 3, NULL);
    AddUserInfoToItem(lstvObj, 4, NULL);
    AddUserInfoToItem(lstvObj, 5, NULL);
    AddUserInfoToItem(lstvObj, 6, NULL);
    AddUserInfoToItem(lstvObj, 7, NULL);
    AddUserInfoToItem(lstvObj, 8, NULL);
    AddUserInfoToItem(lstvObj, 9, NULL);
    AddUserInfoToItem(lstvObj, 10, NULL);
    AddUserInfoToItem(lstvObj, 11, NULL);

    SetListvBkColor(lstvObj);
}

static void UpdatePageInfo(uint32_t cur_page, uint32_t total_page)
{
    char str[20];
    sprintf(str, "%u/%u", (cur_page + 1), total_page);
    printf("listview UpdatePageInfo %s\n", str);
    SetWindowText (GetDlgItem(dlg->hwnd, IDC_USER_MANAGE_PAGE_STATUS_STC), str);
    printf("listview UpdatePageInfo end\n");
}
//==================================================================================================

//======================================================================================
//common function
static void set_image_info(mWidget* self, int id, PBITMAP pbmp, int align_id, int align)
{
    mImage *img = (mImage *)ncsGetChildObj(self->hwnd, id);

    if(img)
    {
        _c(img)->setProperty(img, NCSP_IMAGE_IMAGE, (DWORD)pbmp);
        _c(img)->setProperty(img, align_id, align);
    }
}

static void set_button_info(mWidget* self, int id, PBITMAP pbmp, int align_id, int align)
{
    mButton *btn = (mButton *)ncsGetChildObj(self->hwnd, id);

    if(btn)
    {
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

static BOOL OnDeleteAllButtonClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    if(ShowMessageBoxQuery(self->hwnd, ICON_FILE_PATH_ICON_WARNING_NORMAL, "确定要删除所有用户?"))
    {
        ShowMessageBox(HWND_DESKTOP, ICON_FILE_PATH_ICON_SUCC_NORMAL, "删除成功!");
    }
    else
    {
        ShowMessageBox(HWND_DESKTOP, ICON_FILE_PATH_ICON_WARNING_NORMAL, "删除失败!");
    }
    return FALSE;
}

static BOOL OnListViewClicked (mMainWnd *self, mListView *sender, int id, DWORD param)
{
    int sel = _c(sender)->getCurSel(sender);
    printf("OnListViewClicked sel=%u\n", this_cur_page * MAX_ITEM_PER_PAGE + sel);

    return FALSE;
}

static BOOL OnPrevPageClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    printf("OnPrevClicked\n");
    if(this_cur_page > 0)
    {
        this_cur_page --;
        AddItemForPage((mListView*)ncsObjFromHandle(GetDlgItem (self->hwnd, IDC_USER_MANAGE_LISTVIEW)), this_cur_page);
        UpdatePageInfo(this_cur_page, this_total_page);
    }
    else
    {
        ShowMessageBox(self->hwnd, ICON_FILE_PATH_ICON_WARNING_NORMAL, "已经是第一页!");
    }
    return FALSE;
}
static BOOL OnNextPageClicked (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    printf("OnNextClicked\n");
    if(this_cur_page < (this_total_page - 1))
    {
        this_cur_page ++;
        AddItemForPage((mListView*)ncsObjFromHandle(GetDlgItem (self->hwnd, IDC_USER_MANAGE_LISTVIEW)), this_cur_page);
        UpdatePageInfo(this_cur_page, this_total_page);
    }
    else
    {
        ShowMessageBox(self->hwnd, ICON_FILE_PATH_ICON_WARNING_NORMAL, "已经是最后一页!");
    }
    return FALSE;
}

static BOOL RegisterCtrlHanders (mWidget* self, DWORD addData)
{
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_USER_MANAGE_RETURN_BTN)),
                         (mObject*)self,
                         (NCS_CB_ONOBJEVENT)OnReturnButtonClicked,
                         NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_USER_MANAGE_DELETE_ALL_BTN)),
                         (mObject*)self,
                         (NCS_CB_ONOBJEVENT)OnDeleteAllButtonClicked,
                         NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mListView*)_c(self)->getChild (self, IDC_USER_MANAGE_LISTVIEW)),
                         (mObject*)self,
                         (NCS_CB_ONOBJEVENT)OnListViewClicked,
                         NCSN_LISTV_CLICKED);
    ncsAddEventListener ((mObject*)((mListView*)_c(self)->getChild (self, IDC_USER_MANAGE_PREV_PAGE_BTN)),
                         (mObject*)self,
                         (NCS_CB_ONOBJEVENT)OnPrevPageClicked,
                         NCSN_LISTV_CLICKED);
    ncsAddEventListener ((mObject*)((mListView*)_c(self)->getChild (self, IDC_USER_MANAGE_NEXT_PAGE_BTN)),
                         (mObject*)self,
                         (NCS_CB_ONOBJEVENT)OnNextPageClicked,
                         NCSN_LISTV_CLICKED);
  return TRUE;
}

//======================================================================================
//windos events
static BOOL OnCreate(mWidget* self, DWORD add_data)
{//TODO : initialize

    SetWindowFont(GetDlgItem(self->hwnd, IDC_USER_MANAGE_TIP_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_USER_MANAGE_DELETE_ALL_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_USER_MANAGE_LISTVIEW), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_USER_MANAGE_HEADER_NAME_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_USER_MANAGE_HEADER_ID_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_USER_MANAGE_HEADER_AUTH_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_USER_MANAGE_PAGE_STATUS_STC), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_USER_MANAGE_PREV_PAGE_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_USER_MANAGE_NEXT_PAGE_BTN), logfont_unifont_default);


    if (0 != LoadBitmapFromFile(HDC_SCREEN, &returnBtnBmp, IMAGE_FILE_PATH_USER_MANAGE_RETURN_BTN))
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_USER_MANAGE_RETURN_BTN);
    }
    set_button_info(self, IDC_USER_MANAGE_RETURN_BTN, &returnBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    if (0 != LoadBitmapFromFile(HDC_SCREEN, &delAllBtnBmp, IMAGE_FILE_PATH_USER_MANAGE_DEL_ALL_BTN))
    {
        GuiDbgPrintf("cannot load image file \"%s\"\n", IMAGE_FILE_PATH_USER_MANAGE_DEL_ALL_BTN);
    }
    set_button_info(self, IDC_USER_MANAGE_DELETE_ALL_BTN, &delAllBtnBmp, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);

    RegisterCtrlHanders(self, add_data);
    return NCSR_CONTINUE_MSG;
}


static void OnPaint (mWidget *self, HDC hdc, const CLIPRGN* inv)
{
    SetBkMode (hdc, BM_TRANSPARENT);
}

static BOOL OnClose(mWidget* self, int message)
{
    GuiDbgPrintf("UserManage OnClose\n");

    DestroyMainWindow(self->hwnd);
    MainWindowCleanup(self->hwnd);
    return NCSR_STOP_MSG;
}
static BOOL OnDestroy(mWidget* self, int message)
{
    DestroyAllControls(self->hwnd);
    UnloadBitmap(&returnBtnBmp);
    UnloadBitmap(&delAllBtnBmp);
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
static BOOL lstv_init(mDialogBox* self)
{
    mListView *lstvObj;
    NCS_LISTV_CLMINFO lstv_clminfo;
    //HWND hwnd = GetDlgItem (self->hwnd, IDC_USER_MANAGE_LISTVIEW);
    lstvObj = (mListView*)ncsObjFromHandle(GetDlgItem (self->hwnd, IDC_USER_MANAGE_LISTVIEW));
	if (!lstvObj) return FALSE;

	_c(lstvObj)->freeze(lstvObj, TRUE);

    //add column
    lstv_clminfo.index = 0;
	lstv_clminfo.text = "姓名";
	lstv_clminfo.width = 238;
	lstv_clminfo.pfnCmp = NULL;
	lstv_clminfo.flags = NCSF_LSTCLM_CENTERALIGN | NCSF_LSTHDR_CENTERALIGN | NCSF_LSTCLM_VCENTERALIGN;
	_c(lstvObj)->addColumn(lstvObj, &lstv_clminfo);
	lstv_clminfo.index = 1;
	lstv_clminfo.text = "ID";
	lstv_clminfo.width = 100;
	lstv_clminfo.pfnCmp = NULL;
	lstv_clminfo.flags = NCSF_LSTCLM_CENTERALIGN | NCSF_LSTHDR_CENTERALIGN;
	_c(lstvObj)->addColumn(lstvObj, &lstv_clminfo);
    lstv_clminfo.index = 2;
	lstv_clminfo.text = "权限";
	lstv_clminfo.width = 140;
	lstv_clminfo.pfnCmp = NULL;
	lstv_clminfo.flags = NCSF_LSTCLM_CENTERALIGN | NCSF_LSTHDR_CENTERALIGN;
	_c(lstvObj)->addColumn(lstvObj, &lstv_clminfo);

    AddItemForPage(lstvObj, 0);
    UpdatePageInfo(this_cur_page, this_total_page);
    _c(lstvObj)->freeze(lstvObj, FALSE);
    return TRUE;
}
//======================================================================================
static NCS_RDR_ELEMENT btn_rdr_elements[] =
{
  {NCS_FGC_WINDOW , 0XFFFFFFFF},
  { -1, 0 }
};

static NCS_RDR_INFO btn_rdr_info[] =
{
    {"flat", "flat", btn_rdr_elements},
};
/*static NCS_RDR_ELEMENT delete_all_btn_rdr_elements[] =
{
    {NCS_FGC_3DBODY , 0xFFFF0000},
    {NCS_BGC_3DBODY , 0xFFFF0000},
    {NCS_METRICS_3DBODY_ROUNDY, 15},
    {NCS_FGC_WINDOW , 0XFF0000FF},
    { -1, 0 }
};
static NCS_RDR_INFO delete_all_btn_rdr_info[] =
{
    {"flat", "flat", delete_all_btn_rdr_elements},
};
*/
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

static NCS_RDR_ELEMENT header_stc_rdr_elements[] =
{
  {NCS_FGC_WINDOW , 0xFFFFFFFF},
  { -1, 0 }
};
static NCS_RDR_INFO header_static_rdr_info[] = {
  { "flat","flat", header_stc_rdr_elements }
};
static NCS_PROP_ENTRY header_static_props [] =
{
    { NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER },  //Align mode
    { NCSP_STATIC_VALIGN, NCS_VALIGN_CENTER },//Align mode
    {0, 0}
};


static NCS_PROP_ENTRY rect_props [] = {
    {NCSP_RECTANGLE_BORDERSIZE, 0},
    //{NCSP_RECTANGLE_BORDERCOLOR, 0xFF000000},
    {NCSP_RECTANGLE_FILLCOLOR, 0xFF31221c},
    {0, 0}
};

static NCS_RDR_ELEMENT listview_rdr_elements[] =
{
  {NCS_FGC_WINDOW , 0XFF00FFFF},
  {NCS_BGC_HILIGHT_ITEM , 0XFFFF0000},
  { -1, 0 }
};
static NCS_RDR_INFO listview_rdr_info[] =
{
    {"flat", "flat", listview_rdr_elements},
};
static NCS_PROP_ENTRY listview_props [] =
{
    { NCSP_LISTV_HDRVISIBLE,    0      },
    { NCSP_LISTV_HDRHEIGHT,     0      },
    //{ NCSP_LISTV_HDRWIDTH,      480     },
    { NCSP_LISTV_DEFITEMHEIGHT, 64      },
    { NCSP_LISTV_GRIDLINEWIDTH, 0      },
    //{ NCSP_LISTV_GRIDLINECOLOR, 0X00000000},
    {0, 0}
};

static NCS_WND_TEMPLATE _ctrl_templ[] =
{
    {
        NCSCTRL_STATIC,
        IDC_USER_MANAGE_TIP_STC,
        100, 0, 280, 75,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "用户管理",
        static_props,
        static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_USER_MANAGE_RETURN_BTN,
        0, 0, 75, 75,
        WS_VISIBLE | WS_CHILD | NCSS_BUTTON_IMAGE,
        WS_EX_TRANSPARENT,
        "",
        NULL, //props,
        btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_USER_MANAGE_DELETE_ALL_BTN,
    	400, 0, 75, 75,
    	WS_VISIBLE | WS_CHILD | NCSS_BUTTON_IMAGE,
    	WS_EX_TRANSPARENT,
    	"删除所有用户",
    	NULL, //props,
    	btn_rdr_info, //delete_all_btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },

    {
        NCSCTRL_RECTANGLE,
        IDC_USER_MANAGE_HEADER_RECT,
        0, 75, 480, 55,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "",
        rect_props, //props
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_USER_MANAGE_HEADER_NAME_STC,
        0, 75, 240, 55,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "姓名",
        header_static_props,
        header_static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_USER_MANAGE_HEADER_ID_STC,
        240, 75, 100, 55,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "ID",
        header_static_props,
        header_static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_STATIC,
        IDC_USER_MANAGE_HEADER_AUTH_STC,
        340, 75, 140, 55,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "权限",
        header_static_props,
        header_static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_LISTVIEW,
        IDC_USER_MANAGE_LISTVIEW,
        0, 130, 480, 770,
        WS_VISIBLE | WS_CHILD | NCSS_LISTV_SINGLE | NCSS_LISTV_NOHDRDRAG | NCSS_LISTV_NOTIFY,
        WS_EX_NONE, //WS_EX_TRANSPARENT,
        "",
        listview_props, //props,
        listview_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_RECTANGLE,
        IDC_USER_MANAGE_BOTTOM_RECT,
        0, 898, 480, 62,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "",
        rect_props, //props
        NULL, //rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },

    {
        NCSCTRL_STATIC,
        IDC_USER_MANAGE_PAGE_STATUS_STC,
        190, 900, 100, 60,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "1/2",
        header_static_props,
        header_static_rdr_info,  //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_USER_MANAGE_PREV_PAGE_BTN,
    	0, 900, 190, 60,
    	WS_VISIBLE | WS_CHILD,
    	WS_EX_TRANSPARENT,
    	"上一页",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
    	NCSCTRL_BUTTON,
    	IDC_USER_MANAGE_NEXT_PAGE_BTN,
    	290, 900, 190, 60,
    	WS_VISIBLE | WS_CHILD,
    	WS_EX_TRANSPARENT,
    	"下一页",
    	NULL, //props,
    	btn_rdr_info, //rdr_info
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
};

static NCS_MNWND_TEMPLATE mywnd_templ = {
    NCSCTRL_DIALOGBOX,
    IDC_USER_MANAGE_DIALOGBOX,
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

mDialogBox* DialogUserManageViewCreate(HWND parent_hwnd)
{
    this_cur_page = 0;
    this_total_page = 2;
    dlg = (mDialogBox *)ncsCreateMainWindowIndirect (&mywnd_templ, parent_hwnd);
    lstv_init(dlg);
    return dlg;
}
//==============================================================================================
//end files

