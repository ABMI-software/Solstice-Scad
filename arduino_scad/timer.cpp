/*#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#define OLED_WIDTH 128 // OLED display width,  in pixels
#define OLED_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1); // // create SSD1306 display object connected to I2C
RTC_DS3231 rtc;

String time;
DateTime dt;


String DayOfTheWeek(uint8_t Day){
  String DayText;
  Serial.println(Day);
  if (Day==1)  DayText="Monday";
  if (Day==2)  DayText="Tuesday";
  if (Day==3)  DayText="Wednesday";
  if (Day==4)  DayText="Thursday";
  if (Day==5)  DayText="Friday";
  if (Day==6)  DayText="Saturday";
  if (Day==0)  DayText="Sunday";
  return DayText;
}

String DayMonthYear(uint8_t Day,uint8_t Month,uint16_t Year){
  String DayMonthYearText;
  if (Month==1)  DayMonthYearText="JAN ";
  if (Month==2)  DayMonthYearText="FEB ";
  if (Month==3)  DayMonthYearText="MAR ";
  if (Month==4)  DayMonthYearText="APR ";
  if (Month==5)  DayMonthYearText="MAY ";
  if (Month==6)  DayMonthYearText="JUN ";
  if (Month==7)  DayMonthYearText="JUL ";
  if (Month==8)  DayMonthYearText="AUG ";
  if (Month==9)  DayMonthYearText="SEP ";
  if (Month==10)  DayMonthYearText="OCT ";
  if (Month==11)  DayMonthYearText="NOV ";
  if (Month==12)  DayMonthYearText="DEC ";

  DayMonthYearText=DayMonthYearText+Day;
  if (Day==1)DayMonthYearText=DayMonthYearText+"st ";
  if (Day==2)DayMonthYearText=DayMonthYearText+"nd ";
  if (Day>2)DayMonthYearText=DayMonthYearText+"th ";

  DayMonthYearText=DayMonthYearText+Year;
  
  return DayMonthYearText;
}

String AddLeadingZero(uint8_t x){
  String AddLeadingZeroText;
  if(x<10) AddLeadingZeroText="0"; 
  else AddLeadingZeroText=""; 
  AddLeadingZeroText=AddLeadingZeroText+x;
  return AddLeadingZeroText;
}

String CurrentTime(uint8_t H, uint8_t I ){
  String CurrentTimeText="";
  CurrentTimeText=CurrentTimeText + AddLeadingZero(H) +":"+AddLeadingZero(I);
  return CurrentTimeText;
}

void setup() {
  Serial.begin(9600);
/*
  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display

  oled.setTextSize(1);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 10);        // position to display

  // SETUP RTC MODULE
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (true);
  }

  // automatically sets the RTC to the date & time on PC this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  time.reserve(10); // to avoid fragmenting memory when using String
}

void loop() {
  dt = rtc.now();
/*
  // oled_display_center(time);
  oled.fillRect(0,0,128,16,SSD1306_WHITE);
  oled.fillRect(0,17,128,16,SSD1306_BLACK);
  oled.fillRect(0,31,128,33,SSD1306_WHITE);

  oled.setCursor(1,1); 
  oled.setTextSize(2);
  oled.setTextColor(SSD1306_BLACK); 
  oled.println(DayOfTheWeek(dt.dayOfTheWeek()));

  oled.setCursor(1,18); 
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE); 
  oled.println(DayMonthYear(dt.day(),dt.month(),dt.year()));

  oled.setCursor(3,35); 
  oled.setTextSize(3);  
  oled.setTextColor(SSD1306_BLACK); 
  oled.println(CurrentTime(dt.hour(),dt.minute()));

  oled.setCursor(100,35); 
  oled.setTextSize(2);
  oled.setTextColor(SSD1306_BLACK); 
  oled.println(AddLeadingZero(dt.second()));

  oled.display();
  delay(1000);
  
}

*/
