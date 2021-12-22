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
#define SPACE   0x1082  // 4226

// http://drakker.org/convert_rgb565.html


MCUFRIEND_kbv tft;

// touch screen
#include "TouchScreen_kbv.h"
#define TouchScreen TouchScreen_kbv
#define TSPoint     TSPoint_kbv

const int XP = 6, XM = A2, YP = A1, YM = 7; //240x400 ID=0x9327
TouchScreen ts(XP, YP, XM, YM, 300);   //re-initialised after diagnose
TSPoint tp;

const int TS_LEFT = 927, TS_RT = 180, TS_TOP = 959, TS_BOT = 166;

uint16_t version = MCUFRIEND_KBV_H_;

int mapNumber = 3141592;
int pos_x = 0;
int pos_y = 0;
int pos_x2 = 0;
int pos_y2 = 0;
int pos_x3 = 0;
int pos_y3 = 0;
int pos = 0;
int boxsize = 0;
int color = 0;
String command = "";


//bool imageChanged = true;

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
  //    Serial.println("tp.x=" + String(tp.x) + ", tp.y=" + String(tp.y) + ", tp.z =" + String(tp.z));
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
  Serial.begin(115200);
  if (!Serial) delay(5000);           //allow some time for Leonardo
  uint16_t ID = tft.readID(); //
  tft.begin(ID);
  tft.setRotation(1); // "PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV"
  //  tft.print("x=" + String(x) + ", y=" + String(y));
  tft.fillScreen(SPACE);
}

void drawTitle(String msg) {
  tft.fillRect(0, 0, 400, 12, WHITE);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 2);
  tft.print(msg);
}

void loop()
{

  if (Serial.available() > 0) {
    // title;Ships in Space
    // drawCrossHair;200;180;63488
    // drawBox;50;180;10;2016
    // drawBox;150;100;20;2016
    // drawCircle;200;20;10;63488
    // drawPixel;200;120;65535
    // fillTriangle;200;120;200;130;210;125;65535
    // text;30;30;1;65035;teste 1
    // text;30;60;2;60535;teste 2
    // text;30;90;3;60535;teste 3
    // clear;4226

    String received = Serial.readStringUntil('\n');
    Serial.println(received);

    pos = received.indexOf(";");
    command = received.substring(0, pos);
    received = received.substring(pos + 1);

    if (command.equalsIgnoreCase("title")) {
      drawTitle(received);
    } else if (command.equalsIgnoreCase("drawCrosshair")) {
      pos = received.indexOf(";");
      pos_x = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_y = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      color = received.toInt();

      drawCrossHair(pos_x, pos_y, color);
    } else if (command.equalsIgnoreCase("drawbox")) {
      pos = received.indexOf(";");
      pos_x = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_y = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      boxsize = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      color = received.toInt();
      tft.drawRect(pos_x - (boxsize / 2), pos_y - (boxsize / 2), boxsize, boxsize, color);
    } else if (command.equalsIgnoreCase("drawTriangle")) {
      pos = received.indexOf(";");
      pos_x = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_y = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_x2 = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_y2 = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_x3 = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_y3 = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      color = received.toInt();
      tft.drawTriangle(pos_x, pos_y, pos_x2, pos_y2, pos_x3, pos_y3, color);
    } else if (command.equalsIgnoreCase("fillTriangle")) {
      pos = received.indexOf(";");
      pos_x = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_y = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_x2 = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_y2 = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_x3 = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_y3 = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      color = received.toInt();
      tft.fillTriangle(pos_x, pos_y, pos_x2, pos_y2, pos_x3, pos_y3, color);
    } else if (command.equalsIgnoreCase("drawCircle")) {
      pos = received.indexOf(";");
      pos_x = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_y = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      boxsize = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      color = received.toInt();
      tft.drawCircle(pos_x, pos_y, boxsize, color);
    } else if (command.equalsIgnoreCase("fillCircle")) {
      pos = received.indexOf(";");
      pos_x = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_y = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      boxsize = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      color = received.toInt();
      tft.fillCircle(pos_x, pos_y, boxsize, color);
    } else if (command.equalsIgnoreCase("drawPixel")) {
      pos = received.indexOf(";");
      pos_x = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_y = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      color = received.toInt();
      tft.drawPixel(pos_x, pos_y, color);
    } else if (command.equalsIgnoreCase("text")) {
      pos = received.indexOf(";");
      pos_x = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      pos_y = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      boxsize = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);

      pos = received.indexOf(";");
      color = received.substring(0, pos).toInt();
      received = received.substring(pos + 1);
      
      String msg = received;
      tft.setCursor(pos_x, pos_y);
      tft.setTextSize(boxsize);
      tft.setTextColor(color);
      tft.print(msg);

    } else if (command.equalsIgnoreCase("clear")) {
      color = received.toInt();
      tft.fillScreen(color);
    }

  }

  //  if (isPressed()) {
  //    readResistiveTouch();
  //    //  LANDSCAPE CALIBRATION     400 x 240
  //    int x = map(tp.y, 959, 166, 0, 400);
  //    int y = map(tp.x, 180, 927, 0, 240);
  //    drawCrossHair(x, y, BLUE);
  //  }


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
  delay(50);
}
