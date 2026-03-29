/**
 * THE HYPHEN
 * A physical computing project representing tension between states.
 * 
 * Hardware:
 * - Arduino Uno
 * - Tilt Switch: Pin 2 (with 10k Pull-down)
 * - Active Buzzer: Pin 9 (via PN2222 Transistor)
 */

// Pin Definitions
const int TILT_PIN = 2;
const int BUZZER_PIN = 9;

// Tension Parameters
float tension = 0.0;           // Current tension level (0.0 to 1.0)
const float GROWTH_RATE = 0.002; // How fast tension builds (adjust for feel)
const float DECAY_RATE = 0.001;  // How fast tension fades
const float MAX_TENSION = 1.0;
const float MIN_TENSION = 0.0;

// Pulse Timing
unsigned long lastPulseTime = 0;
bool buzzerState = false;

// Loop Timing
unsigned long lastUpdate = 0;
const int UPDATE_INTERVAL = 10; // Update logic every 10ms

void setup() {
  pinMode(TILT_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentTime = millis();

  // 1. Update Tension Logic (Timed Interval)
  if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = currentTime;
    
    bool isTilted = digitalRead(TILT_PIN);

    if (isTilted) {
      // Exponential-like growth: faster as it gets higher
      tension += GROWTH_RATE * (1.0 + tension); 
    } else {
      tension -= DECAY_RATE;
    }

    // Constrain tension
    tension = constrain(tension, MIN_TENSION, MAX_TENSION);
  }

  // 2. Control Buzzer Pulse Speed based on Tension
  if (tension > 0.05) { // Small threshold to avoid ghost pulses
    
    // Map tension to interval (ms)
    // High tension = Low interval (fast pulses)
    // Low tension = High interval (slow pulses)
    // Using a non-linear mapping for "tension" feel
    long pulseInterval = mapFloat(tension, 0.0, 1.0, 500.0, 30.0);

    if (currentTime - lastPulseTime >= pulseInterval) {
      lastPulseTime = currentTime;
      buzzerState = !buzzerState;
      digitalWrite(BUZZER_PIN, buzzerState);
    }
  } else {
    // If tension is effectively zero, ensure buzzer is off
    digitalWrite(BUZZER_PIN, LOW);
    buzzerState = false;
  }
}

/**
 * Helper to map floats (standard map() only handles long)
 */
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
