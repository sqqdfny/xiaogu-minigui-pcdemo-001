
#ifndef __RECORD_PASSWORD_VIEW_H__
#define __RECORD_PASSWORD_VIEW_H__

typedef enum
{
	eRECORD_PASSWORD_TYPE_ADD = 0,
	eRECORD_PASSWORD_TYPE_MODIFY,
    eRECORD_PASSWORD_TYPE_QUERY,
}eRECORD_PASSWORD_TYPE;

mDialogBox* DialogRecordPasswordViewCreate(HWND parent_hwnd, eRECORD_PASSWORD_TYPE type);


#endif  //__RECORD_PASSWORD_VIEW_H__
//end files
