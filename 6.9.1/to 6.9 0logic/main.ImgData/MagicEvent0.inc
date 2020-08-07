//
// Added 6/3/2020 4:52:04 PM
//
func MagicEvent0()
    //activeStatus := 1;
    if  (img_GetWord(hndl, iSlider0, IMAGE_INDEX) == 0)
        txt_MoveCursor(4, 45);
        //print("slider is pos0");
        processStatus := 0;
        activeStatus := 0;

    endif
    if  (img_GetWord(hndl, iSlider0, IMAGE_INDEX) == 1)
        txt_MoveCursor(4, 45);
        //print("slider is pos1");
        processStatus := 1;
        activeStatus := 1;
    endif
    if  (img_GetWord(hndl, iSlider0, IMAGE_INDEX) == 2)
        txt_MoveCursor(4, 45);
        //print("slider is pos2");
        processStatus := 2;
        activeStatus := 2;
    endif
    if  (img_GetWord(hndl, iSlider0, IMAGE_INDEX) == 3)
        txt_MoveCursor(4, 45);
        //print("slider is pos3");
        processStatus := 3;
        activeStatus := 3;
    endif
endfunc
