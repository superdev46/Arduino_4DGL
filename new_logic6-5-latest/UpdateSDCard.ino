void updateSDcard(void)
{
  Serial.print("Trying to save in ");Serial.println("datalog1.txt");
  renewDateTime();
  File data_file = SD.open( "datalog1.txt", FILE_WRITE );
  delay(1500);

  if ( data_file ) 
  {
    data_file.print(ts.year);                                      // log incoming text message to setup file
    data_file.print(';');
    if (ts.month < 10)
    {
      data_file.print(0);data_file.print(ts.month);
    }
    else data_file.print(ts.month);
    data_file.print(";");
    if (ts.day < 10)
    {
      data_file.print(0);data_file.print(ts.day);
    }
    else data_file.print(ts.day);
    data_file.print(';');
    secTime = (long(ts.hour)*60*60)+(long(ts.minute)*60)+(long(ts.second));
    Serial.println(secTime);
    data_file.print(secTime);
    data_file.print(';');
    sprintf(value,"%04d",tVal);
    data_file.println(value);

    //data_file.print( ts.month ); data_file.print(F(",")); data_file.print( ts.day );data_file.print(F(","));data_file.print( 20);data_file.print( ts.year);data_file.print(F(","));     //data_file.print('\n');
    Serial.println( F("Saved temp to SD card") );
  }
  else 
  {                                                                   // if the file isn't open, pop up an error:
    Serial.println( F("Error opening setup file!!") );
  } 
  data_file.close();     
}
