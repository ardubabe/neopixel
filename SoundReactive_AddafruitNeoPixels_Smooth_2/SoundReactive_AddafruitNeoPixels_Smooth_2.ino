#include <Adafruit_NeoPixel.h>

#define PIN 25        // Pin where the NeoPixel data line is connected
#define NUMPIXELS 1   // Number of NeoPixels

const int microphonePin = A0;  // Analog pin connected to the microphone's AOUT
const int digitalPin = 1;      // Digital pin connected to the microphone's D0

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t currentColor = 0; // Start with LEDs off (black)

// Function to blend between two colors
uint32_t blend(uint32_t color1, uint32_t color2, float ratio) {
  uint8_t r1 = (color1 >> 16) & 0xFF;
  uint8_t g1 = (color1 >> 8) & 0xFF;
  uint8_t b1 = color1 & 0xFF;

  uint8_t r2 = (color2 >> 16) & 0xFF;
  uint8_t g2 = (color2 >> 8) & 0xFF;
  uint8_t b2 = color2 & 0xFF;

  uint8_t r = r1 * (1 - ratio) + r2 * ratio;
  uint8_t g = g1 * (1 - ratio) + g2 * ratio;
  uint8_t b = b1 * (1 - ratio) + b2 * ratio;

  return pixels.Color(r, g, b);
}

void setup() {
  Serial.begin(9600);
  pinMode(digitalPin, INPUT);  // Set digital pin as input
  pixels.begin();
  pixels.show();  // Initialize all pixels to 'off'
}

void loop() {
  int mn = 1024;
  int mx = 0;

  for (int i = 0; i < 100; ++i) {
    int val = analogRead(microphonePin);
    mn = min(mn, val);
    mx = max(mx, val);
  }

  int delta = mx - mn;
  Serial.print("Delta: ");
  Serial.println(delta);

  // Read the digital output from the microphone
  int digitalVal = digitalRead(digitalPin);
  Serial.print("Digital: ");
  Serial.println(digitalVal);

  // Map delta to color range (0-5) and assign colors accordingly
  uint32_t targetColor;

  if (delta == 0) {
    targetColor = pixels.Color(255, 0, 255);  // Magenta
  } else if (delta >= 1 && delta <= 6) {
    targetColor = pixels.Color(255, 0, 255);  // Magenta
  } else {
    targetColor = pixels.Color(0, 255, 0);  // Green
  }

  // Smoothly transition to the target color
  currentColor = blend(currentColor, targetColor, 0.5);  // Change 0.5 to adjust smoothing speed (0.0 to 1.0)

  // Set the color for all NeoPixels
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, currentColor);
  }

  pixels.show();  // Update the NeoPixels with the new color

  delay(30);  // Small delay to maintain responsiveness
}
