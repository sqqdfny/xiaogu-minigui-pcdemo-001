
#ifndef __NUM_KEYPAD_VIEW_H__
#define __NUM_KEYPAD_VIEW_H__

typedef enum
{
    eNUM_KEYPAD_TYPE_OPEN_DOOR = 0,      //open door
    eNUM_KEYPAD_TYPE_VERIFY_CUSTOMER,    //verify customer
    eNUM_KEYPAD_TYPE_VERIFY_ADMIN,       //verify admin
}eNUM_KEYPAD_TYPE;
mDialogBox* DialogNumKeypadViewCreate(HWND parent_hwnd, eNUM_KEYPAD_TYPE type);


#endif  //__NUM_KEYPAD_VIEW_H__
//end files
