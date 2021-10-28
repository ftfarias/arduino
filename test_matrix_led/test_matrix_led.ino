
#include "LedControl.h"


#define     DATA_PIN     10  
#define     CS_PIN       11  
#define     CLK_PIN      12  

LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, 1); 

void setup() {

  lc.shutdown(0, false);
  delay(1000);
  lc.shutdown(0, false);
  delay(1000);
  lc.clearDisplay(0);
  delay(1000);
  lc.setIntensity(0,1); 
  delay(100);
}

void loop() {
  byte N0[8] = {B00000000, B00000000, B00111110, B01000001,
                  B01000001, B00111110, B00000000, B00000000
                 };
  int tela = 0;
  lc.setRow(tela, 0, N0[0]);
  lc.setRow(tela, 1, N0[1]);
  lc.setRow(tela, 2, N0[2]);
  lc.setRow(tela, 3, N0[3]);
  lc.setRow(tela, 4, N0[4]);
  lc.setRow(tela, 5, N0[5]);
  lc.setRow(tela, 6, N0[6]);
  lc.setRow(tela, 7, N0[7]);
  delay(1000);
}
