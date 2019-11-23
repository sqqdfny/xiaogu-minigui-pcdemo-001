
#ifndef __EDIT_USER_VIEW_H__
#define __EDIT_USER_VIEW_H__

typedef enum
{
	eEDIT_USER_TYPE_ADD = 0,
	eEDIT_USER_TYPE_MODIFY,
    eEDIT_USER_TYPE_QUERY,
}eEDIT_USER_TYPE;



mDialogBox* DialogEditUserViewCreate(HWND parent_hwnd, eEDIT_USER_TYPE type, uint32_t id);


#endif  //__EDIT_USER_VIEW_H__
//end files
