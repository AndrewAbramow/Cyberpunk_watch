/*  
 Test the tft.print() viz embedded tft.write() function

 This sketch used font 2, 4, 7
 
 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 Note that yield() or delay(0) must be called in long duration for/while
 loops to stop the ESP8266 watchdog triggering.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */


#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <DS3231.h>
#include <Wire.h>
	 
//#define TFT_CS 5
//#define TFT_RST 23
//#define TFT_DC 16
	 
#define SDA 11
#define SCL 12
DS3231 myRTC;

	byte HOURS;
	byte L_HOURS = 0;
	byte MINS;
	byte L_MINS = 0;
	byte SECS;
	byte L_SECS = 0;
	byte DOW;
	byte L_DOW = 3;
	byte Date;
	byte L_Date = 4;
	byte Temp;
	byte L_Temp;
	bool h12Flag,pmFlag;  // Not Used Here
	bool century = false;
	String Dow []= {"0","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday" };
	byte Spc []= {0,49,34,14,38,49,38,49};  // X position of Weekday
	byte L_Month = 1;
	byte Months;

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

#define TFT_GREY 0x5AEB // New colour

void setup(void) {
  Serial.begin(57600);
	   Wire.begin();
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
	   tft.setTextColor(TFT_WHITE);
	   tft.setTextSize(4);
	   tft.drawRect(0,0,319,169,TFT_RED);
	   tft.drawLine(1,46,319,46,TFT_RED);
	   tft.drawLine(1,88,319,88,TFT_RED);
	   tft.setCursor(24,8);
	   tft.print("00:00:00");
	   //tft.setCursor(Spc[L_DOW],52);
	   //tft.setTextColor(TFT_GREEN);
	   //tft.print(Dow[L_DOW]);
	   //tft.setCursor(30,100);
	   //tft.setTextSize(3);
	   //tft.setTextColor(TFT_YELLOW);
	   //tft.print("04/");
	   //L_Month = myRTC.getMonth(century);
	   //PRINT_Month();
	   //tft.print("/");
	   if (century) {
	    //tft.print("2024");
	   }
	   else {
	    //tft.print("2023");   
	   }
	 
	   //tft.setTextSize(4);
}

void loop() {
  
  SECS = myRTC.getSecond();
	  if (L_SECS != SECS) {
	    tft.setTextColor(TFT_BLACK);
	    PRINT_SECS();
	    L_SECS = SECS;
	    tft.setTextColor(TFT_WHITE);
	    PRINT_SECS();
	    }
	  MINS = myRTC.getMinute();
	  if (L_MINS != MINS) {
	    tft.setTextColor(TFT_BLACK);
	    PRINT_MINS();
	    L_MINS = MINS;
	    tft.setTextColor(TFT_WHITE);
	    PRINT_MINS();
	  }
	  HOURS = myRTC.getHour(h12Flag,pmFlag);
	  if (L_HOURS != HOURS) {
	    tft.setTextColor(TFT_BLACK);
	    PRINT_HOURS();
	    L_HOURS = HOURS;
	    tft.setTextColor(TFT_WHITE);
	    PRINT_HOURS();
	  }
	  /*DOW = myRTC.getDoW();
	  if (L_DOW != DOW) {
	    tft.setTextColor(TFT_BLACK);
	    PRINT_DOW();
	    L_DOW = DOW;
	    tft.setTextColor(TFT_GREEN);
	    PRINT_DOW();
	  }
	  Date = myRTC.getDate();
	  if (L_Date != Date) {
	    tft.setTextColor(TFT_BLACK);
	    tft.setTextSize(3);
	    PRINT_Date();
	    L_Date = Date;
	    tft.setTextColor(TFT_YELLOW);
	    PRINT_Date();
	    tft.setTextSize(4);
	  }
	  Months = myRTC.getMonth(century);
	  if (L_Month != Months) {
	    tft.setTextColor(TFT_BLACK);
	    tft.setTextSize(3);
	    //PRINT_Month();
	    L_Month = Months;
	    tft.setTextColor(TFT_YELLOW);
	    //PRINT_Month();
	    tft.setTextSize(4);
	  }*/
}

void PRINT_SECS() {
	  tft.setCursor(168,8);
	  if (L_SECS < 10) {
	    tft.print("0");
	  }
	  tft.print(L_SECS);
	}
	 void PRINT_MINS() {
	  tft.setCursor(96,8);
	  if (L_MINS < 10) {
	    tft.print("0");
	  }
	  tft.print(L_MINS);
	 }
	 void PRINT_HOURS() {
	  tft.setCursor(24,8);
	  if (L_HOURS < 10) {
	    tft.print("0");
	  }
	  tft.print(L_HOURS);
	 }
	 void PRINT_DOW() {
	  tft.setCursor(Spc[L_DOW],52);
	  tft.print(Dow[L_DOW]);
	 }
	 void PRINT_Date() {
	  tft.setCursor(30,98);
	  if (L_Date < 10) {
	    tft.print("0");
	  }
	  tft.print(L_Date);
	 }
	 void PRINT_Month() {
	  tft.setCursor(84,100);
	  if (L_Month < 10) {
	   tft.print("0");
	      }
	  tft.print(L_Month);
	 }



