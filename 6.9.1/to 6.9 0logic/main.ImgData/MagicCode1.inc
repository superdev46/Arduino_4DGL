//
// Added 6/3/2020 4:58:03 PM
//
// MagicCode1
//
while (processStatus == 0)
    agoStatus := 0;
    index := 0;
    txt_MoveCursor(4, 20);
    print("Loading...");
    serout('S');
    serout('0');
    pause(2000);
    processStatus := 5;
    //print(com_Count());
    if (com_Count()>0)
    //print("Started receiving init data\r\n");
       while(1)
         if (com_Count()>= 3250)
           processStatus := 5;
           //print("n-data received\r\n");
           break;
         endif
       wend
    endif
    //pause(2000);

wend

if (processStatus == 5)
    //pause(1000);
    getSerialFrame();
    //print('');
    if (activeStatus == 0)
        processStatus := 6;
    else
        processStatus :=7;
    endif
endif

if (processStatus == 6)
    pause(100);
    //com_Init(combuf, 4096, 0);
    getSerial();
    //processStatus :=3;
endif

if (processStatus == 7)
    pause(100);
    //com_Init(combuf, 4096, 0);
    getTemp();
    //processStatus :=3;
endif

while (processStatus == 1)
    agoStatus := 1;
    index := 0;
    txt_MoveCursor(4, 20);
    print("Loading...");
    serout('S');
    serout('D');
    pause(25000);
    processStatus := 5;
    //print(com_Count());
    if (com_Count()>0)
    //print("Started receiving init data\r\n");
       while(1)
         if (com_Count()>= 3250)
           processStatus := 5;
           txt_MoveCursor(3, 30);
           //print("daydata received\r\n");
           //print("n-data received\r\n");
           break;
         endif
       wend
    endif
wend



while (processStatus == 2)
    agoStatus := 2;
    index := 0;
    txt_MoveCursor(4, 20);
    print("Loading...");
    serout('S');
    serout('W');
    pause(25000);
    processStatus := 5;
    //print(com_Count());
    if (com_Count()>0)
    //print("Started receiving init data\r\n");
       while(1)
         if (com_Count()>= 3250)
           processStatus := 5;
           txt_MoveCursor(5, 30);
           //print("weekdata received\r\n");
           break;
         endif
       wend
    endif
wend

while (processStatus == 3)
    agoStatus := 3;
    index := 0;
    txt_MoveCursor(4, 20);
    print("Loading...");
    serout('S');
    serout('M');
    pause(25000);
    processStatus := 5;
    //print(com_Count());
    if (com_Count()>0)
    //print("Started receiving init data\r\n");
       while(1)
         if (com_Count()>= 3250)
           processStatus := 5;
           //print("n-data received\r\n");
           break;
         endif
       wend
    endif
wend

while (processStatus == 10)
   // agoStatus := 1;
    index := 0;
    txt_MoveCursor(4, 20);
    print("Loading...");
    serout('P');
    serout('V');
    pause(25000);
    processStatus := 5;
    //print(com_Count());
    if (com_Count()>0)
    //print("Started receiving init data\r\n");
       while(1)
         if (com_Count()>= 3250)
           processStatus := 5;
           //print("n-data received\r\n");
           break;
         endif
       wend
    endif
wend

while (processStatus == 11)
   //agoStatus := 1;
    index := 0;
    txt_MoveCursor(4, 20);
    print("Loading...");
    serout('N');
    serout('X');
    pause(25000);
    processStatus := 5;
    //print(com_Count());
    if (com_Count()>0)
    //print("Started receiving init data\r\n");
       while(1)
         if (com_Count()>= 3250)
           processStatus := 5;
           //print("n-data received\r\n");
           break;
         endif
       wend
    endif
wend

