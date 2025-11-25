#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

// Includes
#include "FastLED.h"
#include <IRremote.hpp>
#include <Stepper.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Defines
#define PIN 5
#define NUM_LEDS 59
//#define NUM_LEDS 300

#ifndef STASSID
#define STASSID "linksys"        // Wifi Name 
#define STAPSK "brina123"             // Wifi Password
#endif

// Global variables
bool isUnlocking = false;             // Boolean for unlocking the box
bool isFire = false;                  // Boolean for continuous fire
int stepsToMove = 512;                // Number of steps for 90 degrees
int stepCounter = 0;                  // Step counter for rotating the stepper
int angle = 90;                        // angle variable to keep track of the position 
int brightness = 255;                 // Initial brightness
uint8_t c_index = 127;                // Color index for cylcing
const int stepsPerRevolution = 2048;  // Steps per one revolution of the stepper
const int RECV_PIN = 12;              // Pin for IR 
const char* ssid = STASSID;           // Wifi Login credentials 
const char* password = STAPSK;        // Wifi Login credentials 


const int NUM_SPARKS = 20; // max number (could be NUM_LEDS / 2);
const int maxPos = (NUM_LEDS-1) * 128 ;

int sparkPos[3][NUM_SPARKS] ;
int sparkVel[3][NUM_SPARKS] ;
int sparkHeat[3][NUM_SPARKS];
int flarePos[3];
bool flareShot[3] = {false,false,false} ;
byte nSparks[3];


CRGBPalette16 gPal1;
CRGBPalette16 gPal2;
CRGBPalette16 gPal3;
 
DEFINE_GRADIENT_PALETTE( testp ) {
0, 0, 0,0, 
32, 8, 0, 0, 
64, 16,0, 0, 
96, 64,32, 0, 
128,128,80, 0, 
160, 160,100,0, 
192, 192, 192,0, 
224, 255, 255, 255, 
255, 200, 200, 255};

CRGBPalette16 oceanPalette;
TBlendType    currentBlending;

// CRGBPalette16 myPalette;

// Define a color palette for a dark and misty ocean
// CRGBPalette16 oceanPalette = CRGBPalette16(
//     CRGB::MidnightBlue, CRGB::Navy, CRGB::DarkBlue, CRGB::Black,
//     CRGB::MidnightBlue, CRGB::DarkSlateGray, CRGB::DimGray, CRGB::Black,
//     CRGB::DarkBlue, CRGB::Navy, CRGB::MidnightBlue, CRGB::Black,
//     CRGB::DarkSlateGray, CRGB::DimGray, CRGB::DarkBlue, CRGB::Black
// );


CRGBPalette16 myPalette(
    CRGB::Black,
    CRGB::Black,
    CRGB::Red,
    CRGB::Yellow,
    CRGB::Green,
    CRGB::Blue,
    CRGB::Purple,
    CRGB::Black,
 
    0x100000,
    0x200000,
    0x400000,
    0x800000,
 
    CHSV( 30,255,255),
    CHSV( 50,255,255),
    CHSV( 70,255,255),
    CHSV( 90,255,255)
);




Stepper myStepper = Stepper(stepsPerRevolution, 2, 4, 0, 5);  // in1, in3, in2, in4
CRGB leds[NUM_LEDS];


void setup() {
  delay(1000);                        
  Serial.begin(9600);                 // Serial communication at 9600 baud
  Serial.println("Serial begin");    

  // ////// WIFI Connection Stuff
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  // while (WiFi.waitForConnectResult() != WL_CONNECTED) {
  //   Serial.println("Connection Failed! Rebooting...");
  //   delay(5000);
  //   ESP.restart();
  // } 
  // ArduinoOTA.onStart([]() {
  //   String type;
  //   if (ArduinoOTA.getCommand() == U_FLASH) {
  //     type = "sketch";
  //   } else {  // U_FS
  //     type = "filesystem";
  //   }

  //   // NOTE: if updating FS this would be the place to unmount FS using FS.end()
  //   Serial.println("Start updating " + type);
  // });
  // ArduinoOTA.onEnd([]() {
  //   Serial.println("\nEnd");
  // });
  // ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  //   Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  // });
  // ArduinoOTA.onError([](ota_error_t error) {
  //   Serial.printf("Error[%u]: ", error);
  //   if (error == OTA_AUTH_ERROR) {
  //     Serial.println("Auth Failed");
  //   } else if (error == OTA_BEGIN_ERROR) {
  //     Serial.println("Begin Failed");
  //   } else if (error == OTA_CONNECT_ERROR) {
  //     Serial.println("Connect Failed");
  //   } else if (error == OTA_RECEIVE_ERROR) {
  //     Serial.println("Receive Failed");
  //   } else if (error == OTA_END_ERROR) {
  //     Serial.println("End Failed");
  //   }
  // });
  // ArduinoOTA.begin();
  // Serial.println("Ready");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());
  // //// WIFI Connection Stuff

  IrReceiver.begin(RECV_PIN);
  Serial.println("IR receiver enabled");
  
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  Serial.println("LEDs started");
  oceanPalette = OceanColors_p;
  currentBlending = LINEARBLEND;

  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(0, LOW);
  digitalWrite(5, LOW);
  Serial.println("Stepper initialized.");
  setAll(100, 100, 100);
  gPal1 = testp;
  gPal2 = CRGBPalette16( CRGB::Black, CRGB::DarkBlue, CRGB::Aqua,  CRGB::White);
  gPal3 = CRGBPalette16( CRGB::Black, CRGB::DarkGreen, CRGB::Yellow, CRGB::White);
 
  meteorRain(0x56, 0x69, 0xCC, 10, 64, true, 1);


}

byte nrv;

void loop() {
//  ArduinoOTA.handle();
  // uint32_t irValue = IrReceiver.decodedIRData.decodedRawData;
  // Serial.println(irValue, HEX);
  if (IrReceiver.decode()) {
    uint32_t irValue = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(irValue, HEX);
    switch (irValue) {
      case 0xF30CFF00: //small remote commands
      case 0xED121DCC:
      case 0xEC13EF00:
        // Expecto Patronum
        meteorRain(0x56, 0x69, 0xCC, 6, 64, true, 15);
        break;

      case 0xE718FF00:
      case 0xFF001DCC:
      case 0xE817EF00:
        // Expelliarmos
        meteorRain(0xC2, 0x17, 0x17, 5, 64, false, 5);
        break;

      case 0xF609FF00:
      case 0x97681DCC:
      case 0xF10EEF00:
        // Lumos
        FadeIn(0x56, 0x69, 0xCC, 10);
        break;

      case 0xF807FF00:
      case 0xEC131DCC:
      case 0xED12EF00:
        // Nox
        FadeOut(0x56, 0x69, 0xCC, 10);
        break;

      case 0xB847FF00:
      case 0xA15E1DCC:
      case 0xF50AEF00:      
        // Lumos Maxima
        FadeInMax(0x56, 0x69, 0xCC, 1);
        break;

      case 0xA15EFF00:
      case 0x9F601DCC:
      case 0xF00FEF00:
        Sparkle(0xff, 0xff, 0xff, 0);
        break;

      case 0xF708FF00:
      case 0xFD021DCC:
      case 0xF609EF00:
        rainbowCycle(5);
        break;

      case 0xE31CFF00:
      case 0xF40B1DCC:
      case 0xEF10EF00:
        dynamicLightWave();
        break;

      case 0xF20DFF00:
      case 0xA35C1DCC: 
      case 0xF30CEF00:
        // Alohomora
        isUnlocking = true;
        break;

      case 0xA55AFF00:
      case 0x9D621DCC:
      case 0xEB14EF00:
        // Incendio 
        setAll(0, 0, 0);
        for (int i = 0; i < 400; i++) {   // This specifies the duration 
            Fire(110, 120, 35);
          }
          break;  

      case 0xBA45FF00:
      case 0xF708FB04:
      case 0xFD02EF00:
        // Turn all off 
        setAll(0, 0, 0);
        break;

      case 0xBD42FF00:
      case 0xF00F1DCC:
      case 0xF708EF00:
        for (int i = 0; i < 100; i++) {
          FillLEDsFromPaletteColorsOcean(i);
          FastLED.delay(20);
        }
        break;

      case 0xBB44FF00: // Left for color cycle left
      case 0xA25D1DCC: // Left for color cycle left
      case 0xFB04EF00:
        c_index -= 25;
        FillLEDsFromPaletteColors(c_index);
        FastLED.show();
        break;

      case 0xBC43FF00: // Right for color cycle right
      case 0xEE111DCC: // Right for color cycle right
      case 0xFA05EF00:
        c_index += 25;
        FillLEDsFromPaletteColors(c_index);
        FastLED.show();
        break;

      case 0xEA15FF00: // Down for lower brightness
      case 0xF8071DCC: // Down for lower brightness
      case 0xFE01EF00:
        if (brightness > 0) {
          brightness -= 51;
          Serial.println(brightness);
          FastLED.setBrightness(brightness);
          FastLED.show();
        }
        break;

      case 0xB946FF00: // Up for higher brightness
      case 0x77881DCC: // Up for higher brightness
      case 0xFF00EF00:
        if (brightness < 255) {
          brightness += 51;
          Serial.println(brightness);
          FastLED.setBrightness(brightness);
          FastLED.show();
        }
        break;
        
      case 0xB54AFF00:
      case 0xE916EF00:
       while (1) {   
            Fire(65, 120, 35);
          }
          break;  

      case 0xF40BEF00:
        EVERY_N_MILLIS(10) { 
          // random16_add_entropy( random());
          if  (not flareShot[0])   shoot(0);
          if  (not flareShot[1])   shoot(1);
          if  (not flareShot[2])   shoot(2);
      
        }
        EVERY_N_MILLIS(10) {     
        if  (flareShot[0])  doSparks(0);
        if   (flareShot[1]) doSparks(1);
        if   (flareShot[2]) doSparks(2);
      } 
      
        EVERY_N_MILLIS(10) {  
      
          FastLED.show();
        // FastLED.clear();
          fadeToBlackBy(leds,NUM_LEDS,80);
        }
          break;  
        
      default:
        // Handle unknown or unhandled IR codes
        break;
    } 

    IrReceiver.resume();
  }
  
  unlockBox();
}



void unlockBox() {
  if (isUnlocking) {
    if (stepCounter < stepsToMove) {
      myStepper.setSpeed(10);

      // Determine the direction based on the current angle
      if (angle == 0) {
        myStepper.step(1);  // Move one step clockwise
      } else if (angle == 90) {
        myStepper.step(-1);  // Move one step counterclockwise
      }

      stepCounter++;
    } else {
      // Update the angle based on the direction of rotation
      if (angle == 0) {
        angle = 90;
      } else if (angle == 90) {
        angle = 0;
      }
      digitalWrite(2, LOW);
      digitalWrite(4, LOW);
      digitalWrite(0, LOW);
      digitalWrite(5, LOW);
      Serial.println("All should be off");
      isUnlocking = false;
      stepCounter = 0;  // Reset step counter for next time
    }
  }
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {
  setAll(0, 0, 0);

  for (int i = 0; i < 2*NUM_LEDS; i++) {
    // fade brightness all LEDs one step
    for (int j = 0; j < NUM_LEDS; j++) {
      if ((!meteorRandomDecay) || (random(10) > 5)) {
        fadeToBlack(j, meteorTrailDecay);
      }
    }

    // draw meteor
    for (int j = 0; j < meteorSize; j++) {
      if ((i - j < NUM_LEDS) && (i - j >= 0)) {
        setPixel(i - j, red, green, blue);
        // setPixel(NUM_LEDS - (i - j), red, green, blue);
      }
    }

    showStrip();
    delay(SpeedDelay);
  }
  setAll(0, 0, 0);
}

 
 
void shoot(byte nr) {
 
  if (random(1000) <10) {
    flareShot[nr]=true;
 
  flarePos[nr]=random(40,NUM_LEDS-40);
  nSparks[nr]=30;
  // initialize sparks
  for (int x = 0; x < nSparks[nr]; x++) { 
    sparkPos[nr][x] = flarePos[nr]<<7;
    sparkVel[nr][x] = random16(0, 5120)-2560;  // velocitie original -1 o 1 now -255 to + 255
    word sph = abs(sparkVel[nr][x])<<2;
    if (sph>2550) sph=2550;  // set heat before scaling velocity to keep them warm heat is 0-500 but then clamped to 255
    sparkHeat[nr][x]=sph ;
  } 
  sparkHeat[nr][0] = 5000; // this will be our known spark 
  }
}
 
void doSparks(byte nr){
  for (int x = 0; x < nSparks[nr]; x++) { 
    sparkPos[nr][x] = sparkPos[nr][x] + (sparkVel[nr][x]>>5);   // adjust speed of sparks here
    sparkPos[nr][x]=constrain(sparkPos[nr][x],0,maxPos);
    sparkHeat[nr][x]=scale16(sparkHeat[nr][x],64000);   // adjust speed of cooldown here

    CRGB color;
    if (nr==0) color = ColorFromPalette( gPal1  , scale16(sparkHeat[nr][x],6600));
    if (nr==1) color = ColorFromPalette( gPal2  , scale16(sparkHeat[nr][x],6600));
    if (nr==2) color = ColorFromPalette( gPal3  , scale16(sparkHeat[nr][x],6600));


    leds[sparkPos[nr][x]>>7]+=color;
    if (sparkHeat[nr][0] < 1) {
        flareShot[nr]=false;  // this fireworks is done
      }
  }
}


void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256; j++) {  // 1 cycle of all colors on wheel
    for (i = 0; i < NUM_LEDS; i++) {
      c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2));
    }
    showStrip();
    delay(SpeedDelay);
  }
  setAll(0, 0, 0);
}

byte *Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  for (int i = 0; i < 550; i++) {
    int Pixel = random(NUM_LEDS);
    setPixel(Pixel, red, green, blue);
    showStrip();
    delay(SpeedDelay);
    setPixel(Pixel, 0, 0, 0);
  }
  setAll(0, 0, 0);
}

void FadeOut(byte red, byte green, byte blue, int SpeedDelay) {
  float r, g, b;
  for (int k = 15; k >= 0; k = k - 2) {
    r = (k / 256.0) * red;
    g = (k / 256.0) * green;
    b = (k / 256.0) * blue;
    setAll(r, g, b);
    showStrip();
    delay(SpeedDelay);
  }
}

void FadeIn(byte red, byte green, byte blue, int SpeedDelay) {
  float r, g, b;
  for (int k = 0; k < 15; k = k + 1) {
    r = (k / 256.0) * red;
    g = (k / 256.0) * green;
    b = (k / 256.0) * blue;
    setAll(r, g, b);
    showStrip();
    delay(SpeedDelay);
  }
}

void FadeInMax(byte red, byte green, byte blue, int SpeedDelay) {
  float r, g, b;
  for (int k = 15; k < 256; k = k + 15) {
    r = (k / 256.0) * red;
    g = (k / 256.0) * green;
    b = (k / 256.0) * blue;
    setAll(r, g, b);
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
  #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;

    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r = (r <= 10) ? 0 : (int)r - (r * fadeValue / 256);
    g = (g <= 10) ? 0 : (int)g - (g * fadeValue / 256);
    b = (b <= 10) ? 0 : (int)b - (b * fadeValue / 256);

    strip.setPixelColor(ledNo, r, g, b);
  #endif
  #ifndef ADAFRUIT_NEOPIXEL_H
    // FastLED
    leds[ledNo].fadeToBlackBy(fadeValue);
  #endif
}

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[NUM_LEDS];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for (int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if (random(255) < Sparking) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }

  // // Step 4.  Convert heat to LED colors
  // for (int j = 0; j < NUM_LEDS; j++) {
  //   setPixelHeatColor(j, heat[j]);
  //   // setPixelHeatColor(NUM_LEDS/2 + j, heat[j]);
  //   // setPixelHeatColor(NUM_LEDS/2 - j, heat[j]);
  //   // setPixelHeatColor(NUM_LEDS - j, heat[j]);
  // }

  // Step 4.  Convert heat to LED colors
  for (int j = 0; j < 30; j++) {
    setPixelHeatColor(j, heat[j]);
    setPixelHeatColor(NUM_LEDS - j, heat[j]);
    // setPixelHeatColor(NUM_LEDS - j, heat[j]);
  }

  showStrip();
  delay(SpeedDelay);
  // setAll(0, 0, 0);
}

void setPixelHeatColor(int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F;  // 0..63
  heatramp <<= 2;               // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if (t192 > 0x80) {  // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if (t192 > 0x40) {  // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {  // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  uint8_t brightness = 255;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(myPalette, colorIndex, brightness, currentBlending);
  }
  showStrip();
}

void FillLEDsFromPaletteColorsOcean(uint8_t colorIndex) {
  uint8_t brightness = 255;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(oceanPalette, colorIndex, brightness, currentBlending);
    colorIndex += 1;
  }
  showStrip();
}

void dynamicLightWave() {
  static uint8_t hue = 0;
  int waveLength = NUM_LEDS / 4; // Length of the wave
  int waveHeight = 128; // Brightness of the wave peak

  for(int k = 0; k < 300; k++) {

  // Gradually change the hue for a rainbow effect
  hue++;

  for(int i = 0; i < NUM_LEDS; i++) {
    // Calculate the brightness for each LED based on a sine wave
    int brightness = waveHeight * (1 + sin(i * 2 * PI / waveLength + millis() / 1000.0)) / 2;

    // Set the LED color based on the Hue, Saturation, and Brightness (HSB/HSV model)
    leds[i] = CHSV(hue, 255, brightness);
  }

  // Update the LEDs
  FastLED.show();

  // Short delay to control wave speed
  delay(20);
  }
}


// TOOO HERRREE

void showStrip() {
  #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    strip.show();
  #endif
  #ifndef ADAFRUIT_NEOPIXEL_H
    // FastLED
    FastLED.show();
  #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
  #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    strip.setPixelColor(Pixel, strip.Color(red, green, blue));
  #endif
  #ifndef ADAFRUIT_NEOPIXEL_H
    // FastLED
    leds[Pixel].r = red;
    leds[Pixel].g = green;
    leds[Pixel].b = blue;
  #endif
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}
