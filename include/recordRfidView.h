
#ifndef __RECORD_RFID_VIEW_H__
#define __RECORD_RFID_VIEW_H__

typedef enum
{
	eRECORD_RFID_TYPE_ADD = 0,
	eRECORD_RFID_TYPE_MODIFY,
    eRECORD_RFID_TYPE_QUERY,
}eRECORD_RFID_TYPE;

mDialogBox* DialogRecordRfidViewCreate(HWND parent_hwnd, eRECORD_RFID_TYPE type);


#endif  //__RECORD_RFID_VIEW_H__
//end files
