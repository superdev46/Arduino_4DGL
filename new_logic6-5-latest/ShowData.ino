
void showDataPeriod(byte page)
{
     Serial.println("Entering showDataPeriod");
     char iData[5];                                                         //Use to hold temperature info to be passed to LCD
     char dData[2];                                                         //Used to hold date data to manage periods of times requests
     char tData[5];    
     char mData[2];
     //int checkTime = 0; 
     memset (iData,'\0',5);
     int index1;
     long fileSize;
     int timeCounter = 0;                                                  //Used to control only getting the time once during the while loop
     File dataFile = SD.open( "datalog1.txt" );
     fileSize = dataFile.size();
     Serial.print("File size = ");Serial.println(fileSize);
  
     index = 0;
     index1 = 0;
     memset(dataString,'\0',22);
    
     if (dataFile) 
     {    
        dataFile.seek(fileSize - FRAME_SIZE * seekVal);
        while (dataFile.available()) 
        {
        
    //      dataFile.seek(fileSize-(seekVal)*22-offSeek);
            mData[0] = (char)dataString[3];
            mData[1] = (char)dataString[4];
            mData[2] = '\0';
            checkMonth = atoi(mData);
            serIn = dataFile.read(); 
            if (serIn == '\n')
            {
                if ( currentState == 1 )                                               //For getting date for 24 hour or week check
                { 
                  dData[0] = (char)dataString[6];                                       //grabs date from data file lines
                  dData[1] = (char)dataString[7];
                  checkDay = atoi(dData);                                               //converts date to integer
                  Serial.print("checkDay:");Serial.println(checkDay);
                  Serial.print("dataString:");Serial.println(dataString);
                }
                else if ( currentState == 2 )                                         //Gets the month from the month check
                {
                  dData[0] = (char)dataString[6];                                      //grabs month from data file lines
                  dData[1] = (char)dataString[7];
                  checkDay = atoi(dData);                                             //converts month to integer
                  Serial.print("checkday:"); Serial.println(checkDay);
                }
                else if ( currentState == 3 )                                         //Gets the month from the month check
                {
                  dData[0] = (char)dataString[3];                                      //grabs month from data file lines
                  dData[1] = (char)dataString[4];
                  checkDay = atoi(dData);                                            //converts month to integer
                  //Serial.println(checkDay);
                  //Serial.println(dataString);
                }
                
                Serial.print(checkDay);Serial.print(" vs ");Serial.println(startDate);
                if ( timeCounter == 0 && currentState == 1 && checkDay == startDate && checkMonth == ws.month)                      //used to find data start point out of 650 for 24 hour mode
                { 
                    Serial.print("Im 24hr------");
                    Serial.print("valid datastring---:");Serial.println(dataString);
                    tData[0] = (char)dataString[9];
                    tData[1] = (char)dataString[10];
                    tData[2] = (char)dataString[11];
                    tData[3] = (char)dataString[12];
                    tData[4] = (char)dataString[13];
                    tData[5] = '\0';
                    
                    long checkTime = atol(tData);            
                    Serial.print("Did checkTime get populated correctly? ");Serial.println(checkTime);
                    startPlace =  ( checkTime ) / (86400 / 650);                                          //startPlace determines where the first found point should appear on screen            
                    Serial.print("startPlace="); Serial.println(startPlace);
                  timeCounter++;           
                }  
                if ( timeCounter == 0  && currentState == 2 && checkDay >= startDate && checkDay < endDate && checkMonth == ws.month)                      //used to find data start point out of 650 for 24 hour mode
                { 
                    Serial.print("Im week------");
                    Serial.print("valid datastring---:");Serial.println(dataString);
                    tData[0] = (char)dataString[9];
                    tData[1] = (char)dataString[10];
                    tData[2] = (char)dataString[11];
                    tData[3] = (char)dataString[12];
                    tData[4] = (char)dataString[13];
                    tData[5] = '\0';
                  
                    long checkTime = atol(tData);            
                    Serial.print("Did checkTime get populated correctly? ");Serial.println(checkTime);
                    startPlace =  ( checkTime ) / (86400*7 / 650);                                          //startPlace determines where the first found point should appear on screen            
                    Serial.print("startPlace=");Serial.println(startPlace);
                    timeCounter++;        
                 } 
                if ( timeCounter == 0  && checkDay == startDate && currentState == 3 && checkMonth == ws.month)                      //used to find data start point out of 650 for 24 hour mode
                { 
                    Serial.print("Im Month---------");
                    Serial.print("valid datastring---:");Serial.println(dataString);
                    tData[0] = (char)dataString[9];
                    tData[1] = (char)dataString[10];
                    tData[2] = (char)dataString[11];
                    tData[3] = (char)dataString[12];
                    tData[4] = (char)dataString[13];
                    tData[5] = '\0';
                  
        //          long tDataBig = atoi(tData[0])*10000;                                                 //This ensures that when the seconds counter reaches above unsigned int max, it can use a long
        //          int checkTimeHold = atoi(tData[1-4]);
        //          long checkTime = checkTimeHold + tDataBig; 
                     long checkTime = atol(tData);          
                     Serial.print("Did checkTime get populated correctly? ");Serial.println(checkTime);
                     startPlace =  ( checkTime ) / (86400*30 / 650);                                          //startPlace determines where the first found point should appear on screen            
                     Serial.print("startPlace="); Serial.println(startPlace);
                     timeCounter++;           
                } 
                index=0;
                ptCount++;           //controls how many lines of data to skip before grabbing data
                if ( checkDay >= startDate && checkDay < endDate && ptCount >= ptThreshold && checkMonth == ws.month)
                {
                    //strncpy(iData,dataString[15],4);
                    Serial.print("valid group:---");Serial.println(dataString);
                    iData[0]=(char)dataString[15];
                    iData[1]=(char)dataString[16];
                    iData[2]=(char)dataString[17];
                    iData[3]=(char)dataString[18]; 
                    iData[4]='\0';        
                    Serial.print("The data in iData");Serial.println(iData);
                    dataFrame1[index1++]= atoi(iData);
                    if (index1 >= FRAME_SIZE) break;                                   // break after loading last file instead of waiting for end of file.
                    //index=0;
                    ptCount=0;                                                        //resets the line skipper 
                  }                
                }
          else
          {
            if (serIn != '\n' || serIn != '\r'  )
            {
              dataString[index++] = serIn;
            }
          }
      }
      int pCounter = 0;
      dataFile.close();
      //Serial.print("This is start place: "); Serial.println(startPlace);
      for (int x=0; x < startPlace; x++)                                                        //Fills in leading time points as zeroes until reachin the startPlace
      {
        //sprintf(value,"%04dE", dataFrame1[x]);
        sprintf(value, "0000E", dataFrame1[x]);
        Serial.println("OUTPUT:11111-------------");Serial.println(value); 
        Serial2.print(value);
        delay(10);
      }
      pCounter = startPlace + 1;
      Serial.print("pCounter::::");Serial.println(pCounter);
      Serial.print(pCounter);Serial.print(" vs ");Serial.print(startPlace);
  //    for ( pCounter; pCounter <= ( startPlace * 2 ) || pCounter == 650; pCounter++ )         //Fills in the data points
  //    {
  //      sprintf(value,"%04dE", dataFrame1[pCounter]);
  //      //sprintf( value, 0000, dataFrame1[x]);
  //      Serial2.print(value);
  //      delay(10);
  //    }
      int k = 0, tempInt;
      for ( pCounter; pCounter <= 650; pCounter++ )                                          //Fills in trailing points with zeroes. 
      {
          Serial.print("dataFrame1:::"); Serial.println(dataFrame1[pCounter]);
          if (k < index1) {
            sprintf(value,"%04dE", dataFrame1[k]);  // in our case, all values are 0..why?in May
            tempInt =  dataFrame1[k]/10;
          } else {
            sprintf(value,"0000E", dataFrame1[0]); 
            tempInt = 0;
          }
          k++;
          Serial.print("OUTPUT:333::");Serial.println(value); 
          Serial2.print(value); 
          delay(10);
      }
      
       Serial.println("showing data-1 period- ended");
      
    }
    // if the file isn't open, pop up an error:
    else 
    {
        Serial.println("error opening datalog1.txt");
    }
    delay(1000);
}
