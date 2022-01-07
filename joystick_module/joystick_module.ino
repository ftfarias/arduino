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

byte screen = 0;
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

void checkPosition()
{
  encoder->tick();
}

void setup() {
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
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Saving calibrating...."));
  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Calibration saved"));
  delay(100);
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

  encoderBtn = digitalRead(PIN_ENCODER_BTN)  == HIGH;
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
      int c = center[i];
      int j = joy[i];
      if (j < c - 5) {
        output[i] = map(j, range_min[i], c, 0, -100);
      } else if (j > c + 5) {
        output[i] = map(j, c, range_max[i], 100, c);
      } else {
        output[i] = 0;
      }
      // output[i] = map(j, range_min[i], range_max[i], 100, -100);
    }
  }

  screen = encoder_pos % 2;
  if (screen == 0) {
    screenAxis();
  }

  if (screen == 1) {
    screenOutput();
  }


}
