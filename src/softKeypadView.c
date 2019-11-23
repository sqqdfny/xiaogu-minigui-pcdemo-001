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
#include "softKeypadView.h"
//=====================================================================================
typedef enum
{
    eSOFT_KEYPAD_MODE_NUM = 0,
    eSOFT_KEYPAD_MODE_EN,
    eSOFT_KEYPAD_MODE_PINYIN,
    eSOFT_KEYPAD_MODE_MAX,
}eSOFT_KEYPAD_MODE;

typedef enum
{
    eSOFT_KEYPAD_SELECT_MODE_SYMBOL = 0,
    eSOFT_KEYPAD_SELECT_MODE_EN,
    eSOFT_KEYPAD_SELECT_MODE_PINYIN,
}eSOFT_KEYPAD_SELECT_MODE;

static mWidget* dlg =NULL;
static HWND this_parent_hwnd;
static uint32_t this_fun;           //not used
static eSOFT_KEYPAD_MODE this_mode;
static BOOL this_caps;     //TRUE->UPPER  FALSE->LOWER
static eSOFT_KEYPAD_SELECT_MODE this_select_mode;
static uint8_t this_select_page;


static const int scancode_num_tab[10] = {SCANCODE_0, SCANCODE_1, SCANCODE_2, SCANCODE_3, SCANCODE_4,
                                         SCANCODE_5, SCANCODE_6, SCANCODE_7, SCANCODE_8, SCANCODE_9};

#define SYMBOL_TAB_MAX    (6U)
static const char symbol_tab[SYMBOL_TAB_MAX][6][2] =
{
    "!",  "\"", "#", "$", "%", "&",
    "\'", "(",  ")", "*", "+", ",",
    "-",  ".",  "/", ":", ";", "<",
    "=",  ">",  "?", "@", "[", "\\",
    "]",  "^",  "_", "`", "{", "|",
    "}",  "~",  "",  "",  "",  "",
};

static const char en_char_upper_tab[8][6][2] =
{
    "A", "B", "C", "",  "", "",
    "D", "E", "F", "",  "", "",
    "G", "H", "I", "",  "", "",
    "J", "K", "L", "",  "", "",
    "M", "N", "O", "",  "", "",
    "P", "Q", "R", "S", "", "",
    "T", "U", "V", "",  "", "",
    "W", "X", "Y", "Z", "", "",
};
static const char en_char_lower_tab[8][6][2] =
{
    "a", "b", "c", "",  "", "",
    "d", "e", "f", "",  "", "",
    "g", "h", "i", "",  "", "",
    "j", "k", "l", "",  "", "",
    "m", "n", "o", "",  "", "",
    "p", "q", "r", "s", "", "",
    "t", "u", "v", "",  "", "",
    "w", "x", "y", "z", "", "",
};

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
static void ChangeToNextMode()
{
    if(++this_mode >= eSOFT_KEYPAD_MODE_MAX) //eSOFT_KEYPAD_MODE_PINYIN)
    {
        this_mode = eSOFT_KEYPAD_MODE_NUM;
    }
}
static void SetSelectSymbol(mWidget *self, uint8_t page)
{
    SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_0_BTN), symbol_tab[page][0]);
    SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_1_BTN), symbol_tab[page][1]);
    SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_2_BTN), symbol_tab[page][2]);
    SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_3_BTN), symbol_tab[page][3]);
    SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_4_BTN), symbol_tab[page][4]);
    SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_5_BTN), symbol_tab[page][5]);
}
static void SetSelectEnChar(mWidget *self, int index, BOOL caps)
{
    if(caps)
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_0_BTN), en_char_upper_tab[index][0]);
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_1_BTN), en_char_upper_tab[index][1]);
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_2_BTN), en_char_upper_tab[index][2]);
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_3_BTN), en_char_upper_tab[index][3]);
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_4_BTN), en_char_upper_tab[index][4]);
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_5_BTN), en_char_upper_tab[index][5]);
    }
    else
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_0_BTN), en_char_lower_tab[index][0]);
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_1_BTN), en_char_lower_tab[index][1]);
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_2_BTN), en_char_lower_tab[index][2]);
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_3_BTN), en_char_lower_tab[index][3]);
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_4_BTN), en_char_lower_tab[index][4]);
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_5_BTN), en_char_lower_tab[index][5]);
    }
}
static void SetSoftKeypadMode(mWidget* self, eSOFT_KEYPAD_MODE mode, BOOL caps)
{
    this_select_page = 0;
    this_select_mode = eSOFT_KEYPAD_SELECT_MODE_SYMBOL;
    if(eSOFT_KEYPAD_MODE_NUM == mode)
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_MODE_BTN), "数字");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_0_BTN), "0");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_2_BTN), "2");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_3_BTN), "3");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_4_BTN), "4");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_5_BTN), "5");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_6_BTN), "6");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_7_BTN), "7");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_8_BTN), "8");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_9_BTN), "9");
        EnableWindow(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_0_BTN), TRUE);
        EnableWindow(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_1_BTN), TRUE);
    }
    else
    if(eSOFT_KEYPAD_MODE_EN == mode)
    {
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_MODE_BTN), "英文");
        if(caps)
        {
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_0_BTN), "大写");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_2_BTN), "ABC");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_3_BTN), "DEF");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_4_BTN), "GHI");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_5_BTN), "JKL");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_6_BTN), "MNO");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_7_BTN), "PQRS");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_8_BTN), "TUV");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_9_BTN), "WXYZ");
        }
        else
        {
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_0_BTN), "小写");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_2_BTN), "abc");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_3_BTN), "def");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_4_BTN), "ghi");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_5_BTN), "jkl");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_6_BTN), "mno");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_7_BTN), "pqrs");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_8_BTN), "tuv");
            SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_9_BTN), "wxyz");
        }
        EnableWindow(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_0_BTN), TRUE);
        EnableWindow(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_1_BTN), FALSE);
    }
    else
    {//eSOFT_KEYPAD_MODE_PINYIN
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_MODE_BTN), "拼音");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_0_BTN), "0");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_2_BTN), "abc");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_3_BTN), "def");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_4_BTN), "ghi");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_5_BTN), "jkl");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_6_BTN), "mno");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_7_BTN), "pqrs");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_8_BTN), "tuv");
        SetWindowText (GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_9_BTN), "wxyz");
        EnableWindow(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_0_BTN), FALSE);
        EnableWindow(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_1_BTN), FALSE);
    }

    SetSelectSymbol(self, this_select_page);
}

//======================================================================================
static BOOL OnNumButtonClicked (mWidget *self, mButton *sender, int id, DWORD param)
{
    int id_btn = _c(sender)->getId(sender);
    //GuiDbgPrintf("SoftKeypad OnNumButtonClicked %d\n", id_btn - IDC_SOFT_KEYPAD_NUM_0_BTN);

    if(eSOFT_KEYPAD_MODE_NUM == this_mode)
    {
        //PostMessage (this_parent_hwnd, MSG_CHAR, '0' + id_btn - IDC_SOFT_KEYPAD_NUM_0_BTN, 0);
        PostMessage (this_parent_hwnd, MSG_KEYDOWN, scancode_num_tab[id_btn - IDC_SOFT_KEYPAD_NUM_0_BTN], 0);
    }
    else
    if(eSOFT_KEYPAD_MODE_EN == this_mode)
    {
        if(IDC_SOFT_KEYPAD_NUM_0_BTN == id_btn)
        {
            this_caps = !this_caps;
            SetSoftKeypadMode(self, this_mode, this_caps);
        }
        else
        if((id_btn >= IDC_SOFT_KEYPAD_NUM_2_BTN) && (id_btn <= IDC_SOFT_KEYPAD_NUM_9_BTN))
        {
            this_select_mode = eSOFT_KEYPAD_SELECT_MODE_EN;
            SetSelectEnChar(self, id_btn - IDC_SOFT_KEYPAD_NUM_2_BTN, this_caps);
        }
    }
    else
    {//eSOFT_KEYPAD_MODE_PINYIN

    }

    return FALSE;
}
static BOOL OnSelectButtonClicked (mWidget *self, mButton *sender, int id, DWORD param)
{
    int len;
    char str[6] = "";

    len = GetDlgItemText(self->hwnd, _c(sender)->getId(sender), str, 5);
    //GuiDbgPrintf("SoftKeypad OnSelectButtonClicked len = %d, text = %s\n", len, str);

    if(eSOFT_KEYPAD_SELECT_MODE_PINYIN != this_select_mode)
    {
        if('\0' != str[0])
        {
            PostMessage (this_parent_hwnd, MSG_CHAR, str[0], 0);
        }
    }
}

static BOOL OnSymbolButtonClicked (mWidget *self, mButton *sender, int id, DWORD param)
{
    //GuiDbgPrintf("SoftKeypad OnSymbolButtonClicked\n");
    if(eSOFT_KEYPAD_SELECT_MODE_SYMBOL != this_select_mode)
    {
        this_select_mode = eSOFT_KEYPAD_SELECT_MODE_SYMBOL;
        this_select_page = 0;
        SetSelectSymbol(self, this_select_page);
    }

    return FALSE;
}
static BOOL OnPagePrevButtonClicked (mWidget *self, mButton *sender, int id, DWORD param)
{
    //GuiDbgPrintf("SoftKeypad OnPagePrevButtonClicked\n");
    if(eSOFT_KEYPAD_SELECT_MODE_SYMBOL == this_select_mode)
    {
        if(this_select_page > 0)
        {
            this_select_page --;
            SetSelectSymbol(self, this_select_page);
        }
    }
    else
    if(eSOFT_KEYPAD_SELECT_MODE_PINYIN == this_select_mode)
    {

    }
    return FALSE;
}
static BOOL OnPageNextButtonClicked (mWidget *self, mButton *sender, int id, DWORD param)
{
    //GuiDbgPrintf("SoftKeypad OnPageNextButtonClicked\n");
    if(eSOFT_KEYPAD_SELECT_MODE_SYMBOL == this_select_mode)
    {
        if(this_select_page < (SYMBOL_TAB_MAX - 1))
        {
            this_select_page ++;
            SetSelectSymbol(self, this_select_page);
        }
    }
    else
    if(eSOFT_KEYPAD_SELECT_MODE_PINYIN == this_select_mode)
    {

    }
    return FALSE;
}

static BOOL OnSpaceButtonClicked (mWidget *self, mButton *sender, int id, DWORD param)
{
    PostMessage (this_parent_hwnd, MSG_KEYDOWN, SCANCODE_SPACE, 0);
    return FALSE;
}
static BOOL OnModeButtonClicked (mWidget *self, mButton *sender, int id, DWORD param)
{
    //GuiDbgPrintf("SoftKeypad OnModeButtonClicked\n");
    ChangeToNextMode();
    SetSoftKeypadMode(self, this_mode, this_caps);
    return FALSE;
}

static BOOL OnBackButtonClicked (mWidget *self, mButton *sender, int id, DWORD param)
{
    PostMessage (this_parent_hwnd, MSG_KEYDOWN, SCANCODE_BACKSPACE, 0);
    return FALSE;
}
static BOOL OnEnterButtonClicked (mWidget *self, mButton *sender, int id, DWORD param)
{
    PostMessage (this_parent_hwnd, MSG_KEYDOWN, SCANCODE_ENTER, 0);
    SendMessage (self->hwnd, MSG_CLOSE, 0, 0);
    dlg = NULL;
    return FALSE;
}

static BOOL RegisterCtrlHanders (mWidget* self, DWORD addData)
{
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_NUM_0_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_NUM_1_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_NUM_2_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_NUM_3_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_NUM_4_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_NUM_5_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_NUM_6_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_NUM_7_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_NUM_8_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_NUM_9_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnNumButtonClicked, NCSN_WIDGET_CLICKED);

    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_SELECT_0_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnSelectButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_SELECT_1_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnSelectButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_SELECT_2_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnSelectButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_SELECT_3_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnSelectButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_SELECT_4_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnSelectButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_SELECT_5_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnSelectButtonClicked, NCSN_WIDGET_CLICKED);

    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_SYMBOL_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnSymbolButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_SPACE_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnSpaceButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_MODE_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnModeButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_BACK_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnBackButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_ENTER_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnEnterButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_PAGE_PREV_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnPagePrevButtonClicked, NCSN_WIDGET_CLICKED);
    ncsAddEventListener ((mObject*)((mButton*)_c(self)->getChild (self, IDC_SOFT_KEYPAD_PAGE_NEXT_BTN)),
                            (mObject*)self, (NCS_CB_ONOBJEVENT)OnPageNextButtonClicked, NCSN_WIDGET_CLICKED);
	return TRUE;
}

//======================================================================================
//windos events
static BOOL OnCreate(mWidget* self, DWORD add_data)
{//TODO : initialize
    this_fun = this_fun;
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_0_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_1_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_2_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_3_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_4_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SELECT_5_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_PAGE_PREV_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_PAGE_NEXT_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_0_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_1_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_2_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_3_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_4_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_5_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_6_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_7_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_8_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_NUM_9_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_BACK_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SYMBOL_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_ENTER_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_MODE_BTN), logfont_unifont_default);
    SetWindowFont(GetDlgItem(self->hwnd, IDC_SOFT_KEYPAD_SPACE_BTN), logfont_unifont_default);

    SetSoftKeypadMode(self, eSOFT_KEYPAD_MODE_NUM, this_caps);

    RegisterCtrlHanders(self, add_data);

    return NCSR_CONTINUE_MSG;
}


static void OnPaint (mWidget *self, HDC hdc, const CLIPRGN* inv)
{
    SetBkMode (hdc, BM_TRANSPARENT);
}

static BOOL OnClose(mWidget* self, int message)
{
    GuiDbgPrintf("SoftKeypad OnClose\n");

    DestroyMainWindow(self->hwnd);
    MainWindowCleanup(self->hwnd);
    return NCSR_STOP_MSG;
}

static BOOL OnDestroy(mWidget* self, int message)
{
    DestroyAllControls(self->hwnd);
    return NCSR_STOP_MSG;
}

static NCS_EVENT_HANDLER handlers[] =
{
    {MSG_CREATE,            OnCreate    },
    {MSG_PAINT,             OnPaint     },
    {MSG_CLOSE,             OnClose     },
    {MSG_DESTROY,           OnDestroy   },
    {0,                     NULL        }
};

//======================================================================================
static NCS_RDR_ELEMENT select_btn_rdr_elements[] =
{
    {NCS_FGC_WINDOW , 0XFF000000},
	{ -1, 0 }
};
static NCS_RDR_INFO select_btn_rdr_info[] =
{
    {"flat", "flat", select_btn_rdr_elements},
};

static NCS_RDR_ELEMENT page_btn_rdr_elements[] =
{
    {NCS_FGC_3DBODY , 0xFFFFFFFF},
    {NCS_BGC_3DBODY , 0xFFFFFFFF},
    {NCS_FGC_WINDOW , 0XFF5F5F5F},
    {NCS_METRICS_3DBODY_ROUNDY, 5},
	{ -1, 0 }
};
static NCS_RDR_INFO page_btn_rdr_info[] =
{
    {"flat", "flat", page_btn_rdr_elements},
};

static NCS_RDR_ELEMENT num_btn_rdr_elements[] =
{
    {NCS_FGC_3DBODY ,           0xFFFFFFFF},
    {NCS_BGC_3DBODY ,           0xFFFFFFFF},
    {NCS_FGC_WINDOW ,           0XFF5F5F5F},
    {NCS_METRICS_3DBODY_ROUNDY, 5},
	{ -1, 0 }
};
static NCS_RDR_INFO num_btn_rdr_info[] =
{
    {"flat", "flat", num_btn_rdr_elements},
};

#define MAIN_WINDOW_BG_COLOR     MakeRGBA(0xEF, 0xFD, 0xEF, 0xC1)
static NCS_WND_TEMPLATE _ctrl_templ[] =
{
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_SELECT_0_BTN,
        70, 8, 50, 42,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "壹",
        NULL,
        select_btn_rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_SELECT_1_BTN,
        128, 8, 50, 42,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "贰",
        NULL,
        select_btn_rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_SELECT_2_BTN,
        186, 8, 50, 42,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "叁",
        NULL,
        select_btn_rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_SELECT_3_BTN,
        244, 8, 50, 42,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "肆",
        NULL,
        select_btn_rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_SELECT_4_BTN,
        302, 8, 50, 42,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "伍",
        NULL,
        select_btn_rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_SELECT_5_BTN,
        360, 8, 50, 42,
        WS_VISIBLE | WS_CHILD,
        WS_EX_TRANSPARENT,
        "陆",
        NULL,
        select_btn_rdr_info,
        NULL,NULL,0,0, MakeRGBA(0x00, 0x00, 0x00, 0xFF),0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_PAGE_PREV_BTN,
        8, 8, 48, 42,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "<",
        NULL,
        page_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_PAGE_NEXT_BTN,
        424, 8, 48, 42,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        ">",
        NULL,
        page_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_NUM_1_BTN,
        8, 58, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "1",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_NUM_2_BTN,
        126, 58, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "2",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_NUM_3_BTN,
        244, 58, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "3",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_NUM_4_BTN,
        8, 116, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "4",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_NUM_5_BTN,
        126, 116, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "5",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_NUM_6_BTN,
        244, 116, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "6",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_NUM_7_BTN,
        8, 174, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "7",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_NUM_8_BTN,
        126, 174, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "8",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_NUM_9_BTN,
        244, 174, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "9",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_NUM_0_BTN,
        126, 232, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "0",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_BACK_BTN,
        362, 58, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "退格",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_SYMBOL_BTN,
        362, 116, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "符号",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_ENTER_BTN,
        362, 174, 110, 109,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "完成",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_MODE_BTN,
        8, 232, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "数字",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
    {
        NCSCTRL_BUTTON,
        IDC_SOFT_KEYPAD_SPACE_BTN,
        244, 232, 110, 50,
        WS_VISIBLE | WS_CHILD,
        WS_EX_NONE,
        "空格",
        NULL,
        num_btn_rdr_info,
        NULL,NULL,0,0, MAIN_WINDOW_BG_COLOR,0,0,0,0,0  //handlers,//controls //count //add data//bk_color//font_name//hIcon//hMenu//notify_info//special_id
    },
};

static NCS_MNWND_TEMPLATE mywnd_templ = {
    NCSCTRL_DIALOGBOX,
    IDC_SOFT_KEYPAD_DIALOGBOX,
    0, 670, 480, 290,
    WS_VISIBLE,
    WS_EX_AUTOSECONDARYDC | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
    "softkeypad",
    NULL,
    NULL,
    handlers,
    _ctrl_templ,
    sizeof(_ctrl_templ)/sizeof(NCS_WND_TEMPLATE),
    0,                      //user_data
    MAIN_WINDOW_BG_COLOR,    //bk_color
    0,                      //font_name
    0,                      //hIcon
    0,                      //hMenu
    0,                      //notify_info
    0                       //special_id
};

mWidget* DialogSoftKeypadViewCreate(HWND parent_hwnd, uint32_t soft_keypad_fun, int x, int y)
{
    this_parent_hwnd = parent_hwnd;
    this_fun = soft_keypad_fun;
    this_mode = eSOFT_KEYPAD_MODE_NUM;
    this_caps = TRUE;
    mywnd_templ.x = 0;
    mywnd_templ.y = y;
    dlg = (mWidget *)ncsCreateMainWindowIndirect (&mywnd_templ, parent_hwnd);

    return dlg;
}

void CloseSoftKeypad()
{
    if(dlg)
    {
        SendMessage (dlg->hwnd, MSG_CLOSE, 0, 0);
        dlg = NULL;
    }
}

BOOL IsSoftKeypadRun()
{
    return ((NULL == dlg) ? FALSE : TRUE);
}
//==============================================================================================
//end files

