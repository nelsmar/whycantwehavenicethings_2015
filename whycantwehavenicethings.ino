#include <FastLED.h>

#define BUTTON_PIN  2
#define LED_PIN_1   3
#define LED_PIN_2   4
#define LED_PIN_3   5
#define LED_PIN_4   6
#define LED_PIN_5   7
#define LED_PIN_6   8
#define LED_PIN_7   12
#define LED_PIN_8   13

#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_STRIPS  1
#define NUM_LEDS    75

#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 60
#define SECONDS_PER_CYCLE 120

int program = 5;
uint8_t palIndex = 0;
uint8_t indexSpeed = 1;
uint8_t indexSize = 2;
bool isPaused = false;
int buttonCount = 0;

// Variables will change:
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

CRGB leds[NUM_STRIPS][NUM_LEDS];
//CRGB leds_1[NUM_LEDS];
//CRGB leds_2[NUM_LEDS];
//CRGB leds_3[NUM_LEDS];
//CRGB leds_4[NUM_LEDS];

// Fire2012 with programmable Color Palette
//
// This code is the same fire simulation as the original "Fire2012",
// but each heat cell's temperature is translated to color through a FastLED
// programmable color palette, instead of through the "HeatColor(...)" function.
//
// Four different static color palettes are provided here, plus one dynamic one.
// 
// The three static ones are: 
//   1. the FastLED built-in HeatColors_p -- this is the default, and it looks
//      pretty much exactly like the original Fire2012.
//
//  To use any of the other palettes below, just "uncomment" the corresponding code.
//
//   2. a gradient from black to red to yellow to white, which is
//      visually similar to the HeatColors_p, and helps to illustrate
//      what the 'heat colors' palette is actually doing,
//   3. a similar gradient, but in blue colors rather than red ones,
//      i.e. from black to blue to aqua to white, which results in
//      an "icy blue" fire effect,
//   4. a simplified three-step gradient, from black to red to white, just to show
//      that these gradients need not have four components; two or
//      three are possible, too, even if they don't look quite as nice for fire.
//
// The dynamic palette shows how you can change the basic 'hue' of the
// color palette every time through the loop, producing "rainbow fire".

CRGBPalette16 gPal;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void clearAll()
{
  FastLED.clear();
//  for(int i = 0; i < NUM_LEDS; i++) {
//    leds_1[i] = CRGB::Black;
//    leds_2[i] = CRGB::Black;
//    leds_3[i] = CRGB::Black;
//  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT);    // button as input
  digitalWrite(BUTTON_PIN, HIGH); // turns on pull-up resistor after input
  delay(3000); // sanity delay
//  FastLED.addLeds<CHIPSET, LED_PIN_1, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
//  FastLED.addLeds<CHIPSET, LED_PIN_2, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
//  FastLED.addLeds<CHIPSET, LED_PIN_3, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection( TypicalLEDStrip );
//  FastLED.addLeds<CHIPSET, LED_PIN_4, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_1, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_2, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_3, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_4, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_6, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
//  FastLED.addLeds<CHIPSET, LED_PIN_6, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
//  FastLED.addLeds<CHIPSET, LED_PIN_7, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
//  FastLED.addLeds<CHIPSET, LED_PIN_8, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  // This first palette is the basic 'black body radiation' colors,
  // which run from black to red to bright yellow to white.
  gPal = HeatColors_p;
  
  // These are other ways to set up the color palette for the 'fire'.
  // First, a gradient from black to red to yellow to white -- similar to HeatColors_p
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  
  // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  
  // Third, here's a simpler, three-step gradient, from black to red to white
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);

  Serial.begin(9600);
  Serial.println("Program started...!");

  // Setup for rainbow
  currentPalette = RainbowColors_p;
  currentBlending = BLEND;
}

void loop()
{
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  
  // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  
  // Third, here's a simpler, three-step gradient, from black to red to white
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);
  

  // Fourth, the most sophisticated: this one sets up a new palette every
  // time through the loop, based on a hue that changes every time.
  // The palette is a gradient from black, to a dark color based on the hue,
  // to a light color based on the hue, to white.
  //
  //   static uint8_t hue = 0;
  //   hue++;
  //   CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
  //   CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
  //   gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);


  if(program < 4 && program > 0) {
    for(int i = 0; i < NUM_STRIPS; i++)
    {
      static uint8_t hue = 0;
      hue++;
      CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
      CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
      gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);
      Fire2012WithPalette(leds[i]); // run simulation frame, using palette colors
      
      uint8_t secondHand = (millis() / 1000) % SECONDS_PER_CYCLE;
      static uint8_t lastSecond = 99;
      if( lastSecond != secondHand && ! isPaused) {
        lastSecond = secondHand;
        if(lastSecond >= SECONDS_PER_CYCLE-1)
        {
          program = 5;
          Serial.print("switching back to rainbow theme");
        }
      }
    } 
    FastLED.show(); // display this frame
  }

  if (program == 4) {
    for(int i = 0; i < NUM_STRIPS; i++)
    {
//      BouncyBall(leds[i]);  
    }
  }

  if (program == 5) {
    ChangePalettePeriodically();
    static uint8_t startIndex = 0;
    startIndex = startIndex + indexSpeed; /* motion speed */
  
    for(int i = 0; i < NUM_STRIPS; i++)
    {
      FillLEDsFromPaletteColors( startIndex, leds[i]);  
    }
  
    FastLED.show();
  }

  if(program == 6)
  {
    ChangePalettePeriodically();
//    static uint8_t index = 0;
//    index = index + 1;
    spin();
    FastLED.show();
  }

  FastLED.delay(1000 / FRAMES_PER_SECOND);  

  // See if we reveived any serial commands to change our program...
  serialCommand();
  checkButton();
}

void checkButton()
{
  // read the state of the switch into a local variable:
  int reading = digitalRead(BUTTON_PIN);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        buttonCount++;
        if(buttonCount > 1)
        {
          Serial.println("Button push!");
          palIndex++;
          Serial.println("Changing mode. Pausing until next reboot...");
          isPaused = true; 
          if(program == 1) {
            program = 5; 
          }
        }
        
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

void spin()
{
  uint8_t brightness = 255;
  static uint8_t index = 0;
  
  for(int s = 0; s < NUM_STRIPS; s++)
  {
    for(int i = 0; i < NUM_LEDS; i++)
    {
      leds[s][i] = ColorFromPalette( currentPalette, index, brightness, currentBlending);
    }
    index += 3;
  }

//  FastLED.show();
}

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


void Fire2012WithPalette(CRGB *leds)
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      leds[j] = ColorFromPalette( gPal, colorindex);
    }
}

/*********
 * This is the color wheel scripting?
 */

void FillLEDsFromPaletteColors( uint8_t colorIndex, CRGB *leds)
{
  uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
//    colorIndex += 2;
    colorIndex += indexSize;
  }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % SECONDS_PER_CYCLE;
  static uint8_t lastSecond = 99; 
//  static uint8_t index = 0;
  if( lastSecond != secondHand && ! isPaused) {
    lastSecond = secondHand;
    if(lastSecond >= SECONDS_PER_CYCLE-1)
    {
      palIndex++;
      Serial.print("-palet index: ");
      Serial.println(palIndex);
    }
  }
  if(palIndex > 10)
  {
    palIndex = 0;
    Serial.print("-palet index: ");
    Serial.println(palIndex);
    program = 1;
  }
  
  if( palIndex ==  0)  { currentPalette = RainbowColors_p;         currentBlending = BLEND; }
  if( palIndex ==  1)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
  if( palIndex ==  2)  { currentPalette = RainbowStripeColors_p;   currentBlending = BLEND; }
  if( palIndex ==  3)  { SetupPurpleAndGreenPalette();             currentBlending = BLEND; }
//  if( palIndex ==  4)  { SetupTotallyRandomPalette();              currentBlending = BLEND; }
  if( palIndex ==  5)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
  if( palIndex ==  6)  { SetupBlackAndWhiteStripedPalette();       currentBlending = BLEND; }
  if( palIndex ==  7)  { currentPalette = CloudColors_p;           currentBlending = BLEND; }
  if( palIndex ==  8)  { currentPalette = PartyColors_p;           currentBlending = BLEND; }
  if( palIndex ==  9)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
  if( palIndex == 10)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = BLEND; }
  
//  if( lastSecond != secondHand) {
//    lastSecond = secondHand;
//    if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = BLEND; }
//    if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
//    if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = BLEND; }
//    if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = BLEND; }
//    if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = BLEND; }
//    if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
//    if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = BLEND; }
//    if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = BLEND; }
//    if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = BLEND; }
//    if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
//    if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = BLEND; }
//  }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
  for( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;
  
  currentPalette = CRGBPalette16( 
    green,  green,  black,  black,
    purple, purple, black,  black,
    green,  green,  black,  black,
    purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more 
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

void serialCommand()
{
  int incomingByte;      // a variable to read incoming serial data into
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == '0') {
      program = 0;
      Serial.println("Default program Starting...");
      clearAll();
    }
    if (incomingByte == '1') {
      program = 1;
      Serial.println("Program 1 Starting...");
      gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == '2') {
      program = 2;
      Serial.println("Proram 2 Starting...");
      gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == '3') {
      program = 3;
      Serial.println("Program 3 Starting...");
      gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);
    }
    if (incomingByte == '4') {
      program = 4;
      Serial.println("Program 4 Starting...");
      clearAll();
    }
    if (incomingByte == '5') {
      program = 5;
      Serial.println("Program 5 Starting...");
      clearAll();
    }
    if (incomingByte == '6') {
      program = 6;
      Serial.println("Program 6 Starting...");
      clearAll();
    }
    if(incomingByte == '`') {
      palIndex++;
      Serial.println("Changing color palet");
    }
    if(incomingByte == '+') {
      indexSpeed++;
      Serial.println("Increasing Speed");
    }
    if(incomingByte == '-') {
      indexSpeed --;
      Serial.println("Decreasing Speed");
    }
    if(incomingByte == ']') {
      indexSize++;
      Serial.println("Increasing Size");
    }
    if(incomingByte == '[') {
      indexSize --;
      Serial.println("Decreasing Size");
    }
    if(incomingByte == 'x') {
      indexSize = 2;
      indexSpeed = 1;
      Serial.println("Resetting speed & size");
    }
    static uint8_t pSize = indexSize;
    static uint8_t pSpeed = indexSpeed;
    if(incomingByte == 'p') {      
      isPaused = !isPaused;
      if(isPaused) {
        Serial.println("Pausing");
        pSpeed = indexSpeed;
        indexSpeed = 0;
      } else {
        Serial.println("Play");
        indexSpeed = pSpeed;
      }
    }
  }
}

