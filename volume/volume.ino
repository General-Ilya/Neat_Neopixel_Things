#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 4

//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  //pinMode(5, OUTPUT);
  //Serial.begin(9600);
  strip.begin();

  strip.show(); // Initialize all pixels to 'off'
}
uint8_t colour;
const int buffer_size = 64;
int samples[buffer_size];
int sample;

void loop()
{
  for(int i = 0; i < buffer_size-1; i++){
    samples[i] = samples[i+1];
  }
  int input = analogRead(1);
  samples[buffer_size-1] = input;
  for(int i = 0; i < buffer_size; i++){
    sample += samples[i];
  }
  sample = sample/buffer_size;

  
  colour+=sample/400;
  strip.setBrightness(min(log(sample)*255,255));
//    if(sample>700){
//      colour+=15;
//      delay(100);
//    }
    //colour += 1;
    uint32_t c = hsl(50, 50, colour);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + colour) & 255));

    }
    strip.show();
    //delay(50);
//  }


}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint32_t hsl(uint16_t ih, uint8_t is, uint8_t il) {

  float h, s, l, t1, t2, tr, tg, tb;
  uint8_t r, g, b;

  h = (ih % 360) / 360.0;
  s = constrain(is, 0, 100) / 100.0;
  l = constrain(il, 0, 100) / 100.0;

  if ( s == 0 ) {
    r = g = b = 255 * l;
    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  }

  if ( l < 0.5 ) t1 = l * (1.0 + s);
  else t1 = l + s - l * s;

  t2 = 2 * l - t1;
  tr = h + 1 / 3.0;
  tg = h;
  tb = h - 1 / 3.0;

  r = hsl_convert(tr, t1, t2);
  g = hsl_convert(tg, t1, t2);
  b = hsl_convert(tb, t1, t2);

  // NeoPixel packed RGB color
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}
uint8_t hsl_convert(float c, float t1, float t2) {

  if ( c < 0 ) c += 1;
  else if ( c > 1 ) c -= 1;

  if ( 6 * c < 1 ) c = t2 + ( t1 - t2 ) * 6 * c;
  else if ( 2 * c < 1 ) c = t1;
  else if ( 3 * c < 2 ) c = t2 + ( t1 - t2 ) * ( 2 / 3.0 - c ) * 6;
  else c = t2;

  return (uint8_t)(c * 255);
}

