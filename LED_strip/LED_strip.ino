// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!

#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

#include <math.h>

#define NUMPIXELS 150 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    4
#define CLOCKPIN   5
//Adafruit_DotStar strip = Adafruit_DotStar(
//  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
//  Serial.begin(9600);
//  randomSeed(analogRead(0));
  strip.setBrightness(32);
}

// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.
const int
  numLEDs = 50;                 // on-LED width

int      head  = 0, tail = - numLEDs; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

int
  tempo = 10; // set tempo with poti?

bool goingUp = true;


void loop() {
 
 up_down(tempo);
// up_down_bounce(tempo);
  
}


void up_down(int tempo){
  strip.setPixelColor(head, color); 
  strip.setPixelColor(tail, 0, 0 ,0);     
  strip.show();                     
  delay(tempo);

  if(goingUp) {
    head++;
    if(++tail >= NUMPIXELS) {
      goingUp = false;
      head = NUMPIXELS;
      tail = NUMPIXELS + numLEDs;
    }
  }
  else {
    head--;
    if(--tail <= 0) {
      head = 0;
      tail = - numLEDs;
      goingUp = true;
      if((color >>= 8) == 0) color = 0xFF0000;
    }    
  }
}


void up_down_bounce(int tempo){
  //BUGGY
  strip.setPixelColor(head, color); 
  strip.setPixelColor(tail, 0, 0 ,0);     
  strip.show();                     
  delay(tempo);

  if(goingUp) {
    if(++head >= NUMPIXELS) {
      goingUp = false;
      head = tail;
      tail = NUMPIXELS;
    }
    tail++;
  }
  else {
    if(--head <= 0) {
      head = tail;
      tail = 0;
      goingUp = true;
      if((color >>= 8) == 0) color = 0xFF0000;
    }
    tail--;   
  }
}
