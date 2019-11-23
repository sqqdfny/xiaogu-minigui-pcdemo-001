
#ifndef __SOFT_KEYPAD_VIEW_H__
#define __SOFT_KEYPAD_VIEW_H__

#define SOFT_KEYPAD_FUN_NUM     (0x01U)
#define SOFT_KEYPAD_FUN_EN      (0x02U)
#define SOFT_KEYPAD_FUN_PINYIN  (0x04U)

//soft_keypad_fun: not used
mWidget* DialogSoftKeypadViewCreate(HWND parent_hwnd, uint32_t soft_keypad_fun, int x, int y);

void CloseSoftKeypad();
BOOL IsSoftKeypadRun();


#endif  //__SOFT_KEYPAD_VIEW_H__
//end files
