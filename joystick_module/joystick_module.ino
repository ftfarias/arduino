// https://github.com/mathertel/RotaryEncoder
#include <RotaryEncoder.h>

#include <LiquidCrystal_I2C.h>
#include <stdio.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
RotaryEncoder *encoder = nullptr;

#define PIN_IN1 2
#define PIN_IN2 3

#define PIN_ENCODER_BTN 4
#define PIN_JOY_BTN     5
#define PIN_SAVE_BTN    6
#define PIN_HOLD        7
#define PIN_CAL_RANGE   8
#define PIN_CAL_CENTER  9
int latchPin = 11;
int clockPin = 12;
int dataPin = 10;


byte screen = 0;
byte oldScreen = 99;
word calCenterCounter = 0;
word calRangeCounter = 0;
int encoder_pos = 0;

int joy[6] = { 0, 0, 0, 0, 0, 0 };
int output[6] = { 0, 0, 0, 0, 0, 0 };
int center_sum[6] = { 0, 0, 0, 0, 0, 0 };
int center[6] = { 512, 512, 512, 512, 512, 512 };
int range_max[6] = { 900, 900, 900, 900, 900, 900 };
int range_min[6] = { 100, 100, 100, 100, 100, 100  };

boolean encoderBtn;
boolean joyBtn;
boolean saveBtn;
boolean calCenterBtn;
boolean calRangeBtn;
boolean holdBtn;

boolean running = false;

unsigned long startTime;
unsigned long stopTime;
word counterTime = 0;
byte leds = 0;
const byte LEDS_CENTER1 = B00100000; // OK
const byte LEDS_CENTER2 = B00010000; // OK
const byte LEDS_CENTER3 = B00001000; // OK
const byte LEDS_BUTTON =       B00000001; // OK
const byte LEDS_DATASENT =     B01000000; // OK
const byte LEDS_DATARECEIVED = B10000000;
const byte LEDS_CALIBRATION1 = B00000010; // OK
const byte LEDS_CALIBRATION2 = B00000100; 


void checkPosition()
{
  encoder->tick();
}



void setup() {
  Serial.begin(115200);

  pinMode(PIN_ENCODER_BTN, INPUT_PULLUP);
  pinMode(PIN_JOY_BTN, INPUT_PULLUP);
  pinMode(PIN_SAVE_BTN, INPUT_PULLUP);
  pinMode(PIN_CAL_CENTER, INPUT_PULLUP);
  pinMode(PIN_CAL_RANGE, INPUT_PULLUP);
  pinMode(PIN_HOLD, INPUT_PULLUP);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  // A4 - SDA
  // A5 - SCL
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);  // col, row
  //         12345678901234567890
  lcd.print("Booting");

  encoder = new RotaryEncoder(PIN_IN2, PIN_IN1, RotaryEncoder::LatchMode::TWO03);
  encoder->setPosition(32000);

  // register interrupt routine
  attachInterrupt(digitalPinToInterrupt(PIN_IN1), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_IN2), checkPosition, CHANGE);

  startTime = micros();
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low
  //internal function setup
  int i = 0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  //for each bit in the byte myDataOut&#xFFFD;
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i = 7; i >= 0; i--)  {
    digitalWrite(myClockPin, 0);
    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1 << i) ) {
      pinState = 1;
    }
    else {
      pinState = 0;
    }
    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }
  //stop shifting//
  digitalWrite(myClockPin, 0);
}

//void registerWrite(int whichPin, int whichState) {
//  // the bits you want to send
//  byte bitsToSend = 0;
//  // turn off the output so the pins don't light up
//  // while you're shifting bits:
//  digitalWrite(latchPin, LOW);
//  // turn on the next highest bit in bitsToSend:
//  bitWrite(bitsToSend, whichPin, whichState);
//  // shift the bits out:
//  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);
//  // turn on the output so the LEDs can light up:
//  digitalWrite(latchPin, HIGH);
//}

void updateShiftRegister()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, leds);
  digitalWrite(latchPin, HIGH);
}

void screenSpeed() {
  lcd.clear();
  lcd.setCursor(0, 0);
  char c[16];
  sprintf(c, "Counter: %4d", counterTime);
  lcd.print(c);
  lcd.setCursor(0, 1);
  lcd.print((stopTime - startTime) / counterTime);
  counterTime = 0;
  startTime = micros();
  delay(1000);
}

void screenAxis() {
  char c[16];
  sprintf(c, "D:%4d:%4d:%4d", joy[0], joy[1], joy[2]);
  lcd.setCursor(0, 0);
  lcd.print(c);

  sprintf(c, "D:%4d:%4d:%4d", joy[3], joy[4], joy[5]);
  lcd.setCursor(0, 1);
  lcd.print(c);
}


void screenOutput() {
  char c[16];
  sprintf(c, "O:%4d:%4d:%4d", output[0], output[1], output[2]);
  lcd.setCursor(0, 0);
  lcd.print(c);

  sprintf(c, "O:%4d:%4d:%4d", output[3], output[4], output[5]);
  lcd.setCursor(0, 1);
  lcd.print(c);
}


void screenButton() {
  char c[16];
  sprintf(c, "Button :%d", joyBtn);
  lcd.setCursor(0, 0);
  lcd.print(c);
}

void calCenter() {
  if (calCenterCounter == 0) {
    for (byte i = 0; i < 6; i++) {
      center_sum[i] = 0;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cal. Center");
  }
  calCenterCounter++;
  byte axis = encoder_pos % 6;

  char c[16];
  sprintf(c, "Cal.Center:%4d", calCenterCounter);
  lcd.setCursor(0, 0);
  lcd.print(c);

  for (byte i = 0; i < 6; i++) {
    center_sum[i] += joy[i];
    center[i] = center_sum[i] / calCenterCounter;
  }

  sprintf(c, "Axis %d: %4d", axis, center[axis]);
  lcd.setCursor(0, 1);
  lcd.print(c);
  delay(50);
}

void calRange() {
  if (calRangeCounter == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Cal. Range"));
  }
  calRangeCounter++;

  byte axis = encoder_pos % 6;

  char c[16];
  sprintf(c, "Cal.Range:%5d", calRangeCounter);
  lcd.setCursor(0, 0);
  lcd.print(c);

  for (byte i = 0; i < 6; i++) {
    range_max[i] = max(range_max[i], joy[i]);
    range_min[i] = min(range_min[i], joy[i]);
  }

  sprintf(c, "Axis %d:%4d/%4d ", axis, range_min[axis], range_max[axis]);
  lcd.setCursor(0, 1);
  lcd.print(c);
  delay(50);

}

void saveCal() {
  leds |= LEDS_CALIBRATION1;
  updateShiftRegister();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Saving"));
  lcd.setCursor(0, 1);
  lcd.print(F("calibration"));
  delay(100);
  leds &= ~LEDS_CALIBRATION1;
  leds |= LEDS_CALIBRATION2;
  updateShiftRegister();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Calibration"));
  lcd.setCursor(0, 1);
  lcd.print(F("saved"));
  delay(100);
  leds &= ~LEDS_CALIBRATION2;
  updateShiftRegister();
}


void loop() {

  encoder->tick(); // just call tick() to check the state.

  encoder_pos = encoder->getPosition();

  if (encoder_pos < 0) {
    encoder_pos = 32000;
    encoder->setPosition(32000);

  } else if (encoder_pos > 64000) {
    encoder_pos = 32000;
    encoder->setPosition(32000);
  }

  joy[0] = analogRead(A0);
  joy[1] = analogRead(A1);
  joy[2] = analogRead(A2);
  joy[3] = analogRead(A3);
  joy[4] = analogRead(A6);
  joy[5] = analogRead(A7);

  encoderBtn = digitalRead(PIN_ENCODER_BTN)  == LOW;
  joyBtn = digitalRead(PIN_JOY_BTN)          == HIGH;
  saveBtn = digitalRead(PIN_SAVE_BTN)        == HIGH;
  calCenterBtn = digitalRead(PIN_CAL_CENTER) == LOW;
  calRangeBtn = digitalRead(PIN_CAL_RANGE)   == LOW;
  holdBtn = digitalRead(PIN_HOLD)            == LOW;

  if (calCenterBtn) {
    calCenter();
    return;
  } else {
    calCenterCounter = 0;
  }

  if (calRangeBtn) {
    calRange();
    return;
  } else {
    calRangeCounter = 0;
  }

  if (saveBtn) {
    saveCal();
    return;
  }

  if (holdBtn) {
    for (byte i = 0; i < 6; i++) {
      output[i] = 0;
    }
  } else {
    for (byte i = 0; i < 6; i++) {
      //  range_min  - c - range_max
      //  +100       - 0 -   -100
      int c = center[i];
      int j = joy[i];
      if (j < c ) {
        // range_min < j < c
        // +100      < x < 0
        output[i] = map(j, range_min[i], c, 100, 0);
      } else if (j > c ) {
        //  c < j < range_max
        //  0 < x < -100
        output[i] = map(j, c, range_max[i], 0, -100);
      } else {
        output[i] = 0;
      }
      // output[i] = map(j, range_min[i], range_max[i], 100, -100);
    }
  }

  

  if (output[0] < 2 && output[0] > -2) {
    leds |= LEDS_CENTER1;
  } else {
    leds &= ~LEDS_CENTER1;
  }

  if (output[1] < 2 && output[1] > -2) {
    leds |= LEDS_CENTER2;
  } else {
    leds &= ~LEDS_CENTER2;
  }

  if (output[2] < 2 && output[2] > -2) {
    leds |= LEDS_CENTER3;
  } else {
    leds &= ~LEDS_CENTER3;
  }


  if (joyBtn) {
    leds |= LEDS_BUTTON;
  } else {
    leds &= ~LEDS_BUTTON;
  }

  
  screen = encoder_pos % 4;

  if (screen != oldScreen) {
    lcd.clear();
    oldScreen = screen;
  }
  
  if (encoderBtn) {
    stopTime = micros();
    screenSpeed();
  }

  if (screen == 0) {
    if (!running) {
      lcd.setCursor(0, 0);
      lcd.print(F("Running"));
      lcd.setCursor(0, 1);
      lcd.print(F("<no output>"));
      leds |= LEDS_DATASENT;

    }
    running = true;
    Serial.println("start");
    for (byte i = 0; i < 6; i++) {
      Serial.println(output[i]);
    }
  } else {
    leds &= ~LEDS_DATASENT;
  }

    
  if (screen == 1) {
    screenAxis();
    running = false;
  }

  if (screen == 2) {
    screenOutput();
    running = false;

  }

  if (screen == 3) {
    screenButton();
    running = false;

  }
  
//  if ((counterTime % 10) == 0) {
    updateShiftRegister();
//  }

  counterTime++;
}
