
#include <Arduino.h>

const byte pin1 = 5; // Connected to DT on KY-040
const byte pin2 = 4; // Connected to CLK on KY-040
int state = 0;
int counter = 0;

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