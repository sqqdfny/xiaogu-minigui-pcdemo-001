
#ifndef __RECORD_FACE_VIEW_H__
#define __RECORD_FACE_VIEW_H__

typedef enum
{
	eRECORD_FACE_TYPE_ADD = 0,
	eRECORD_FACE_TYPE_MODIFY,
    eRECORD_FACE_TYPE_QUERY,
}eRECORD_FACE_TYPE;

mDialogBox* DialogRecordFaceViewCreate(HWND parent_hwnd, eRECORD_FACE_TYPE type);


#endif  //__RECORD_FACE_VIEW_H__
//end files
