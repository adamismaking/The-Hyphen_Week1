/**
 * THE HYPHEN - LOW HUM VIBRATION VERSION
 * Using micro-pulse modulation to create a quiet, low-frequency hum.
 */

#include <Arduino.h>

const int TILT_PIN = 2;
const int BUZZER_PIN = 9;

float tension = 0.0;
const float GROWTH_RATE = 0.010; 
const float DECAY_RATE = 0.015;  
const float MAX_TENSION = 1.0;
const float MIN_TENSION = 0.0;

unsigned long lastHumTime = 0;
unsigned long lastUpdate = 0;
const int UPDATE_INTERVAL = 10; 

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);

void setup() {
  pinMode(TILT_PIN, INPUT_PULLUP); 
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("SYSTEM:HARDWARE_DEMO_V1_READY");
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long currentMicros = micros();

  // 1. HARDWARE READ
  bool isTilted = (digitalRead(TILT_PIN) == HIGH);

  // 2. TENSION MATH
  if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = currentTime;
    
    digitalWrite(LED_BUILTIN, isTilted ? HIGH : LOW);

    if (isTilted) {
      tension += GROWTH_RATE * (1.0 + tension); 
    } else {
      tension -= DECAY_RATE;
    }
    tension = constrain(tension, MIN_TENSION, MAX_TENSION);

    static unsigned long lastSerialSend = 0;
    if (currentTime - lastSerialSend >= 33) {
      lastSerialSend = currentTime;
      Serial.print("T:"); 
      Serial.println(tension, 3);
    }
  }

  // 3. LOW HUM MODULATION
  if (tension > 0.05) {
    // We calculate the gap between "ticks" in microseconds
    // 16000us gap = ~60Hz (Deep Hum)
    // 6000us gap = ~160Hz (Tense Vibration)
    unsigned long humGap = (unsigned long)mapFloat(tension, 0.0, 1.0, 16000.0, 6000.0);

    if (currentMicros - lastHumTime >= humGap) {
      lastHumTime = currentMicros;
      
      // THE "TICKLE" PULSE
      // Extremely short pulse creates a quiet mechanical click
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(180); // Adjust this (100-300) to change volume/texture
      digitalWrite(BUZZER_PIN, LOW);
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  if (in_max == in_min) return out_min;
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
