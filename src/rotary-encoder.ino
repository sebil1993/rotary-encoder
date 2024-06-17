
#include <Arduino.h>

const int pin1 = 5;
const int pin2 = 4;
const int keyPin = 0;

int state = 0;
int counter = 0;
int pressCounter = 0;

unsigned long debounceDelay = 100;
unsigned long lastDebounce = 0;

bool buttonState = HIGH;
bool lastButtonState = HIGH;

const unsigned char ttable[7][4] = {
    {0, 2, 4, 0},
    {3, 0, 1, 0 | 16},
    {3, 2, 0, 0},
    {3, 2, 1, 0},
    {6, 0, 4, 0},
    {6, 5, 0, 0 | 32},
    {6, 5, 4, 0},
};

void setup()
{
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
  pinMode(keyPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop()
{

  unsigned char result = process();
  // Clockwise rotation.
  if (result == 16)
  {
    counter++;
    Serial.println(counter);
  }

  // Counter clockwise rotation.
  else if (result == 32)
  {
    counter--;
    Serial.println(counter);
  }

  bool reading = digitalRead(keyPin);
  if (reading != lastButtonState)
  {
    lastDebounce = millis();
  }
  if ((millis() - lastDebounce) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == LOW)
      {
        pressCounter++;
        Serial.print("Press: ");
        Serial.println(pressCounter);
      }
    }
  }
  lastButtonState = reading;
}

unsigned char process()
{

  // Grab state of input pins.
  unsigned char pinstate = (digitalRead(pin2) << 1) | digitalRead(pin1);

  // Determine new state from the pins and state table.
  state = ttable[state & 0xf][pinstate];

  // Return emit bits, ie the generated event.
  return state & 48;
}

bool processButton(bool btn)
{
  if (btn == true)
  {
    return false;
  }
  else
    return true;
}