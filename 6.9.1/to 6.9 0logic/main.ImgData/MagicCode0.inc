//
// Added 6/3/2020 4:55:10 PM
//
// MagicCode0
//  ----6.13---

var values[10];
var gotMessage := 0;
var tempMessage[15];
var tMcount;
var combuf[4096];
var xTempSample[700];
var oldSample[700];
var processStatus;
var index;
var activeStatus:=0;
var agoStatus:=0;

func getSerial()                 //new temp...
    //print("getSerial precessing...");

    var recbyte, n, g;
    while(com_Count() > 0)
        if (activeStatus != agoStatus)
            //txt_MoveCursor(4, 9);
            //print("slider is changed to ", activeStatus,"processStatus:", processStatus);
            break;
        endif
        recbyte := serin();
        if(recbyte == 97) gotMessage := 1; // check for "a" and set message complete flag
        if(recbyte == 98) gotMessage := 2;
        if(gotMessage == 0) // if message not complete keep building
            tempMessage[tMcount] := recbyte;
            tMcount++;
        endif
    wend
    if (activeStatus != agoStatus)
        print("i'll goto break.");
        goto p;
    endif
    if(gotMessage == 1)
        //print("got1");
        gotMessage := 0;
        //var n;
        var multiplier;
        var tempval;
        var tempdate[10];
        tempval := 0;
        multiplier := 1;
        var kk := 0;
        txt_MoveCursor(4, 33);
        gfx_RectangleFilled(324, 68, 800, 100, BLACK);
        print("Date:");
        for(n := tMcount-1; n >= 0; n--)
            if (kk<4)
                if(tempMessage[n] > 47 && tempMessage[n] < 58)
                     tempval := tempval + ((tempMessage[n] - 48) * multiplier);
                     multiplier := multiplier *10;
                     //print("U:",tempMessage[n]);
                endif
            else
                if(tempMessage[n] > 47 && tempMessage[n] < 58)
                     tempdate[kk-4] := tempMessage[n] ;
                     putchXY(695-(kk-4)*10, 80, tempMessage[n]);
                     //print("U:", tempdate[kk-4]);
                else
                    if (tempMessage[n] == 47) putchXY(695-(kk-4)*10, 80,'/');
                endif
            endif
            kk := kk + 1;
        next
       //if(tempMessage[0] == 97) values[0] := tempval; // if message starts with 'a' 97 being ASCII for a
        // add morechecks here in the same format eg 'b600\r\n'
        tMcount := 0;

        WriteObject(tLeddigits, 0, tempval);   //Is it right?
       // WriteObject(tStrings, 0,"current temp date");
        //if (tempval > 0 && tempval<3500)
            for (n := 0; n<650; n++)   //mem_copy doesnt work correctly - move current data to old data
                oldSample[n] := xTempSample[n];
            next

            for (n := 0; n<649; n++)  //shifting data to allow new reading
                xTempSample[n] := xTempSample[n+1];
            next

            xTempSample[649] := ((tempval/10))*2;  //devide by 10 to make true integer temperature value
            //print("xTempSample[649]:",  xTempSample[649]);
            for (n := 0; n<650; n++) // print new pixels - old pixels blacked out or graticule color depending on location
                //g := 425 - oldSample[n];
                //(g == 425 || g == 375 || g == 325 || g == 275 || g == 225 || g == 175 || g == 125) ?  gfx_PutPixel((50+n),g ,0x0280): gfx_PutPixel((50+n),g ,BLACK);
                //gfx_PutPixel((55+n),(425 - xTempSample[n]) ,YELLOW);
                gfx_Vline((50+n),(450 - oldSample[n]),450,0x083C);   //
                gfx_Vline((50+n),480,450,BLACK);
                gfx_PutPixel((50+n),450, 0x0280);
                gfx_PutPixel((50+n),400, 0x0280);
                gfx_PutPixel((50+n),350, 0x0280);
                gfx_PutPixel((50+n),300, 0x0280);
                gfx_PutPixel((50+n),250, 0x0280);
                gfx_PutPixel((50+n),200, 0x0280);
                gfx_PutPixel((50+n),150, 0x0280);
                if ( activeStatus == 0 )
                    gfx_Vline((50+n),(450 - xTempSample[n]),450,YELLOW);
                else
                    gfx_Vline((50+n),(450 - xTempSample[n]),450,0x083C);
                endif
                //if (n==649)  gfx_Vline((50+n),(450 - xTempSample[n]),450,RED);
            next
        //endif
    endif
     if(gotMessage == 2)
        print("got2");
        gotMessage := 0;
        //var n;
        var tempDate:="";
        //txt_MoveCursor(3, 3);
        for(n := tMcount-1; n >= 0; n--)
                //tempval := tempval + tempMessage[n];
                // str_Copy(tempDate, tempMessage[n]);

        next
        //WriteObject(tStrings, 0 ,tempDate);
       //if(tempMessage[0] == 97) values[0] := tempval; // if message starts with 'a' 97 being ASCII for a
        // add morechecks here in the same format eg 'b600\r\n'
        tMcount := 0;

    p:endif



endfunc

func getSerialFrame()           //static...
    //print("S_Frame");

    var recbyte, n, g;
    index := 0;
    tMcount := 0;
    gotMessage := 0;
    //if (activeStatus !=0 )   //clear the graph
    for (n := 0; n<650; n++) // print new pixels - old pixels blacked out or graticule color depending on location
        //g := 425 - oldSample[n];
        //(g == 425 || g == 375 || g == 325 || g == 275 || g == 225 || g == 175 || g == 125) ?  gfx_PutPixel((50+n),g ,0x0280): gfx_PutPixel((50+n),g ,BLACK);
        //gfx_PutPixel((55+n),(425 - xTempSample[n]) ,YELLOW);
        gfx_Vline((50+n),(450 - xTempSample[n]),450,0x083C);   //
        gfx_PutPixel((50+n),450, 0x0280);
        gfx_PutPixel((50+n),400, 0x0280);
        gfx_PutPixel((50+n),350, 0x0280);
        gfx_PutPixel((50+n),300, 0x0280);
        gfx_PutPixel((50+n),250, 0x0280);
        gfx_PutPixel((50+n),200, 0x0280);
        gfx_PutPixel((50+n),150, 0x0280);
    next
    //endif
    while(com_Count() > 0)
        //if (processStatus != 1 && processStatus != 3) break;
        recbyte := serin();
        if(recbyte == 69) gotMessage := 1; // check for "E" and set message complete flag

        if(gotMessage == 0) // if message not complete keep building
            tempMessage[tMcount] := recbyte;
            tMcount++;
        endif

        if(gotMessage == 1)
            gotMessage := 0;
            //index := index+1;
            var multiplier;
            var tempval;
            multiplier := 1;
            tempval := 0;
            for(n := tMcount-1; n >= 0; n--)
                if(tempMessage[n] > 47 && tempMessage[n] < 58)
                    tempval := tempval + ((tempMessage[n] - 48) * multiplier);
                    multiplier := multiplier *10;
                endif
            next
            tMcount := 0;
            //if (activeStatus == 1 ) print("TV=", tempval,"\n");
            if (tempval > 0 && tempval<3500)
                xTempSample[index] := ((tempval/10))*2;    //80 faren=160
                oldSample[index] := xTempSample[index];

                gfx_Vline((50+index),(450 - xTempSample[index]),450,YELLOW);
                index := index+1;
            endif
            if (index == 650) break;
        endif
    wend
    //ledDigitsDisplay(tempval, iLeddigits0+1, 340, 4, 2, 30, 1) ;
endfunc


func getTemp()
    //print("getTemp");

    var recbyte, n, g;
    index := 0;
    tMcount := 0;
    gotMessage := 0;
    //print("Act_sta:", activeStatus,"ago_sta:", agoStatus, "\n");
    if (activeStatus != agoStatus) goto q;
    while(com_Count() > 0)
        if (activeStatus != agoStatus) break;
        recbyte := serin();
        if(recbyte == 97) gotMessage := 1; // check for "a" and set message complete flag
        //if(recbyte == 98) gotMessage := 2; // check for "b" and set message complete flag
        if(gotMessage == 0) // if message not complete keep building
            tempMessage[tMcount] := recbyte;
            tMcount++;
        endif
    q:wend
    if(gotMessage == 1)
        gotMessage := 0;
        //index := index+1;
        var multiplier;
        var tempval;
        multiplier := 1;
        tempval := 0;
        var kk := 0;
        var tempdate[10];

        gfx_RectangleFilled(324, 68, 800, 100, BLACK);
        txt_MoveCursor(4, 33);
        print("Date:");

        for(n := tMcount-1; n >= 0; n--)
            if (kk<4)
                if(tempMessage[n] > 47 && tempMessage[n] < 58)
                     tempval := tempval + ((tempMessage[n] - 48) * multiplier);
                     multiplier := multiplier *10;
                     //print("U:",tempMessage[n]);
                endif
            else
                if(tempMessage[n] > 47 && tempMessage[n] < 58)
                     tempdate[kk-4] := tempMessage[n] ;
                     //print("I am in forloop");
                     putchXY(695-(kk-4)*10, 80,tempdate[kk-4]);
                else
                     if (tempMessage[n] == 47) putchXY(695-(kk-4)*10, 80,'/');
                endif
            endif
            kk := kk + 1;
        next
        //print("tempnow:", tempval);
        WriteObject(tLeddigits, 0, tempval);   //Is it right?
        //txt_MoveCursor(2, 25);
        //print("tempdate:"   , tempdate[0],tempdate[1],' ',tempdate[2],tempdate[3],' ',tempdate[4],tempdate[5]);
        tMcount := 0;
    endif
endfunc
