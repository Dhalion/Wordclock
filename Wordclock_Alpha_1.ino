// NeoPixel Stuff -->
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define INTRO false // Enable for Colorful Intro :D
#define PIN 9

// DCF77 Clock Stuff -->
#include "DCF77.h"
#include "TimeLib.h"
#define DCF_PIN 2           // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0    // Interrupt number associated with pin
time_t prevDisplay = 0;          // when the digital clock was displayed
time_t DCFtime;  // Define TIME

DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);  // Define Object DCF77 (Clock)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(110, PIN, NEO_GRB + NEO_KHZ800); // Define Object NeoPixel RGB Strip


// Variables
int i;


// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  Serial.begin(74880); 
  //DCF.Start();
  setSyncInterval(30);
  setSyncProvider(getDCFTime);
  Serial.println("Waiting for DCF77 time ... ");
  Serial.println("It will take at least 2 minutes until a first update can be processed.");
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  if (INTRO == true){
    theaterChase(strip.Color(127, 127, 127), 70); // White
    theaterChase(strip.Color(127, 0, 0), 70); // Red  
    theaterChase(strip.Color(0, 0, 127), 70); // Blue
    theaterChase(strip.Color(0, 127, 0), 70); // Blue
    for (int i = 1; i < 5;i++)
    {
      rainbow(5);  
    } 
    delay(500);
    colorWipe((0,0,0), 1);

    
    while (DCFtime == 0)
    {
      time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
      if (DCFtime!=0)
      {
        Serial.println("Time is updated");
        setTime(DCFtime);
      }  
      digitalClockDisplay();
      // wait until the time is set by the sync provider     
       Serial.print(".");
      for (i = 37; i < 41; i++)
      {
        strip.setPixelColor(i, 255, 165, 0);
        strip.show();
        Serial.print(".");
      }
      delay(500);
      for (i = 37; i < 41; i++)
      {
        strip.setPixelColor(i, 0, 0, 0);
        strip.show();
      }
      delay(500);    
    }
  }
}

void loop() {
  delay(1000);
  time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
  if (DCFtime!=0)
  {
    Serial.println("Time is updated");
    setTime(DCFtime);
  }  
  digitalClockDisplay(); 
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.println("");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

unsigned long getDCFTime()
{ 
  time_t DCFtime = DCF.getTime();
  // Indicator that a time check is done
  if (DCFtime!=0) {
    Serial.print("X");  
  }
  return DCFtime;
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
