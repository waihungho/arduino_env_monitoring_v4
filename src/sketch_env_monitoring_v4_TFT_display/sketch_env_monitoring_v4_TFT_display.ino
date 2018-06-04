// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Arduino Software Version v1.6.12
// Arduino Board UNO
//
// Sensors:
//  4. TFT Screen
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Program Specification
//    1. setup()
//        initialize TFT Screen 
//    2. loop()
//        c. Display data on Screen    [displayDataOnScreen()]
//    Problem Solving History:
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------
//  Software Configuration (BEGIN)
// ------------------------------------------
#define SERIAL_BAUD_RATE 9600
#define WELCOME_MSG_FIRST_LINE "Amber Caffe Lab."
 
#define READING_SENSOR_INTERVAL 800     // Interval to read ALL sensors. 10s interval.
// ------------------------------------------
//  Software Configuration (END)
// ------------------------------------------


// ------------------------------------------
//  PIN Configuration (BEGIN)
// ------------------------------------------

// TFT PIN:     Arduino
// VCC  VCC     VCC  VCC
// 22   23      22   23
// 24   25      24   25
// 26   27      26   27
// ...  ..      ...  ..
// 52   53      52   53
// GND  GND     GND  GND
// Another Side Pin
// VCC          VCC   
// GND          GND

//      3.3V  RESET
// GND  EMPTY
//53  52
//51
//49
//47
//45
//43
//41
//39
//37
//35
//33
//31
//29
//27
//25
//23
//5V  5V


// ------------------------------------------
//  PIN Configuration (END)
// ------------------------------------------

// TFT Display Configuration (BEGIN)
#include <TFT_HX8357.h> // Hardware-specific library
#include "Free_Fonts.h" // Include the header file attached to this sketch
TFT_HX8357 tft = TFT_HX8357();       // Invoke custom library
#define TFT_GREY 0x5AEB
float ltx = 0;    // Saved x coord of bottom of needle
uint16_t osx = 120, osy = 120; // Saved x & y coords
uint32_t updateTime = 0;       // time for next update
int old_analog ; // Value last displayed
int old_digital; // Value last displayed
int value[6] ;
int old_value[6] ;
int old_text_value[6] ;
int d = 0;
// TFT Display Configuration (END)


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

#define REINIT_TFT_COUNT 1800 // re-init every hour
// Software variables (BEGIN)

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                    FUNCTIONS
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  Serial.println("Setup[TFT]");
  initScreen();

  Serial.println("Setup Arduino Connect Arduino");
  initArduinoConnect();

  firstLoop = true;
  Serial.println("Setup[Done]");
}

long count=0;
void loop() { // put your main code here, to run repeatedly:

//  pm_1 = 0;
//  pm_2_5 = 0;
//  pm_10 = 0;
//  temperatureC = 0;
//  temperatureF = 0;
//  humidityPer = 0;
//  pressure = 0;
//  if ( !firstLoop){ // 2016.11.11: Rule of Thumb: dont log first data, which it is often inaccurate.
//    displayDataOnScreen();
//  }
  
  if(arduinoConnect.receiveData()){
//     Serial.print("temperatureC:");
//     Serial.println(arduinoData.temperatureC);
//     Serial.print("humidityPer:");
//     Serial.println(arduinoData.humidityPer);
//     Serial.print("pressure:");
//     Serial.println(arduinoData.pressure);
//     Serial.print("pm_1:");
//     Serial.println(arduinoData.pm_1);   
//     Serial.print("pm_2_5:");
//     Serial.println(arduinoData.pm_2_5);  
//     Serial.print("pm_10:");
//     Serial.println(arduinoData.pm_10);   
//     Serial.print("clockYear:");
//     Serial.println(arduinoData.clockYear);  
//     Serial.print("clockMonth:");
//     Serial.println(arduinoData.clockMonth);  
//     Serial.print("clockDay:");
//     Serial.println(arduinoData.clockDay);  
//     Serial.print("clockHour:");
//     Serial.println(arduinoData.clockHour);  
//     Serial.print("clockMinute:");
//     Serial.println(arduinoData.clockMinute);  
//     Serial.print("clockSeconds:");
//     Serial.println(arduinoData.clockSeconds);  
    if (count>=REINIT_TFT_COUNT){ // re-init the screen every 1-hour
      initScreen();
      count=0;
    }
    if (!firstLoop){
      displayDataOnScreen();
    }
     firstLoop = false;
     count++;
  }

//  delay(READING_SENSOR_INTERVAL);
//  firstLoop = false;
  //initScreen();
}


void initArduinoConnect(){
  Wire.begin(I2C_SLAVE_ADDRESS);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc. 
  arduinoConnect.begin(details(arduinoData), &Wire);
  //define handler function on receiving data
  Wire.onReceive(receive);
}
void receive(int numBytes) {}

void initScreen() {
  tft.init();
  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);
  //analogMeter(); // Draw analogue meter
  analogMeter_V2(0, "%HUM");
  analogMeter_V2(240, "TEMP");
  byte d = 55;
  int w = 50;

 old_analog =  -999; // Value last displayed
 old_digital = -999; // Value last displayed
 for (int i = 0; i < 6; i++){
  value[i]=0;
  old_value[i]=-1;
  old_text_value[i]=-1;
 }
  
  plotLinear("PM 1 ", 0, 160, w);
  plotLinear("PM 2.5", 1 * d, 160, w);
  plotLinear("PM 10", 2 * d, 160, w);

}


//char displayHumOrig[15], displayTempOrig[15], displayPressureOrig[15], displayPm25Orig[15], displayTimeOrig[TIME_STR_SIZE];

void displayDataOnScreen() {
  Serial.println("Display On Screen");

//  Serial.print("Time:");
//  tmElements_t clockTime = readClock();
//  print2digits(clockTime.Hour);
//  Serial.write(':');
//  print2digits(clockTime.Minute);
//  Serial.write(':');
//  print2digits(clockTime.Second);
//  Serial.print(", Date (D/M/Y) = ");
//  Serial.print(clockTime.Day);
//  Serial.write('/');
//  Serial.print(clockTime.Month);
//  Serial.write('/');
//  Serial.println(tmYearToCalendar(clockTime.Year));
//
//  Serial.print("PM1.0:");
//  Serial.println(pm_1);
//  Serial.print("PM2.5:");
//  Serial.println(pm_2_5);
//  Serial.print("PM10.0:");
//  Serial.println(pm_10);
//
Serial.print("Temperature:");
Serial.println(arduinoData.temperatureC);
//
 Serial.print("Humidity:");
 Serial.println(arduinoData.humidityPer);
//
Serial.print("Pressure:");
Serial.println(arduinoData.pressure);

Serial.print("mq135_co2:");
Serial.println(arduinoData.mq135_co2);

Serial.print("mq5_lpg:");
Serial.println(arduinoData.mq5_lpg);

  d += 4; if (d >= 360) d = 0;

  //value[0] = map(analogRead(A0), 0, 1023, 0, 100); // Test with value form Analogue 0
  //value[1] = map(analogRead(A1), 0, 1023, 0, 100); // Test with value form Analogue 1
  //value[2] = map(analogRead(A2), 0, 1023, 0, 100); // Test with value form Analogue 2
  //value[3] = map(analogRead(A3), 0, 1023, 0, 100); // Test with value form Analogue 3
  //value[4] = map(analogRead(A4), 0, 1023, 0, 100); // Test with value form Analogue 4
  //value[5] = map(analogRead(A5), 0, 1023, 0, 100); // Test with value form Analogue 5

  // Create a Sine wave for testing
  value[0] = 50 + 50 * sin((d + 0) * 0.0174532925);
  value[1] = 50 + 50 * sin((d + 60) * 0.0174532925);
  value[2] = 50 + 50 * sin((d + 120) * 0.0174532925);
  value[3] = 50 + 50 * sin((d + 180) * 0.0174532925);
  value[4] = 50 + 50 * sin((d + 240) * 0.0174532925);
  value[5] = 50 + 50 * sin((d + 300) * 0.0174532925);

  unsigned int DISPLAY_STR_SIZE=20;
  char displayHum[DISPLAY_STR_SIZE], displayTemp[DISPLAY_STR_SIZE], 
      displayHumTemp[DISPLAY_STR_SIZE],
      displayPressure[DISPLAY_STR_SIZE], displayPm25[DISPLAY_STR_SIZE], displayTime[TIME_STR_SIZE],
      displayMq5Lpg[DISPLAY_STR_SIZE], displayMq135Co2[DISPLAY_STR_SIZE];
  unsigned int PM25_STR_SIZE = 4;
  char pm2_5Str[PM25_STR_SIZE];
  memset(temperatureStr, ' ', TEMPERATURE_STR_SIZE);
  memset(humidityStr, ' ', HUMIDITY_STR_SIZE);
  memset(pressureStr, ' ', PRESSURE_STR_SIZE);
  memset(pm2_5Str, ' ', PM25_STR_SIZE);

  memset(mq5LpgStr, ' ', MQ5_LPG_STR_SIZE);
  memset(mq135Co2Str, ' ', MQ135_CO2_STR_SIZE);

  
  dtostrf(arduinoData.temperatureC, 2, 1, temperatureStr);  //format the display string (2 digits, 1 dp)  // temperatureStr[4]
  dtostrf(arduinoData.humidityPer, 2, 1, humidityStr);  // humidityStr[4]
  dtostrf(arduinoData.pressure, 4, 0, pressureStr);
  dtostrf(arduinoData.pm_2_5, 3, 0, pm2_5Str);

  dtostrf(arduinoData.mq5_lpg, 4, 0, mq5LpgStr);
  dtostrf(arduinoData.mq135_co2, 4, 0, mq135Co2Str);

  strcpy(displayHumTemp, "H: ");
  strcat(displayHumTemp, humidityStr);
  strcat(displayHumTemp, "%  T: ");
  strcat(displayHumTemp, temperatureStr);
  strcat(displayHumTemp, "C");
  
   strcpy(displayHum,  "HUM  : "); // need to consider DISPLAY_STR_SIZE
   strcat(displayHum, humidityStr);
   strcat(displayHum, " %");

   strcpy(displayTemp,  "TEMP : ");
   strcat(displayTemp, temperatureStr);
   strcat(displayTemp, " C");

   strcpy(displayPressure,  "BARO : ");
   strcat(displayPressure, pressureStr);
   strcat(displayPressure, " hPa");

   strcpy(displayPm25,  "PM 2.5: ");
   strcat(displayPm25, pm2_5Str);
   strcat(displayPm25, " ug/m3  ");

   strcpy(displayMq5Lpg,  "DGas : ");
   if ( arduinoData.mq5_lpg != 0 ) {
    strcat(displayMq5Lpg, mq5LpgStr);
    strcat(displayMq5Lpg, " ppm  ");
   } else {
     strcat(displayMq5Lpg, " n/a    ");
   }
   
   strcpy(displayMq135Co2,  "CO2    : ");
   if ( arduinoData.mq135_co2 != 0 ) {
    strcat(displayMq135Co2, mq135Co2Str);
    strcat(displayMq135Co2, " ppm  ");
   } else {
     strcat(displayMq135Co2, " n/a    ");
   }
   
  //tmElements_t currentTime = readClock();
  if (  arduinoData.clockYear!=0 ) {
    memset(displayTime, ' ', TIME_STR_SIZE);
    snprintf(displayTime, sizeof(displayTime), "%04d/%02d/%02d %02d:%02d:%02d", arduinoData.clockYear, arduinoData.clockMonth, arduinoData.clockDay, arduinoData.clockHour, arduinoData.clockMinute, arduinoData.clockSeconds); 
  }

//  tft.setTextColor(TFT_BLACK);
//  tft.drawRightString(displayTimeOrig, 450, 150 , 4);
//  tft.drawRightString(displayHumOrig, 450, 180 , 4);
//  tft.drawRightString(displayTempOrig, 450, 210 , 4);
//  tft.drawRightString(displayPressureOrig, 450, 240 , 4);

 // Reference: https://github.com/Bodmer/TFT_HX8357/blob/master/examples/Free_Font_Demo/Free_Font_Demo.ino
//  tft.setFreeFont(FSB9);                              // Select the font
//  tft.drawString("Serif Bold 9pt", xpos, ypos, GFXFF);  // Draw the text string in the selected GFX free font
//  ypos += tft.fontHeight(GFXFF);    
  
  unsigned int DISPLAY_X_COR = 200;
  unsigned int DISPLAY_Y_COR_INIT = 140;
  unsigned int EACH_ROW_HEIGHT =30;
  unsigned int rowSize = 0;
  if ( arduinoData.isSuccesToInitSD == 1){
     tft.setTextColor(TFT_PINK, TFT_BLACK);
     tft.drawString("[SD Card]", 410, DISPLAY_Y_COR_INIT + EACH_ROW_HEIGHT * rowSize , 2);
  }
//tft.setFreeFont(FF4);
  unsigned int fontSize = 4;

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //tft.drawString(displayHumTemp, DISPLAY_X_COR, DISPLAY_Y_COR_INIT + EACH_ROW_HEIGHT * rowSize++ , fontSize);
  //tft.drawString(displayHum, DISPLAY_X_COR, DISPLAY_Y_COR_INIT + EACH_ROW_HEIGHT * rowSize++ , fontSize);
  //tft.drawString(displayTemp, DISPLAY_X_COR, DISPLAY_Y_COR_INIT + EACH_ROW_HEIGHT * rowSize++, fontSize);
  tft.drawString(displayPressure, DISPLAY_X_COR, DISPLAY_Y_COR_INIT + EACH_ROW_HEIGHT * rowSize++, fontSize);


  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(displayHumTemp, DISPLAY_X_COR, DISPLAY_Y_COR_INIT + EACH_ROW_HEIGHT * rowSize++ , fontSize);

  // uint16_t TextColor
  uint16_t mq5LpgColor;
  uint16_t ALERT_MQ5_LPG = 400;
  if ( arduinoData.mq5_lpg <= 200 ) {
    mq5LpgColor=TFT_GREEN;
  } else if ( arduinoData.mq5_lpg > 200 &&  arduinoData.mq5_lpg <= ALERT_MQ5_LPG ) {
    mq5LpgColor=TFT_YELLOW;
  }  else if ( arduinoData.mq5_lpg > ALERT_MQ5_LPG  ) {
    mq5LpgColor=TFT_PURPLE;
  }
  tft.setTextColor(mq5LpgColor, TFT_BLACK);
  tft.drawString(displayMq5Lpg, DISPLAY_X_COR, DISPLAY_Y_COR_INIT + EACH_ROW_HEIGHT * rowSize++ , fontSize);  
  // tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  // tft.drawString(WELCOME_MSG_FIRST_LINE, DISPLAY_X_COR, DISPLAY_Y_COR_INIT + EACH_ROW_HEIGHT * rowSize++ , fontSize);  

  uint16_t mq135Co2Color=TFT_GREEN;
   uint16_t ALERT_MQ135_CO2 = 2000;
  if ( arduinoData.mq135_co2 <= 1000 ) {
    mq135Co2Color=TFT_GREEN;
  } else if ( arduinoData.mq135_co2 > 1000 &&  arduinoData.mq135_co2 <= ALERT_MQ135_CO2 ) {
    mq135Co2Color=TFT_YELLOW;
  }  else if ( arduinoData.mq135_co2 > ALERT_MQ135_CO2  ) {
    mq135Co2Color=TFT_PURPLE;
  }
  tft.setTextColor(mq135Co2Color, TFT_BLACK);
  tft.drawString(displayMq135Co2, DISPLAY_X_COR, DISPLAY_Y_COR_INIT + EACH_ROW_HEIGHT * rowSize++ , fontSize);  
   
 if ( arduinoData.pm_2_5 <= 100 ) {
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
 } else if ( arduinoData.pm_2_5 > 100 &&  arduinoData.pm_2_5 <= 200 ) {
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 } else if ( arduinoData.pm_2_5 > 200 &&  arduinoData.pm_2_5 <= 400 ) {
  tft.setTextColor(TFT_PINK, TFT_BLACK);
 } else if ( arduinoData.pm_2_5 > 400  ) {
  tft.setTextColor(TFT_PURPLE, TFT_BLACK);
 } 
 
  tft.drawString(displayPm25, DISPLAY_X_COR, DISPLAY_Y_COR_INIT + EACH_ROW_HEIGHT * rowSize++ , fontSize);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
//strcpy(displayHumOrig,  displayHum);
//strcpy(displayTempOrig,  displayTemp);
//strcpy(displayPressureOrig,  displayPressure);
//strcpy(displayPm25Orig,  displayPm25);
//strcpy(displayTimeOrig,  displayTime);
//displayHumOrig =displayHum;
//displayTempOrig =displayTemp;
//displayPressureOrig=displayPressure;
//displayPm25Orig =displayPm25;
//displayTimeOrig=displayTime;

  value[0] = arduinoData.pm_1;
  value[1] = arduinoData.pm_2_5;
  value[2] = arduinoData.pm_10;

  plotPointer(); // It takes aout 3.5ms to plot each gauge for a 1 pixel move, 21ms for 6 gauges
  plotNeedle(arduinoData.temperatureC, 0, 240, "TEMP"); // It takes between 2 and 12ms to replot the needle with zero delay
  plotNeedle(arduinoData.humidityPer, 0, 0, "%HUM"); // It takes between 2 and 12ms to replot the needle with zero delay

  if ( arduinoData.clockYear != 0 ) {
    //tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.drawString(displayTime, DISPLAY_X_COR, DISPLAY_Y_COR_INIT + EACH_ROW_HEIGHT * rowSize++ , fontSize);
  }

}

// #########################################################################
// Update needle position
// This function is blocking while needle moves, time depends on ms_delay
// 10ms minimises needle flicker if text is drawn within needle sweep area
// Smaller values OK if text not in sweep area, zero for instant movement but
// does not look realistic... (note: 100 increments for full scale deflection)
// #########################################################################
void plotNeedle(float value, byte ms_delay, int x_adj, char* header)
{
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  char buf[8]; dtostrf(value, 4, 1, buf);
  tft.drawRightString(buf, 40 + x_adj, 119 - 20, 2);

  if (value < -10) value = -10; // Limit value to emulate needle end stops
  if (value > 110) value = 110;

  // Move the needle util new value reached
  //  while (!(value == old_analog)) {
  //    if (old_analog < value) old_analog++;
  //    else old_analog--;
  //
  //    if (ms_delay == 0) old_analog = value; // Update immediately id delay is 0

  float sdeg = map(value, -10, 110, -150, -30); // Map value to angle
  // Calcualte tip of needle coords
  float sx = cos(sdeg * 0.0174532925);
  float sy = sin(sdeg * 0.0174532925);

  // Calculate x delta of needle start (does not start at pivot point)
  float tx = tan((sdeg + 90) * 0.0174532925);

  // Erase old needle image
  tft.drawLine(120 + 20 * ltx - 1 + x_adj, 140 - 20, osx - 1 + x_adj, osy, TFT_WHITE);
  tft.drawLine(120 + 20 * ltx + x_adj, 140 - 20, osx + x_adj, osy, TFT_WHITE);
  tft.drawLine(120 + 20 * ltx + 1 + x_adj, 140 - 20, osx + 1 + x_adj, osy, TFT_WHITE);

  // Re-plot text under needle
  tft.setTextColor(TFT_BLACK);
  tft.drawCentreString(header, 120 + x_adj, 70, 4); // // Comment out to avoid font 4

  // Store new needle end coords for next erase
  ltx = tx;
  osx = sx * 98 + 120;
  osy = sy * 98 + 140;

  // Draw the needle in the new postion, magenta makes needle a bit bolder
  // draws 3 lines to thicken needle
  tft.drawLine(120 + 20 * ltx - 1 + x_adj , 140 - 20, osx - 1 + x_adj, osy, TFT_RED);
  tft.drawLine(120 + 20 * ltx + x_adj, 140 - 20 , osx + x_adj, osy, TFT_MAGENTA);
  tft.drawLine(120 + 20 * ltx + 1 + x_adj , 140 - 20, osx + 1 + x_adj, osy, TFT_RED);

  //    tft.drawLine(120 + (x_adj+20) * ltx - 1 + x_adj, 140 - 20, osx - 1+x_adj, osy, TFT_RED);
  //    tft.drawLine(120 + (x_adj+20) * ltx + x_adj, 140 - 20 , osx+x_adj, osy, TFT_MAGENTA);
  //    tft.drawLine(120 + (x_adj+20) * ltx + 1 + x_adj, 140 - 20, osx + 1+x_adj, osy, TFT_RED);

  // Slow needle down slightly as it approaches new postion
  //    if (abs(old_analog - value) < 10) ms_delay += ms_delay / 5;
  //
  //    // Wait before next update
  //    delay(ms_delay);
  //  }
}

// #########################################################################
//  Draw a linear meter on the screen
// #########################################################################
void plotLinear(char *label, int x, int y, int w)
{
  //int w = 36;
  tft.drawRect(x, y, w, 155, TFT_GREY);
  tft.fillRect(x + 2, y + 19, w - 3, 155 - 38, TFT_WHITE);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawCentreString(label, x + w / 2, y + 2, 2);

  for (int i = 0; i < 110; i += 10)
  {
    tft.drawFastHLine(x + 20, y + 27 + i, 6, TFT_BLACK);
  }

  for (int i = 0; i < 110; i += 50)
  {
    tft.drawFastHLine(x + 20, y + 27 + i, 9, TFT_BLACK);
  }

  tft.fillTriangle(x + 3, y + 127, x + 3 + 16, y + 127, x + 3, y + 127 - 5, TFT_RED);
  tft.fillTriangle(x + 3, y + 127, x + 3 + 16, y + 127, x + 3, y + 127 + 5, TFT_RED);

  tft.drawCentreString("    ", x + w / 2, y + 155 - 18, 2);
}

// #########################################################################
//  Adjust 6 linear meter pointer positions
// #########################################################################
void plotPointer(void)
{
  int dy = 187;
  byte pw = 16;

  int w = 55;

  // restore to black color.
  tft.setTextColor(TFT_BLACK, TFT_BLACK);
  for (int i = 0; i < 3; i++)
  {
    if ( old_text_value[i] >= 0 ) {
      char buf[8]; dtostrf(old_text_value[i], 4, 0, buf);
      tft.drawRightString(buf, i * w + 36 - 5, 187 - 27 + 155 - 18, 2);
      old_text_value[i] = value[i];
    }
  }
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  // Move the 6 pointers one pixel towards new value
  for (int i = 0; i < 3; i++)
  {
    char buf[8]; dtostrf(value[i], 4, 0, buf);
    tft.drawRightString(buf, i * w + 36 - 5, 187 - 27 + 155 - 18, 2);

    int dx = 3 + w * i;
    if (value[i] < 0) value[i] = 0; // Limit value to emulate needle end stops
    if (value[i] > 100) value[i] = 100;

    while (!(value[i] == old_value[i])) {
      dy = 187 + 100 - old_value[i];
      if (old_value[i] > value[i])
      {
        tft.drawLine(dx, dy - 5, dx + pw, dy, TFT_WHITE);
        old_value[i]--;
        tft.drawLine(dx, dy + 6, dx + pw, dy + 1, TFT_RED);
      }
      else
      {
        tft.drawLine(dx, dy + 5, dx + pw, dy, TFT_WHITE);
        old_value[i]++;
        tft.drawLine(dx, dy - 6, dx + pw, dy - 1, TFT_RED);
      }
    }
  }
}


void analogMeter_V2(int x_adj, char* header)
{
  // Meter outline
  tft.fillRect(0 + x_adj, 0, 239, 126, TFT_GREY); // X, Y, width, height
  tft.fillRect(5 + x_adj, 3, 230, 119, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);  // Text colour

  // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  for (int i = -50; i < 51; i += 5) {
    // Long scale tick length
    int tl = 15;

    // Coodinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (100 + tl) + 120;
    uint16_t y0 = sy * (100 + tl) + 140;
    uint16_t x1 = sx * 100 + 120;
    uint16_t y1 = sy * 100 + 140;

    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = sx2 * (100 + tl) + 120;
    int y2 = sy2 * (100 + tl) + 140;
    int x3 = sx2 * 100 + 120;
    int y3 = sy2 * 100 + 140;

    // Yellow zone limits
    //if (i >= -50 && i < 0) {
    //  tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_YELLOW);
    //  tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_YELLOW);
    //}

    // Green zone limits
    if (i >= 0 && i < 25) {
      tft.fillTriangle(x0 + x_adj, y0, x1 + x_adj, y1, x2 + x_adj, y2, TFT_GREEN);
      tft.fillTriangle(x1 + x_adj, y1, x2 + x_adj, y2, x3 + x_adj, y3, TFT_GREEN);
    }

    // Orange zone limits
    if (i >= 25 && i < 50) {
      tft.fillTriangle(x0 + x_adj, y0, x1 + x_adj, y1, x2 + x_adj, y2, TFT_ORANGE);
      tft.fillTriangle(x1 + x_adj, y1, x2 + x_adj, y2, x3 + x_adj, y3, TFT_ORANGE);
    }

    // Short scale tick length
    if (i % 25 != 0) tl = 8;

    // Recalculate coords incase tick lenght changed
    x0 = sx * (100 + tl) + 120;
    y0 = sy * (100 + tl) + 140;
    x1 = sx * 100 + 120;
    y1 = sy * 100 + 140;

    // Draw tick
    tft.drawLine(x0 + x_adj, y0, x1 + x_adj, y1, TFT_BLACK);

    // Check if labels should be drawn, with position tweaks
    if (i % 25 == 0) {
      // Calculate label positions
      x0 = sx * (100 + tl + 10) + 120;
      y0 = sy * (100 + tl + 10) + 140;
      switch (i / 25) {
        case -2: tft.drawCentreString("0", x0 + x_adj, y0 - 12, 2); break;
        case -1: tft.drawCentreString("25", x0 + x_adj, y0 - 9, 2); break;
        case 0: tft.drawCentreString("50", x0 + x_adj, y0 - 6, 2); break;
        case 1: tft.drawCentreString("75", x0 + x_adj, y0 - 9, 2); break;
        case 2: tft.drawCentreString("100", x0 + x_adj, y0 - 12, 2); break;
      }
    }

    // Now draw the arc of the scale
    sx = cos((i + 5 - 90) * 0.0174532925);
    sy = sin((i + 5 - 90) * 0.0174532925);
    x0 = sx * 100 + 120;
    y0 = sy * 100 + 140;
    // Draw scale arc, don't draw the last part
    if (i < 50) tft.drawLine(x0 + x_adj, y0, x1 + x_adj, y1, TFT_BLACK);
  }


  tft.drawCentreString(header, 120 + x_adj, 70, 4); // Comment out to avoid font 4
  tft.drawRect(5 + x_adj, 3, 230, 119, TFT_BLACK); // Draw bezel line

  plotNeedle(0, 0, 240, header); // Put meter needle at 0
}




