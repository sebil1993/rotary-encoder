
#include <Arduino.h>

const int encoderPinA = 4; // S1
const int encoderPinB = 5; // S2
const int keyPin = 0;      // KEY

volatile int encoderPos = 0;
volatile bool A_set = false;
volatile bool B_set = false;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // Debounce-Zeit für den Button
bool buttonState = HIGH;          // Der aktuelle Zustand des Button
bool lastButtonState = HIGH;      // Der letzte bekannte Zustand des Button

void ICACHE_RAM_ATTR handleEncoder() {
  bool A = digitalRead(encoderPinA);
  bool B = digitalRead(encoderPinB);

  if (A && !A_set) {
    if (B) {
      encoderPos++;
    } else {
      encoderPos--;
    }
  }
  A_set = A;

  if (B && !B_set) {
    if (A) {
      encoderPos--;
    } else {
      encoderPos++;
    }
  }
  B_set = B;
}

void setup() {
  Serial.begin(115200);

  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(keyPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderPinA), handleEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), handleEncoder, CHANGE);
}

void loop() {
  static int lastEncoderPos = 0;
  noInterrupts();
  int pos = encoderPos;
  interrupts();

  if (pos != lastEncoderPos) {
    Serial.print("Encoder Position: ");
    Serial.println(pos);
    lastEncoderPos = pos;
  }

  // Entprellung für den Button
  bool reading = digitalRead(keyPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        Serial.println("Key Pressed");
      }
    }
  }
  lastButtonState = reading;

  delay(5); // Kleine Verzögerung zur Stabilisierung der Messungen
}