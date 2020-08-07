/*
void updateFileNames()
{
  int firstDay;
  int endDay;
*/

//********************************************HANDLE 24 Hour Files***********************************//
/*  if ( dayCounter == 1 )
  {
    sprintf( dayFileName, "day%02d%02d.log", ts.month,ts.day);//Creates a filename as dayMMDD
    updateSDcard(dayFileName);
    dayCounter=0;
  }
*/
//*******************************************HANDLE 12 hour files**********************************//
/*  
  if ( ts.hour < 12 )
  {
    sprintf(halfDayName,"AM%02d%02d.log",ts.month,ts.day);//Creates a filename as AMMMDD
    Serial.println(halfDayName);
    updateSDcard(halfDayName);
    dayCounter++;
    weekCounter++;
    monthCounter++;
  }
  else if ( ts.hour >= 12 )
  {
    sprintf(halfDayName,"PM%02d%02d.log",ts.month,ts.day);//Creates a filename as PMMMDD
    updateSDcard(halfDayName);
    dayCounter++;
    weekCounter++;
    monthCounter++;
  }

*/
//****************************************HANDLE Week files*************************************//
  /*  
  if ( fullWeekCounter == 651 )
  {
    firstDay = ts.day;
    endDay = ts.day + 7;    
    sprintf(wkFileName,"%02d%d%d.log",ts.month,firstDay,endDay);     //this wont work because file name will change every day
    fullWeekCounter = 0;
  }
  
  if ( weekCounter == 931 )
  {
     sprintf(wkFileName,"%02d%d%d.log",ts.month,firstDay,endDay);     //this wont work because file name will change every day
     updateSDcard(wkFileName);
     weekCounter = 0;
     fullWeekCounter++;
  }
*/
//***************************************HANDLE Month files*************************************//
/*
  if ( monthCounter == 4120 )
  {
    sprintf(mnthFileName,"Month%02d.log",ts.month);     //shows file name as MM(start)DD(end)DD
    updateSDcard(mnthFileName);
    monthCounter == 0;
  }
}
*/
