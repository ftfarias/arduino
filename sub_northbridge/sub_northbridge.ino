#include <Adafruit_PCD8544.h>
#include <Adafruit_GFX.h>

#include <LiquidCrystal.h>

const int rs = 36, en = 34, d4 = 32, d5 = 30, d6 = 28, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


#define N5110_1_CLK 38
#define N5110_1_DIN 40
#define N5110_1_DC 42
#define N5110_1_CE 44
#define N5110_1_RST 46
Adafruit_PCD8544 nokia1 = Adafruit_PCD8544(N5110_1_CLK,N5110_1_DIN,N5110_1_DC,N5110_1_CE,N5110_1_RST);

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);

  nokia1.begin();     
  nokia1.setContrast(50);
  nokia1.clearDisplay();
  nokia1.drawRoundRect(0, 0, 84 , 25, 1, 2);
  nokia1.setTextSize(2);
  nokia1.setTextColor(BLACK);
  nokia1.setCursor(12, 6);
  nokia1.println("SNAKE");
  nokia1.setTextSize(1);
  nokia1.setTextColor(BLACK);
  nokia1.setCursor(12, 29);
  nokia1.println("FILIPEFLOP");
  nokia1.setCursor(13, 29);
  nokia1.println("FILIPEFLOP");
  nokia1.setCursor(0, 40);
  nokia1.println("filipeflop.com");
 
  nokia1.display();
  delay(2000);
  
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



  
//  delay(700);
//  lcd.noDisplay();
//  delay(800);
//  // Turn on the display:
//  lcd.display();
//  delay(400);
//  lcd.setCursor(0, 1);  // col, row
//  //         12345678901234567890
//  lcd.print("Loading static data");
//  delay(800);
//  lcd.setCursor(0, 2);  // col, row
//  lcd.print("Connect external dev");
//  delay(300);
//  lcd.setCursor(0, 3);  // col, row
//  lcd.print("Start real-time sys");
//  delay(1000);
  lcd.setCursor(0, 1);
  Serial.println("inicio");
}

void loop() {
  lcd.clear();
  
  if (Serial3.available()) {
    lcd.setCursor(0, 4);
    lcd.print("Receiving BT...");
    Serial.println("Receiving BT...");
    String data = Serial3.readString();
    lcd.setCursor(0, 4);
    lcd.print(data);
    Serial.println(data);
  }

  int joy1 = analogRead(A0);
  int joy2 = analogRead(A1);
  int joy3 = analogRead(A2);

  lcd.setCursor(0, 1);
  lcd.print(joy1);

  lcd.setCursor(6, 1);
  lcd.print(joy2);

  lcd.setCursor(12, 1);
  lcd.print(joy3);


  
  nokia1.clearDisplay();
 
  nokia1.setTextSize(1);                 //Tamanho da fonte
  nokia1.setTextColor(BLACK);            //Texto Preto
  nokia1.setTextColor(WHITE, BLACK);     //Texto Branco com fundo Preto
 
  nokia1.setCursor(0,0);                 //Posição do Cursor
 
  nokia1.print("BOOT");                 //Escreve o Texto
 
  nokia1.setCursor(0,20);
  nokia1.setTextSize(1);
  nokia1.print("Display Nokia 5110");
 
  nokia1.display();

  
  delay(1000);
}



// https://alberand.com/hc-05-linux.html
//  sudo minicom -D /dev/rfcomm0
