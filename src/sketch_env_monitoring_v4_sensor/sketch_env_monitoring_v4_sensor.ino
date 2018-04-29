// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Arduino Software Version v1.6.12
// Arduino Board UNO
//
//  Important
//  coz BME280 connect to SCL and SDA pin, arduino with TFT screen must be power on. Otherwise BME280 block to begin.
//
// Sensors:
//  1. Data logging shield (SD Card)
//  2. PMS7003 (PM2.5 / PM1.0 / PM10.0)
//  3. BME280
//  4. Relay
//  5. MQ-5 (LPG sensor)
//  6. MQ-135 (CO2 sensor)
//  7. DS3231 Clock
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Program Specification
//    1. setup()
//        initialize all sensors SD Card, Clock, PMS7003, BME280
//    2. loop()
//        a. Read data from BME280 (for temperature, humidity, pressure)
//        b. Read data from PMS7003 (for PM2.5, PM1.0, PM10.0)
//        c. Write to SD Card
//    Problem Solving History:
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------
//  Software Configuration (BEGIN)
// ------------------------------------------
#define SERIAL_BAUD_RATE 9600
 
#define READING_SENSOR_INTERVAL 800     // Interval to read ALL sensors. 10s interval.
#define ENABLE_SD_CARD 1                 // 1: Enable SD Card, 0; Disable.
#define SD_CARD_FILENAME "MONITOR.TXT"    // Filename "FAT" 8.3 format (XXXXXXXX.XXX) You must use the 8.3 format, so that file names look like AME001.EXT, where AME001 is an 8 character or fewer string, and is a 3 character extension.
// SD Card must be formatted as the "FAT" filesystem first.
// ------------------------------------------
//  Software Configuration (END)
// ------------------------------------------


// ------------------------------------------
//  PIN Configuration (BEGIN)
// ------------------------------------------
// PMS7003 (PM2.5 / PM1.0 / PM10.0) PIN:
#define PMS7003_TXD_PIN A15
#define PMS7003_RXD_PIN A14
//  PMS7003     Arduino(UNO)
//  TXD         Pin A15 (PMS7003_TXD_PIN)
//  RXD         Pin A14 (PMS7003_RXD_PIN)

// BME280 PIN:
//  BME280     Arduino(UNO)
//  SCL         SCL (Pin 21) 
//  SDA         SDA (Pin 20)

////  BME280     Arduino(UNO)
////  SCL         7 
////  SDC         6 
////  SDA         5
////  CSB         4
//#define BME_SCK 7 // Serial Clock   SCL
//#define BME_MISO 6// Serial Data Out SDC 
//#define BME_MOSI 5 // Serial Data In  SDA
//#define BME_CS 4 // Chip Select CSB      

// Clock PIN:
// Data logging shield    Arduino
// SDA                    20
// SCL                    21
// Remark: SDA and SCL pin is different from normal, it locates in "5V GND SDA SCL".
              
// SD Card PIN:
#define SD_CARD_CS_PIN 10  // CS
#define SD_CARD_MOSI_PIN 11
#define SD_CARD_MISO_PIN 12
#define SD_CARD_CLK_PIN 13
//  SD Card Module     Arduino(UNO)
//  CS                 PIN10
//  MOSI               PIN11
//  MISO               PIN12
//  CLK                PIN13

// Arduino To Arduino PIN:
// Arduino_1  Arduino_2
// SCL        SCL
// SDA        SDA
// GND        GND

// Relay
#define RELAY_PIN 4

// MQ-5 (LPG sensor)
#define MQ5_LPG_PIN A3

// MQ-135 (CO2 sensor)
#define MQ135_CO2_PIN A1


// DS3231 Clock   Arduino
// 6              SCL
// 7              SDA
#define DS3231_CLOCK_SDA 7
#define DS3231_CLOCK_SCL 6

// ------------------------------------------
//  PIN Configuration (END)
// ------------------------------------------

// MQ-5 LPG Configuration (BEGIN)
unsigned int mq5_lpg = 0;
// MQ-5 LPG Configuration (END)

// MQ-135 CO2 Configuration (BEGIN)
#include "MQ135.h"
// Modify RZERO value in C:\Users\vfire\Documents\Arduino\libraries\MQ135\MQ135.h
//#define RZERO 6619 //  Define RZERO Calibration Value
MQ135 mq135Sensor = MQ135(MQ135_CO2_PIN);
float mq135_co2 = 0;
// MQ-135 CO2 Configuration (END)

// BME280 Configuration (BEGIN)
#include "cactus_io_BME280_I2C.h"
BME280_I2C bme280(0x76);  // I2C using address 0x76
//#include <SPI.h> 
//#include "cactus_io_BME280_SPI.h" 
//BME280_SPI bme280(BME_CS,BME_MOSI,BME_MISO,BME_SCK); // Using Software SPI 
float temperatureC = 0;
float temperatureF = 0;
float humidityPer = 0;
float pressure = 0;
boolean isSuccesToInitBME = false;  // true: Succes to initialize BME280.
// Pin: SDA, SCL
// BME280 Configuration (END)

// PMS7003 Configuration (BEGIN)
//https://github.com/MartyMacGyver/PMS7003-on-Particle/blob/master/pms7003-photon-demo-1/pms7003-photon-demo-1.ino
//https://www.taiwaniot.com.tw/shop/module-sensor/%E7%A9%BA%E6%B0%A3%E7%B2%89%E5%A1%B5-pm2-5/%E6%94%80%E8%97%A4-g7-pms7003-pm1-0-pm2-5-pm10-%E7%B2%89%E5%A1%B5%E6%BF%83%E5%BA%A6%E6%84%9F%E6%B8%AC%E5%99%A8/
#include <SoftwareSerial.h>
#define START_1 0x42
#define START_2 0x4d
//-----------------------
#define DATA_LENGTH_H        0
#define DATA_LENGTH_L        1
#define PM1_0_CF1_H          2
#define PM1_0_CF1_L          3
#define PM2_5_CF1_H          4
#define PM2_5_CF1_L          5
#define PM10_CF1_H           6
#define PM10_CF1_L           7
#define PM1_0_ATMOSPHERE_H   8
#define PM1_0_ATMOSPHERE_L   9
#define PM2_5_ATMOSPHERE_H   10
#define PM2_5_ATMOSPHERE_L   11
#define PM10_ATMOSPHERE_H    12
#define PM10_ATMOSPHERE_L    13
#define UM0_3_H              14
#define UM0_3_L              15
#define UM0_5_H              16
#define UM0_5_L              17
#define UM1_0_H              18
#define UM1_0_L              19
#define UM2_5_H              20
#define UM2_5_L              21
#define UM5_0_H              22
#define UM5_0_L              23
#define UM10_H               24
#define UM10_L               25
#define VERSION              26
#define ERROR_CODE           27
#define CHECKSUM             29
unsigned char pms7003ChrRecv;
byte bytCount = 0;
SoftwareSerial myPMS7003(PMS7003_TXD_PIN, PMS7003_RXD_PIN);

unsigned int pm_1 = 0;
unsigned int pm_2_5 = 0;
unsigned int pm_10 = 0;
// PMS7003 Configuration (END)


// Clock Configuration (BEGIN)
//#include <Wire.h>
//#include <DS1307RTC.h>
//int isClockOk = true;

#include <DS3231.h>
//DS3231  rtc(SDA, SCL);
DS3231 rtc(DS3231_CLOCK_SDA, DS3231_CLOCK_SCL);
Time timeDS3231;
int isClockOk = true;
// Clock Configuration (END)

// SD Card Module Configuration (BEGIN)
//#include <SPI.h>
//#include <SD.h>


#include "SdFat.h"
//SdFat sd;
//SdFile sdCardFile;
SdFatSoftSpi<SD_CARD_MISO_PIN, SD_CARD_MOSI_PIN, SD_CARD_CLK_PIN> SD;
//File sdCardFile;
//#include <SD.h>

boolean isSuccesToInitSD = false;  // true: Succes to initialize SD Card.
//#define SD_FILE_DATA_HEADER "YYYY/MM/DD HH:MN:SS\tTEMP\tHUM\tBARO\tPM2.5\tPM10\tPM1.0"
#define SD_FILE_DATA_HEADER "YYYY/MM/DD HH:MN:SS\tTEMP\tHUM\tBARO\tLPG\tCO2\tPM2.5\tPM10\tPM1.0"
// SD Card Module Configuration (END)

// Arduino To Arduino Configuration (BEGIN)
#include <Wire.h>
#include <EasyTransferI2C.h>
EasyTransferI2C arduinoConnect; 
struct ARDUINO_CONNECT_DATA_STRUCTURE{
  float temperatureC = 0;
  float humidityPer = 0;
  uint16_t pressure = 0;
  uint16_t pm_1 = 0;
  uint16_t pm_2_5 = 0;
  uint16_t pm_10 = 0;
  uint16_t clockYear =0;
  uint8_t clockMonth =0;
  uint8_t clockDay = 0;
  uint8_t clockHour = 0;
  uint8_t clockMinute = 0;
  uint8_t clockSeconds = 0;
  uint16_t mq135_co2 = 0;
  uint16_t mq5_lpg = 0;
  uint8_t isSuccesToInitSD = 0;
};
//give a name to the group of data
ARDUINO_CONNECT_DATA_STRUCTURE arduinoData;
//define slave i2c address
#define I2C_SLAVE_ADDRESS 9
// Arduino To Arduino Configuration (END)

// Software variables(BEGIN)
#define FIRST_LOOP_DELAY 2000  // delay seconds to wait for PMS3003 sensor on first-loop.
boolean firstLoop = true;
boolean writeSDHeader = false;
#define TIME_STR_SIZE 20
#define TEMPERATURE_STR_SIZE 5
#define HUMIDITY_STR_SIZE 5
#define PRESSURE_STR_SIZE 5
#define MQ5_LPG_STR_SIZE 5
#define MQ135_CO2_STR_SIZE 5

char timeStr[TIME_STR_SIZE];
char temperatureStr[TEMPERATURE_STR_SIZE];
char humidityStr[HUMIDITY_STR_SIZE];
char pressureStr[PRESSURE_STR_SIZE];
char mq5LpgStr[MQ5_LPG_STR_SIZE];
char mq135Co2Str[MQ135_CO2_STR_SIZE];

#define MQ5_LPG_MAX_VALUE 9999 // sync with MQ5_LPG_STR_SIZE
#define MQ135_CO2_MAX_VALUE 9999 // sync with MQ135_CO2_STR_SIZE
// Software variables (BEGIN)

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                    FUNCTIONS
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  Serial.println("Setup[Relay]");
  initRelay();

  Serial.println("Setup[MQ5]");
  initMQ5();

  Serial.println("Setup[MQ135]");
  initMQ135();

  Serial.println("Setup[BME]");
  initBME280();

  Serial.println("Setup[PMS7003]");
  initPMS7003();

  Serial.println("Setup[Clock]");
  initClock();

  Serial.println("Setup[SD]");
  initSDCard();

  Serial.println("Setup Arduino Connect Arduino");
  initArduinoConnect();
  
 //writeClock();

  firstLoop = true;
  Serial.println("Setup[Done]");
}


void loop() { // put your main code here, to run repeatedly:

  pm_1 = 0;
  pm_2_5 = 0;
  pm_10 = 0;
  temperatureC = 0;
  temperatureF = 0;
  humidityPer = 0;
  pressure = 0;
  mq5_lpg=0;
  mq135_co2=0;


  Serial.println("Reading PMS7003");
  readPMS7003();

  Serial.println("Reading BME");
  if ( isSuccesToInitBME ) {
    readBME280();
  }
  
  readMQ5();
  readMQ135();
  
  if ( !firstLoop){ // 2016.11.11: Rule of Thumb: dont log first data, which it is often inaccurate.
    writeToSDCard();
  }


   sendArduinConnect();
  Serial.print("PM1.0:");
  Serial.println(pm_1);
  Serial.print("PM2.5:");
  Serial.println(pm_2_5);
  Serial.print("PM10.0:");
  Serial.println(pm_10);
  Serial.print("Temperature:");
  Serial.println(temperatureC);
  Serial.print("Humidity:");
  Serial.println(humidityPer);
  Serial.print("Pressure:");
  Serial.println(pressure);
  Serial.print("MQ-5 LPG:");
  Serial.println(mq5_lpg);
  Serial.print("MQ-135 CO2:");
  Serial.println(mq135_co2);

  Time clockTime = readClock();
   Serial.print("Timestamp:");
   Serial.print(clockTime.year);
     Serial.print("/");
   Serial.print(clockTime.mon);
      Serial.print("/");
   Serial.print(clockTime.date);
         Serial.print(" ");
   Serial.print(clockTime.hour);
         Serial.print(":");
   Serial.print(clockTime.min);
         Serial.print(":");
   Serial.println(clockTime.sec);


  
  delay(READING_SENSOR_INTERVAL);
  firstLoop = false;
  //initScreen();
}

void initMQ135(){
  // do nothings;
}
void readMQ135(){
  mq135_co2 = mq135Sensor.getPPM();
    if (mq135_co2>MQ135_CO2_MAX_VALUE) {
    mq135_co2 = MQ135_CO2_MAX_VALUE;
  }

}
void initMQ5(){
  pinMode(MQ5_LPG_PIN, INPUT);
}
void readMQ5(){
  mq5_lpg = analogRead(MQ5_LPG_PIN);
  if (mq5_lpg>MQ5_LPG_MAX_VALUE) {
    mq5_lpg = MQ5_LPG_MAX_VALUE;
  }
  
}
void initRelay(){
  pinMode(RELAY_PIN, OUTPUT);
  turnOffRelay();
}
void turnOffRelay(){
  digitalWrite(RELAY_PIN, HIGH); // Power Off
}
void turnOnRelay(){
   digitalWrite(RELAY_PIN, LOW); // Power On
}
void initArduinoConnect(){
  Wire.begin();
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  arduinoConnect.begin(details(arduinoData), &Wire);
  randomSeed(analogRead(0));
}
void sendArduinConnect(){
  Serial.println("Send to another arduino.");
  arduinoData.temperatureC = temperatureC;
  arduinoData.humidityPer = humidityPer;
  arduinoData.pressure = pressure;
  arduinoData.pm_1 = pm_1;
  arduinoData.pm_2_5 = pm_2_5;
  arduinoData.pm_10 = pm_10;

  Time clockTime = readClock();
  arduinoData.clockYear = clockTime.year;
  arduinoData.clockMonth = clockTime.mon;
  arduinoData.clockDay = clockTime.date;
  arduinoData.clockHour = clockTime.hour;
  arduinoData.clockMinute = clockTime.min;
  arduinoData.clockSeconds = clockTime.sec;

  arduinoData.mq135_co2 = mq135_co2;
  arduinoData.mq5_lpg = mq5_lpg;
  
  if ( isSuccesToInitSD ){
    arduinoData.isSuccesToInitSD = 1;
  }
  //send the data
  arduinoConnect.sendData(I2C_SLAVE_ADDRESS);
}
void initBME280() {
  if (!bme280.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
   // while (1);
  } else {
    isSuccesToInitBME = true;
  }
  bme280.setTempCal(-1);// Sensor was reading high so offset by 1 degree C 
}

void initPMS7003() {
  myPMS7003.begin(9600);
}

void initClock() {
//  tmElements_t clockTime;
//  if (!RTC.read(clockTime)) {
//    if (RTC.chipPresent()) {
//      Serial.println("The DS1307 is stopped.  Please run the SetTime");
//      Serial.println("example to initialize the time and begin running.");
//      Serial.println();
//     
//    } else {
//      Serial.println("DS1307 read error!  Please check the circuitry.");
//      Serial.println();
//      isClockOk = false;
//    }
//  }
  rtc.begin();

}

//tmElements_t writeClockTime;
//bool getTime4WriteClock(const char *str) {
//  int Hour, Min, Sec;
//  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
//  writeClockTime.Hour = Hour;
//  writeClockTime.Minute = Min;
//  writeClockTime.Second = Sec;
//  return true;
//}
//bool getDate4WriteClock(const char *str) {
//  const char *monthName[12] = {
//    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
//    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
//  };
//  char Month[12];
//  int Day, Year;
//  uint8_t monthIndex;
//
//  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
//  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
//    if (strcmp(Month, monthName[monthIndex]) == 0) break;
//  }
//  if (monthIndex >= 12) return false;
//  writeClockTime.Day = Day;
//  writeClockTime.Month = monthIndex + 1;
//  writeClockTime.Year = CalendarYrToTm(Year);
//  return true;
//}

void writeClock() {
    // The following lines can be uncommented to set the date and time
  rtc.setDOW(SUNDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(13, 34, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(29, 4, 2018);   // Set the date to January 1st, 2014

//  bool parse = false;
//  bool config = false;
//
//  // get the date and time the compiler was run
//  if (getDate4WriteClock(__DATE__) && getTime4WriteClock(__TIME__)) {
//    parse = true;
//    // and configure the RTC with this info
//    if (RTC.write(writeClockTime)) {
//      config = true;
//    }
//  }
//  while (!Serial) ; // wait for Arduino Serial Monitor
//  delay(200);
//  if (parse && config) {
//    Serial.print("DS1307 configured Time=");
//    Serial.print(__TIME__);
//    Serial.print(", Date=");
//    Serial.println(__DATE__);
//  } else if (parse) {
//    Serial.println("DS1307 Communication Error :-{");
//    Serial.println("Please check your circuitry");
//  } else {
//    Serial.print("Could not parse info from the compiler, Time=\"");
//    Serial.print(__TIME__);
//    Serial.print("\", Date=\"");
//    Serial.print(__DATE__);
//    Serial.println("\"");
//  }
}

void initSDCard() {
  if ( ENABLE_SD_CARD ) {
    pinMode(SD_CARD_CS_PIN, OUTPUT);
    //if (!sd.begin(SD_CARD_CS_PIN, SPI_HALF_SPEED)) {
    if (!SD.begin(SD_CARD_CS_PIN)) {
      //sd.initErrorHalt(); //SdFat specified
      isSuccesToInitSD = false;
      Serial.println("SD Card initialization failed!");
      return;
    } else {
      isSuccesToInitSD = true;
      Serial.println("SD OK");
    }
  }
}

void writeToSDCard() {
  if ( ENABLE_SD_CARD && isSuccesToInitSD ) { 

    //if ( sdCardFile.open(SD_CARD_FILENAME, O_RDWR | O_CREAT | O_AT_END) ) {
    File sdCardFile = SD.open(SD_CARD_FILENAME, FILE_WRITE);
    if ( sdCardFile ) {
      //Serial.println(String("Writing to ") + SD_CARD_FILENAME);
      Time currentTime = readClock();
      memset(timeStr, ' ', TIME_STR_SIZE);
      snprintf(timeStr, sizeof(timeStr), "%04d/%02d/%02d %02d:%02d:%02d", currentTime.year, currentTime.mon, currentTime.date, currentTime.hour, currentTime.min, currentTime.sec); // timeStr[20]
      memset(temperatureStr, ' ', TEMPERATURE_STR_SIZE);
      memset(humidityStr, ' ', HUMIDITY_STR_SIZE);
      memset(pressureStr, ' ', PRESSURE_STR_SIZE);
      dtostrf(temperatureC, 2, 1, temperatureStr);  //format the display string (2 digits, 1 dp)  // temperatureStr[4]
      dtostrf(humidityPer, 2, 1, humidityStr);  // humidityStr[4]
      dtostrf(pressure, 4, 0, pressureStr);
      if (!writeSDHeader) {
        sdCardFile.println(SD_FILE_DATA_HEADER);  // Write Header
        writeSDHeader = true;
      }

      memset(mq5LpgStr, ' ', MQ5_LPG_STR_SIZE);
      memset(mq135Co2Str, ' ', MQ135_CO2_STR_SIZE);
      dtostrf(mq5_lpg, 4, 0, mq5LpgStr);   
      dtostrf(mq135_co2, 4, 0, mq135Co2Str);

      // Modify SD_FILE_DATA_HEADER, if modify this data format
      if (pm_1 == 0 && pm_2_5 == 0 && pm_10 == 0) { // 2016.11.11 (pmat25==0 && pmat100==0 && pmat10==0) means PMS7003 did not provide good data.
        sdCardFile.println(timeStr + String("\t") + temperatureStr + "\t" + humidityStr + "\t" + pressureStr + "\t" + mq5LpgStr+ "\t" + mq135Co2Str);
      } else {
        sdCardFile.println(timeStr + String("\t") + temperatureStr + "\t" + humidityStr + "\t" + pressureStr + "\t" + mq5LpgStr+ "\t" + mq135Co2Str + "\t" + pm_2_5 + "\t" + pm_10 + "\t" + pm_1);
      }
      sdCardFile.close();   // flush(), if keep opening file, use flush() to write data immediately.
      
    } else {

      Serial.println("ERR: Cannot open ENV_MONITORING.txt...");
    }
  } 
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
Time readClock() {
  return rtc.getTime();
//  tmElements_t clockTime;
//  if (RTC.read(clockTime)) {
//        Serial.print("Ok, Time = ");
//        print2digits(clockTime.Hour);
//        Serial.write(':');
//        print2digits(clockTime.Minute);
//        Serial.write(':');
//        print2digits(clockTime.Second);
//        Serial.print(", Date (D/M/Y) = ");
//        Serial.print(clockTime.Day);
//        Serial.write('/');
//        Serial.print(clockTime.Month);
//        Serial.write('/');
//        Serial.print(tmYearToCalendar(clockTime.Year));
//        Serial.println();
//    isClockOk = true;
//  } else {
//    isClockOk = false;
//    if (RTC.chipPresent()) {
//      Serial.println("The DS1307 is stopped.  Please run the SetTime");
//      Serial.println("example to initialize the time and begin running.");
//      Serial.println();
//    } else {
//      Serial.println("DS1307 read error!  Please check the circuitry.");
//      Serial.println();
//    }
//    //delay(9000);
//  }
//  return clockTime;
}


unsigned int getPMS7003Data(unsigned char chrSrc[], byte bytHigh, byte bytLow)
{
  return (chrSrc[bytHigh] << 8) + chrSrc[bytLow];
}
void readPMS7003() {
  boolean isFound = false;
  int roundTry = 0;
  while (!isFound && roundTry <= 500) {
    unsigned char chrData[30];
    roundTry++;
    if (myPMS7003.available())
    {
      pms7003ChrRecv = myPMS7003.read();

      if (pms7003ChrRecv == START_1 && bytCount == 0)  bytCount = 1;
      if (pms7003ChrRecv == START_2 && bytCount == 1)  bytCount = 2;

      if (bytCount == 2)
      {
        bytCount = 0;
        isFound = true;
        for (int i = 0; i < 30; i++)
        {
          chrData[i] = myPMS7003.read();
          //Serial.print(String(chrData[i], HEX));
          //Serial.print("-");
        } // end for(bytCount = 0; bytCount < 30; bytCount++)
        //Serial.println("");

        //         Serial.println("===================================================");
        //         Serial.print("Data Length = ");
        //         Serial.print((unsigned int) (chrData[DATA_LENGTH_H] << 8) + chrData[DATA_LENGTH_L]);
        //         Serial.print(", Device Version = ");
        //         Serial.print((unsigned int) chrData[VERSION]);
        //         Serial.print(" (Src=");  Serial.print(chrData[VERSION]);   Serial.print(" HEX="); Serial.print(String(chrData[VERSION], HEX));
        //         Serial.print("), Error Code = ");
        //         Serial.println((unsigned int) chrData[ERROR_CODE]);
        //         Serial.println("");

        pm_1 = getPMS7003Data(chrData, PM1_0_ATMOSPHERE_H, PM1_0_ATMOSPHERE_L);
        pm_2_5 = getPMS7003Data(chrData, PM2_5_ATMOSPHERE_H, PM2_5_ATMOSPHERE_L);
        pm_10 = getPMS7003Data(chrData, PM10_ATMOSPHERE_H, PM10_ATMOSPHERE_L);
        //         Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        //         Serial.println("                   Atmosphere (ug/m^3)");
        //         Serial.println("  PM 1.0     PM 2.5     PM 10");
        //         Serial.println("---------------------------------------------------");
        //         Serial.print("  ");
        //         Serial.print(pm_1);
        //         Serial.print("          ");
        //         Serial.print(pm_2_5);
        //         Serial.print("          ");
        //         Serial.println(pm_10);
        //         Serial.println("");

        //         Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        //         Serial.println("                   CF = 1 (ug/m^3)");
        //         Serial.println("  PM 1.0     PM 2.5     PM 10");
        //         Serial.println("---------------------------------------------------");
        //         Serial.print("  ");
        //         Serial.print(getPMS7003Data(chrData, PM1_0_CF1_H, PM1_0_CF1_L));
        //         Serial.print("          ");
        //         Serial.print(getPMS7003Data(chrData, PM2_5_CF1_H, PM2_5_CF1_L));
        //         Serial.print("          ");
        //         Serial.println(getPMS7003Data(chrData, PM10_CF1_H, PM10_CF1_L));
        //         Serial.println("");

        //         Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        //         Serial.println("                   Dusty Count");
        //         Serial.println("  >um0.3     >0.5um     >1.0um     >2.5um     >5.0um     >10um");
        //         Serial.println("--------------------------------------------------------------");
        //         Serial.print("  ");
        //         Serial.print(getPMS7003Data(chrData, UM0_3_H, UM0_3_L));
        //         Serial.print("        ");
        //         Serial.print(getPMS7003Data(chrData, UM0_5_H, UM0_5_L));
        //         Serial.print("        ");
        //         Serial.print(getPMS7003Data(chrData, UM1_0_H, UM1_0_L));
        //         Serial.print("        ");
        //         Serial.print(getPMS7003Data(chrData, UM2_5_H, UM2_5_L));
        //         Serial.print("        ");
        //         Serial.print(getPMS7003Data(chrData, UM5_0_H, UM5_0_L));
        //         Serial.print("        ");
        //         Serial.print(getPMS7003Data(chrData, UM10_H, UM10_L));
        //         Serial.print("        ");
        //
        //         Serial.println("");


      }
    }
  }
}



void readBME280() {
  bme280.readSensor();
  temperatureC = bme280.getTemperature_C();
  temperatureF = bme280.getTemperature_F();
  humidityPer = bme280.getHumidity();
  pressure = bme280.getPressure_MB();

  //  Serial.print("temperatureC:");
  //  Serial.println(temperatureC);
  //  Serial.print("temperatureF:");
  //  Serial.println(temperatureF);
  //  Serial.print("humidityPer:");
  //  Serial.println(humidityPer);
  //  Serial.print("pressure:");
  //  Serial.println(pressure);
}



