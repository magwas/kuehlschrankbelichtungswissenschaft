#include <Arduino.h>
#define INTERRUPT_PIN 2
#define LN2 0.6931471805599453
#define R1 1000.0
#define C1 100e-9
// https://en.wikipedia.org/wiki/555_timer_IC#/media/File:555_Astable_Diagram.svg
// + 1k output current limiter resistor
volatile uint32_t pulseCount = 0;
unsigned long measurementTime = 1000; // 1 second default

extern void pulseISR();
extern void measureSoil();
extern float calculateSoilResistance(float freq);

void setup() {
  pinMode(INTERRUPT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), pulseISR, RISING);
  Serial.begin(115200);
}

void pulseISR() {
  pulseCount++;
}

void measureSoil() {
  pulseCount = 0;
  unsigned long startTime = millis();

  // Auto-adjust measurement time based on expected frequency
  // Start with 1 second, extend if counting dry soil
  unsigned long measureDuration = 1000;

  while (millis() - startTime < measureDuration) {
    // Wait for measurement period
  }

  // If we got very few pulses, measure for longer
  if (pulseCount < 10 && measureDuration == 1000) {
    measureDuration = 10000; // 10 seconds for dry soil
    pulseCount = 0;
    startTime = millis();
    while (millis() - startTime < measureDuration) {
      // Wait for extended measurement
    }
  }

  float frequency = (float)pulseCount / (measureDuration / 1000.0);
  float soilResistance = calculateSoilResistance(frequency);

  Serial.print("Frequency: "); Serial.print(frequency, 1);
  Serial.print(" Hz, Soil R: "); Serial.print(soilResistance/1000, 1);
  Serial.println(" kΩ");
}

float calculateSoilResistance(float freq) {

  if (freq < 0.1) return 10000000.0; // Max value
// ⎡  r₁       1     ⎤
// ⎢- ── + ──────────⎥
// ⎣  2    2⋅c₁⋅f⋅ln₂⎦
   return 1/(2.0*C1*freq*LN2)- R1/2;

}