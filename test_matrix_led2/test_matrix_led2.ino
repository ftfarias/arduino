#include "LedController.hpp"


#define     DATA_PIN     10  
#define     CS_PIN       11  
#define     CLK_PIN      12  


#define delayTime 200 // Delay between Frames

LedController<1,1> lc;


void setup() {
  // put your setup code here, to run once:
  lc = LedController<1,1>(DATA_PIN, CLK_PIN, CS_PIN);
  lc.clearMatrix();
//  lc.setLed(0,3,4,true);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  lc.clearMatrix();
//  lc.setLed(0,4,6,true);
//  lc.setLed(0,4,4,true);
//  lc.setLed(0,5,5,true);
//  lc.setLed(0,4,5,true);
//  lc.setLed(0,3,5,true);
  
  lc.setRow(0,0,B01010101);
  lc.setRow(0,1,B10101010);
  lc.setRow(0,2,B01010101);
  lc.setRow(0,3,B10101010);
  lc.setRow(0,4,B01010101);
  lc.setRow(0,5,B01010101);
  lc.setRow(0,6,B01010101);
  lc.setRow(0,7,B10101010);
  delay(1000);
}
