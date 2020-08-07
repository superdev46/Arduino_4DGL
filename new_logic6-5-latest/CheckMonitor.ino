bool checkMonitor(void)
{
if ( Serial2.available() > 0 )
  {
    memset( ser_chars, '\0', SER_MDM_BUF_SIZE );
    int byte_cnt = 0;
    Serial.println(F("Characters found on monitor input"));
    while ( (Serial2.available() > 0) && (byte_cnt < SER_MDM_BUF_SIZE-1) )
    {
      ser_chars[byte_cnt] = Serial2.read() ;                            // read Serial data in positions on array ser_chars
      delay(1);
      byte_cnt++;
      Serial.print(ser_chars); Serial.println("  from lcd...");
    }
    if (strstr(ser_chars,"S0")!= NULL)
    {
      Serial.println("Uploading initial data to LCD");
      //ws = ts;
      loadDataInit(0);
      dataUpload = 1;
      sampleTimer=0;
      Serial.println("Uploading complete");
//      memset(tempString,'\0',256);
    }
    if (strstr(ser_chars, "Year=") !=NULL)
    {
      ptr1 = strstr(ser_chars,"=");
      ptr1 = ptr1 +1;
      ts.year = atoi(ptr1);
    }
    else if(strstr(ser_chars, "Month=") !=NULL)
    {
      ptr1 = strstr(ser_chars,"=");
      ptr1 = ptr1 +1;
      ts.month= atoi(ptr1);
    }
    else if(strstr(ser_chars, "Day=") !=NULL)
    {
      ptr1 = strstr(ser_chars,"=");
      ptr1 = ptr1 +1;
      ts.day = atoi(ptr1);
    }
    else if(strstr(ser_chars, "Hour=") !=NULL)
    {
      ptr1 = strstr(ser_chars,"=");
      ptr1 = ptr1 +1;
      ts.hour = atoi(ptr1);
    }
    else if(strstr(ser_chars, "Min=") !=NULL)
    {
      ptr1 = strstr(ser_chars,"=");
      ptr1 = ptr1 +1;
      ts.minute = atoi(ptr1);
    }
    else if (strstr(ser_chars, "Settime") !=NULL)
    {
      ts.second=0;
      setRTC();
      timeSet = true;
      ws = ts;        //when system time changes, displaying time will be chaneged, too.
    }
    else if (strstr(ser_chars, "Time?")!=NULL)
    {
      showTime();
    }
    else if (strstr(ser_chars, "SD") !=NULL)     //d  Show 24
    {
      newTemp = 0;
      currentState = 1;
      ptThreshold = 2;
      startDate = ws.day;
      endDate = ws.day + 1;
      seekVal = 21*4; Serial.println("Show 24:--");
      showDataPeriod(0);
      newTemp = 1;
    }
    else if (strstr(ser_chars, "SW") !=NULL)   ///Show week
    {
      newTemp = 0;
      currentState = 2;
      ptThreshold = 14;
      startDate = getSunday(ws);
      ws.day = startDate;
      endDate = startDate + 7;
      seekVal = 21*14;
      Serial.println("Show week:--");
      showDataPeriod(0);
      newTemp = 1;
    }
    else if (strstr(ser_chars, "SM") !=NULL)    //Show month
    {
      newTemp = 0;
      currentState = 3;
      ptThreshold = 61;
      startDate = ws.month; 
      endDate = 100;    //       In this case, endDate is not important
      ws.day = 1;
      seekVal = 21*14*31;      //interval for seeking
      Serial.println("Show month:---");
      showDataPeriod(0);
      newTemp = 1;
    }
    else if (strstr(ser_chars, "PV") !=NULL)
    {
      if ( currentState == 1 )
      {
        ws.day = ws.day - 1;
        if (ws.day < 1) ws.day += 1;
        startDate = ws.day; 
        endDate = startDate + 1;
        showDataPeriod(0);
      }
      else if ( currentState == 2 )
      {
        Serial.println("Im in week mode");Serial.print("ws.day = "); Serial.println(ws.day);
        dispCurrentDate();
       
        if (ws.day < 8) {        //if week is not full of 7 days. if ws.day<8 , ws.days- 7 =252
          startDate = 1;
          endDate = ws.day;
          ws.day = 1;
           Serial.print("This is startDate in week mode in case 1: ");Serial.println(startDate);
        } else {
          ws.day = ws.day - 7;  
          startDate = getSunday(ws);
          ws.day = startDate;
          Serial.print("This is startDate in week mode in case2:  ");Serial.println(startDate);
          endDate = startDate + 7;
        }
        showDataPeriod(0);       
      }
      else if ( currentState == 3 )
      {
        ws.month = ws.month - 1;
        if (ws.month < 1) ws.month = 1;
        startDate = ws.month;
        endDate = startDate + 1;
        Serial.print("This is startDate:              ");Serial.println(startDate);
        showDataPeriod(0);
      }
    }
    else if (strstr(ser_chars, "NX") !=NULL)
    {
      if ( currentState == 1 )
      {
        ws.day = ws.day + 1;
        if (ws.day > 31) ws.day -= 1;
        startDate = ws.day; 
        endDate = startDate + 1;
        showDataPeriod(0);
      }
      else if ( currentState == 2 )
      {
        ws.day = ws.day + 7;
        if (ws.day > 31) {        //if week is not full of 7 days.
          ws.day -= 7;
          startDate = getSunday(ws);
          ws.day = startDate;
          endDate = 32;
        } else {
          startDate = getSunday(ws);
          ws.day = startDate;
          endDate = startDate + 7;
        }
        showDataPeriod(0);      
      }
      else if ( currentState == 3 )
      {
        ws.month = ws.month + 1;
        if (ws.month > 12) ws.month = 12;
        startDate = ws.month;
        endDate = startDate + 1;
        Serial.print("This is startDate:              ");Serial.println(startDate);
        showDataPeriod(0);
      }
    }
    else if (strstr(ser_chars, "Resume") !=NULL)
    {
      currentState =0;
    }
  }
}
int getSunday(timeStampType tt) 
{
  int tempSun = tt.day - getDayOfWeek(tt.year, tt.month, tt.day);
  if (tempSun < 1) {
    tempSun = 1;
  }
  return tempSun;
}
void dispCurrentDate() {
    char datum[30];
    sprintf(datum, "%u/%u/%u ", ws.month, ws.day, ws.year);
    Serial.print("Temperature Date:");Serial.println(datum);
//    genie.WriteStr(3, datum);
}
