#define VERSION_INFO "  TLog Rev0.1"
///////////// REVISION HISTORY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//   For a release Version X.Y.Z: X --> Major upgrade in functionality
//                                Y --> Bug fix / minor upgrade
//                                Z --> pre-release value, used for distinguishing tested/corrected versions prior to release
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <Arduino.h>

#include <stdlib.h>
#include <string.h>
#include <Wire.h>
#include <EEPROM.h>
#include <avr/interrupt.h>
#include <SD.h>
#include <SPI.h>
#include <TimerThree.h>
#include <OneWire.h>

#include <avr/wdt.h>             //  Uses WDT to reset processor
#include <avr/pgmspace.h>


#define SEND_TEMP_TIME  100    //number of seconds in 10mS increment to send data to LCD
#define LOG_DATA_TIME   6643    

#define FRAME_SIZE 650
#define EE_BASE_ADDR 0x00
#define DS1307_I2C_ADDRESS  0x68  // This is the I2C address
#define I2C_WRITE           Wire.write 
#define I2C_READ            Wire.read

#define SETUP_FILE           "datog1.txt"
//#define FILL_LOG_FILE        "fill.log"
#define BILL_LOG_FILENAME    "bill.log"
#define EVENT_LOG_FILENAME   "event.log"
//#define ALERT_LOG_FILENAME   "alert.log"
#define SER_MDM_BUF_SIZE      128

#define OFF              false     // defines OFF to be false (0)
#define ON               true      // defines ON to be true (1)
#define ON_OFF           -1        // defines OFF to be -1 (neither true, nor false... for signed data type)
#define OPEN             false     // defines OPEN to be false (0)
#define CLOSE            true      // defines CLOSE to be true (1)

#define LED_OFF          LOW
#define LED_ON           HIGH

#define LCD_RESET 6

char *ptr1, *ptr2, *ptrStart;
int cYear;
int cMonth;
int cDay;
int cHour;
int cMin;
char ser_chars[128];
int updateTimer;
int buttonMove;

//***************************State Variables***************************//
//12 hour state = 0
//24 hour state = 1
//7  day state = 2
//1  month state = 3

  int currentState = 0;
  int ptThreshold;
  int ptCount = 0;                                               
  
  int checkDay;
  int checkMonth;
//***************************fileName variables************************//
  int dayCounter = 0;
  int weekCounter = 0;
  int monthCounter = 0;
  int fullWeekCounter = 0;
  char halfDayName[13];
  char dayFileName[13];
  char wkFileName[13];
  char mnthFileName[13];
  int startPlace;
  int newTemp = 0;


OneWire  ds(35);  // on pin 16 (a 4.7K resistor is necessary)

// EEPROM VARS ////////////////////////////////////////////////////////////////////////////////////////////////////
//   Variable stored into EEPROM will now be stored contiguously by using this structure.
//   This will save space, increase maintainability, allow for easy addition of new vars.
//   Greatly simplifies global EEPROM storage and retrieval at startup
//   
struct EE_TYPE {
  float temperature;

};
EE_TYPE ee;

char value[6];
int tVal;

struct timeStampType {              // consolidation of bytes to hold date/time
  byte second;
  byte minute;
  byte hour;
  byte wkDay;                       // day of week... Sun = 0, Mon = 1, etc.
  byte day;
  byte month;
  byte year;
};

timeStampType ts;                   // Instance of timestamp type
timeStampType ws;                   //Instance of timestamp saving current displaying date

char time_str[9];
char date_str[20];

float celsiusAverage;
float cTemperature;
float fahrenheit;
float celsius;
long secTime;
byte i;
byte data[12];
long sampleTimer;
char tempString[256];
char serIn;
byte index;
byte dataUpload;
char dataString[22];
int dataFrame1[650];
int startDate = ts.day;
int endDate;
long seekVal = 0;
int offSeek = 65;
bool timeSet = false;                                   //determines if time has been set


//********************************************** PROGRAM START **********************************************************
//**********************************************************************************
//* Function setup() 
//*   is called first!
//*
//**********************************************************************************
void setup() 
{
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);

  pinMode(LCD_RESET,OUTPUT);
  digitalWrite(LCD_RESET,LOW);  //pull reset low to hold LCD in reset
  SerialPortInit();                                 // Initializes modem and debug serial ports

  Wire.begin();
  SD_CardInit();                      // Verifies SD card ready, sets global

  //******************** System Timer Interrupt Initialization ***************************
  Timer3.initialize(10000);                         // initialize timer3, and set a 0.01 second period
  Timer3.attachInterrupt(systemTimer);

  
  renewDateTime();                                  // Update time/date globals from RTC
  ws = ts;                                          //intialize diplaying time (current time)
  memset(tempString,'\0',256);

  digitalWrite(LCD_RESET,HIGH);  //pull reset high to release LCD from reset

} // END setup


void loop() 
{

  cTemperature = getTemperature();
  //if ( ts.year != 165 && ts.month != 165 && ts.day !=165 && ts.hour !=45 && ts.minute !=165)
  
  if ( updateTimer >= LOG_DATA_TIME )   //10ms * 6643 = 66430 /1000 = 66s = 1min
  {
    Serial.println("Logging to SD card.");
    updateSDcard();
    updateTimer = 0;
  }
  
  if (cTemperature != 0) {
    fahrenheit = (1.8 * cTemperature) + 32;  
//    Serial.print("Fahrenheit: ");
//    Serial.println(fahrenheit);
    tVal = (int)(fahrenheit*10);
    memset(value,'\0',6);
    index = 0;
  }
  
  checkMonitor();
   
//  while (Serial2.available())
//  {
//    tempString[index++] = Serial2.read();
//    Serial.println(tempString);
//    if (strstr(tempString,"S0")!= NULL)
//    {
//      Serial.println("Uploading initial data to LCD");
//      loadDataInit(0);
//      dataUpload = 1;
//      sampleTimer=0;
//      Serial.println("Uploading complete");
//      memset(tempString,'\0',256);
//    }
//  } 

  if  ( currentState >= 0 )
  {
    
    if (dataUpload == 1 && sampleTimer >= SEND_TEMP_TIME)//send new sample to screen every x seconds in 10mS increments
    {
      delay(100);
      if (currentState == 0) {
          sprintf(value,"%02d/%02d/%02d%04da", ts.month,ts.day, ts.year,tVal);
      } else {
          sprintf(value,"%02d/%02d/%02d%04da", ws.month, ws.day, ws.year,tVal);
      }
      
      Serial.print("data sent");
      Serial.println(value);
      Serial2.print(value);
      char tempdate[10];
      //sprintf(tempdate, "%02d/%02d/%02db", ws.month, ws.day, ws.year);
      //delay(300);
      //Serial2.print(tempdate);
     // Serial.println(tempdate);
      //delay(100);
      sampleTimer=0;
    }
  }
 
} // END loop 

void loadDataInit(byte page)
{
 Serial.println("Entering loadDataInit");
 char iData[5];
 memset (iData,'\0',5);
 int index1;
 long fileSize;
 File dataFile = SD.open( "datalog1.txt" );
 fileSize = dataFile.size();
 Serial.print("This is the file size: "); Serial.println(fileSize);
 Serial.print("File size = ");Serial.println(fileSize);

  index = 0;
  index1 = 0;
  memset(dataString,'\0',22);

  if (page == 0)
  {
    dataFile.seek(fileSize - FRAME_SIZE * 21);
  }
  else
  {
    dataFile.seek(fileSize - FRAME_SIZE * 21 * page);
  }
  
  
  if (dataFile) 
  {
    while (dataFile.available()) 
    {
      serIn = dataFile.read();
      if (serIn == '\n')
      {
        //strncpy(iData,dataString[15],4);
        iData[0]=(char)dataString[15];
        iData[1]=(char)dataString[16];
        iData[2]=(char)dataString[17];
        iData[3]=(char)dataString[18];
        iData[4]='\0';        
        dataFrame1[index1++]= atoi(iData);
        if (index1 >= FRAME_SIZE)break;  // break after loading last file instead of waiting for end of file.
        index=0;
      }
      else
      {
        if (serIn != '\n' || serIn != '\r'  )
        {
          dataString[index++] = serIn;
        }
      }
  
    }
    dataFile.close();
    Serial.println(index1);
    Serial.println(650-index1);
    for (int x=0; x <= (650 - index1); x++)
    {
      sprintf(value,"0000E",dataFrame1[x]);
      Serial2.print(value);
      //Serial.println(value);
      delay(10);
    }
    for (int x=0;x<650;x++)
    {
      sprintf(value,"%04dE",dataFrame1[x]);
      Serial2.print(value);
      Serial.print("init data frame:");Serial.println(value);
      delay(10);
    }
    
  }
  // if the file isn't open, pop up an error:
  else 
  {
    Serial.println("error opening datalog1.txt");
  }
  delay(1000);
  
}
 
float getTemperature()
{
  ds.reset();
 // ds.select(addr);
  ds.write(0xCC);
  ds.write(0x44, 1);        // start conversion
  delay(1000);             //12 bit conversion takes about 750mS 
  ds.reset();
  //ds.select(addr);        //set address 
  ds.write(0xCC);
  ds.write(0xBE);         // Read data
  for ( i = 0; i < 9; i++)
  {          
    data[i] = ds.read(); //read 9 bytes
  }
  int16_t raw = (data[1] << 8) | data[0];
  byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
 
  celsius = (float)raw / 16.0;
  return celsius;

}



//***************************** SYSTEM TIMER INTERRUPT ROUTINE **********************
void systemTimer(void)      //10mS interrupt
{
  sampleTimer++;
  updateTimer++;
  
} // END systemTimer


//******************************************** RETRIEVE DATE & TIME FROM RTC *********************************
void renewDateTime( void )                                 //    gets time from RTC and fills in date/time struct
{
 //             Serial.println(F("                                                                in renewDateTime()"));
  //showTime();
  const byte zero = 0;
  Wire.beginTransmission( DS1307_I2C_ADDRESS );            //    Reset the register pointer
  I2C_WRITE( zero );
  Wire.endTransmission();
  Wire.requestFrom( DS1307_I2C_ADDRESS, 7 );

  //    A few of these need masks because certain bits are control bits
  ts.second  = bcdToDec( I2C_READ() & 0x7f ); delay(1);
  ts.minute  = bcdToDec( I2C_READ() );        delay(1);
  ts.hour    = bcdToDec( I2C_READ() & 0x3f ); delay(1);    // Need to change this if 12 hour am/pm
               bcdToDec( I2C_READ() );        delay(1);    // Need to read this out, but not use it...
  ts.day     = bcdToDec( I2C_READ() );        delay(1);
  ts.month   = bcdToDec( I2C_READ() );        delay(1);
  ts.year    = bcdToDec( I2C_READ() );        delay(1);

  ts.wkDay   = getDayOfWeek( ts.year, ts.month, ts.day );  // RTC chip dont seem to work!!  So use conversion always

/*
Serial.print("Data extracted from RTC chip");
Serial.print(ts.second); Serial.print(F(", "));
Serial.print(ts.minute); Serial.print(F(", "));
Serial.print(ts.hour);   Serial.print(F(", "));
Serial.print(ts.day);    Serial.print(F(", "));
Serial.print(ts.month);  Serial.print(F(", "));
Serial.print(ts.year);   Serial.print(F(", "));
Serial.print(ts.wkDay);  Serial.println();
*/
  //showTime();
} // END renewDateTime




// DO NOT USE Macro - calling fn has a call that should be used twice...
byte bcdToDec( byte val )      // Convert binary coded decimal to normal decimal numbers
{
  return ((val / 16 * 10) + (val % 16));
}
//rod: *********************************** getTimeAfterDelay *****************************
//this function calculates what time it will be after a lag period 






//******************************* Calc. day of week from date **********************************
byte getDayOfWeek(int y, int m, int d) 
{      /* 0 = Sunday */
 //                                         Serial.println(F("                                                                in getDayOfWeek()"));

  const int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
  y -= m < 3;

  return ( y + y/4 - y/100 + y/400 + t[m-1] + d ) % 7;    // returns 0 - 6

} // END getDayOfWeek


//***************************************** SET RTC *************************************
void setRTC(void) 
{
 //                                           Serial.println(F("                                                                in setRTC()"));

  const byte zero = 0;

  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  I2C_WRITE( zero );
  I2C_WRITE( decToBcd(ts.second) & 0x7f );    // 0 to bit 7 starts the clock
  I2C_WRITE( decToBcd(ts.minute) );
  I2C_WRITE( decToBcd(ts.hour) );             // If you want 12 hour am/pm you need to set bit 6 (also need to change readDateDs1307)
  I2C_WRITE( decToBcd(ts.wkDay) );
  I2C_WRITE( decToBcd(ts.day) );
  I2C_WRITE( decToBcd(ts.month) );
  I2C_WRITE( decToBcd(ts.year) );
  Wire.endTransmission();
  showTime();
} // END setRTC


// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val) 
{
  return ((val / 10 * 16) + (val % 10));
}



//*********************************************************************************
//* Function writeEEValues()
//*   Transfers data from SRAM to EEPROM
//*********************************************************************************
bool writeEEValues(void* val, int num_of_bytes)
{

  bool wrOK = true;

  int ee_addr = (int)( (long)EE_BASE_ADDR + ((long)val - (long)&ee) );  // calculate EE address from relative SRAM addressses
  for (int i = 0; i < num_of_bytes; i++) {                              // loop thru each byte
    EEPROM.write( ee_addr + i, *(char*)( (long)val + (long)i ) );       // read each byte from SRAM to EEPROM
    delay(1);   // delay 1ms between writes                             // delay 1ms between reads
  }

  return wrOK;

} // END writeEEValues


//*********************************************************************************
//* Function readEEValues()
//*   Transfers data from EEPROM to SRAM
//*********************************************************************************
bool readEEValues(void* val, int num_of_bytes)
{
 

  bool rdOK = true;

  int ee_addr = (int)( (long)EE_BASE_ADDR + ((long)val - (long)&ee) );   // calculate EE address from relative SRAM addressses
  for (int i = 0; i < num_of_bytes; i++) {                               // loop thru each byte
    *(char*)( (long)val + (long)i ) = EEPROM.read( ee_addr + i );        // read each byte from EEPROM to SRAM
    delay(1);                                                            // delay 1ms between reads
  }

  return rdOK;

} // END readEEValues



//**********************************************************************************
//* Function SerialPortInit()
//*  Verifies serial ports are alive and reports baud rates
//**********************************************************************************
void SerialPortInit(void) 
{

  // initialize both serial ports:
  Serial.begin(115200);
  Serial2.begin(115200);
/*
  if ( Serial && Serial3 ){
    printFBVersion();
    Serial.println( F("Serial Ports Initialized") );
  } else {
    Serial.println( F("\r\nSerial Ports Failure") );
  }

  Serial.print( F("  Serial Port Tx Buffer Size = ") );
  Serial.println( SERIAL_TX_BUFFER_SIZE, DEC );
  Serial.print( F("  Serial Port Rx Buffer Size = ") );
  Serial.println( SERIAL_RX_BUFFER_SIZE, DEC );
  Serial.println();
  */
} // END SerialPortInit


//**********************************************************************************
//* Function printFBVersion()
//*  Prints out version info. to the debug port.
//**********************************************************************************
void printFBVersion(void) 
{
  // Send debug message at startup...
    Serial.println(F("\r\n*****************************************"));
    Serial.println(F("  TEMPERATURE LOGGER")); 
    Serial.println(F(VERSION_INFO));
    Serial.println(F("*****************************************\r\n"));

} // END printFBVersion


//**********************************************************************************
//*  Function SD_CardInit()
//*    Starts SD card, sets global with results for use with SD card functions
//**********************************************************************************
bool SD_CardInit(void) 
{

  Serial.print(F("\r\nInitializing SD card..."));
  // Set default CS pin for SD to OUTPUT
//  pinMode(CHIP_SELECT_DEFAULT, OUTPUT);          // Used by FootBath HW... do not set!!

  // see if the card is present and can be initialized:
  if (!SD.begin(53)) {
    Serial.println(F("Card failed, or not present\r\n"));
    // don't do anything more:
    return false;
  } else {
    Serial.println(F("card initialized.\r\n"));
    return true;
  }

} // END SD_CardInit

//    while ( !Serial3.available() ){  // Wait until something gets into buffer
//      if (i-- == 0) break;           //  After 45 sec. - Fail
//      delay(1000);
//    }
