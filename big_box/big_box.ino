#include <LiquidCrystal.h>

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(20, 4);
  lcd.display();
  lcd.setCursor(0, 0);  // col, row
  //         12345678901234567890
  lcd.print("Booting");
  for (int i=7; i<20; i++) {
       lcd.setCursor(i, 0);
         lcd.print(".");
       delay(100);
  }
  delay(700);
  lcd.noDisplay();
  delay(800);
  // Turn on the display:
  lcd.display();
  delay(400);
  lcd.setCursor(0, 1);  // col, row
  //         12345678901234567890
  lcd.print("Loading static data");
  delay(800);
  lcd.setCursor(0, 2);  // col, row
  lcd.print("Connect external dev");
  delay(300);
  lcd.setCursor(0, 3);  // col, row
  lcd.print("Start real-time sys");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

}
