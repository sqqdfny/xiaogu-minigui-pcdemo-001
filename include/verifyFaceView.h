
#ifndef __VERIFY_FACE_VIEW_H__
#define __VERIFY_FACE_VIEW_H__

typedef enum
{
    eVERIFY_FACE_TYPE_OPEN_DOOR = 0,
    eVERIFY_FACE_TYPE_CUSTOMER,
    eVERIFY_FACE_TYPE_ADMIN,
}eVERIFY_FACE_TYPE;

mDialogBox* DialogVerifyFaceViewCreate(HWND parent_hwnd, eVERIFY_FACE_TYPE type);


#endif  //__VERIFY_FACE_VIEW_H__
//end files
