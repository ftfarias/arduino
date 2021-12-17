#include <MCUFRIEND_kbv.h>

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0x8410

MCUFRIEND_kbv tft;

// touch screen
#include "TouchScreen_kbv.h"
#define TouchScreen TouchScreen_kbv
#define TSPoint     TSPoint_kbv

const int XP=6,XM=A2,YP=A1,YM=7; //240x400 ID=0x9327
TouchScreen ts(XP, YP, XM, YM, 300);   //re-initialised after diagnose
TSPoint tp;     

const int TS_LEFT=927,TS_RT=180,TS_TOP=959,TS_BOT=166;

uint16_t version = MCUFRIEND_KBV_H_;

void drawCrossHair(int x, int y, uint16_t color)
{
    tft.drawRect(x - 10, y - 10, 20, 20, color);
    tft.drawLine(x - 5, y, x + 5, y, color);
    tft.drawLine(x, y - 5, x, y + 5, color);
}

void readResistiveTouch(void)
{
    tp = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    //digitalWrite(YP, HIGH);  //because TFT control pins
    //digitalWrite(XM, HIGH);
    Serial.println("tp.x=" + String(tp.x) + ", tp.y=" + String(tp.y) + ", tp.z =" + String(tp.z));
}

bool isPressed(void)
{
    // .kbv this was too sensitive !!
    // now touch has to be stable for 50ms
    int count = 0;
    bool state, oldstate;
    while (count < 2) {
        readResistiveTouch();
        state = tp.z > 200;     //ADJUST THIS VALUE TO SUIT YOUR SCREEN e.g. 20 ... 250
        if (state == oldstate) count++;
        else count = 0;
        oldstate = state;
        delay(5);
    }
    return oldstate;
}

void setup()
{
  Serial.begin(9600);
  if (!Serial) delay(5000);           //allow some time for Leonardo
  uint16_t ID = tft.readID(); //
  tft.begin(ID);
  tft.setRotation(1); // "PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV"

  tft.fillScreen(GRAY);
  tft.drawRect(0, 0, 10, 10, BLUE);
}

void loop()
{
//  int width = tft.width(); // 400
//  int height = tft.height(); // 240

  tft.setTextColor(WHITE);
  tft.setTextSize(1);

  if (isPressed()) {
    Serial.println("pressed");
    readResistiveTouch();
    //  LANDSCAPE CALIBRATION     400 x 240
    int x = map(tp.y, 959, 166, 0, 400);
    int y = map(tp.x, 180, 927, 0, 240);
    Serial.println(x);
    Serial.println(y);
    drawCrossHair(x,y, BLUE);
    // tft.setCursor(x, y);
    // tft.print("x");
  }

  
//  tft.drawRect(0, 0, 10, 10, WHITE);
//  tft.drawRect(32, 32, width - 64, height - 64, WHITE);
//  tft.setTextSize(2);
//  tft.setTextColor(BLACK);
//  tft.setCursor(40, 40);
//  tft.print("ID=0x");
//  tft.setCursor(40, 70);
//  tft.print("Ola");
//  tft.setCursor(40, 100);
//  tft.print(width);
//  tft.print(" x ");
//  tft.print(height);
//  tft.setTextColor(WHITE);
//  tft.setCursor(40, 130);
//  tft.print(colorname[aspect]);
//  tft.setCursor(40, 160);
//  tft.setTextSize(1);
//  tft.print("MCUFRIEND_KBV_H_ = ");
//  delay(10);
}
