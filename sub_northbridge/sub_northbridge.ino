// https://github.com/victorsvi/MatrixKeypad
#include <MatrixKeypad.h>

//#include <Adafruit_PCD8544.h>
#include <Adafruit_GFX.h>

#include <LiquidCrystal.h>

const int rs = 36, en = 34, d4 = 32, d5 = 30, d6 = 28, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const uint8_t keymapRowns = 4; //4 rows
const uint8_t keymapColns = 4; //4 columns
uint8_t rowPins[keymapRowns] = {46, 48, 50, 52}; //frist row is connect to pin 10, second to 9...
uint8_t colPins[keymapColns] = {38, 40, 42, 44}; //frist column is connect to pin 6, second to 5...
char keymap[keymapRowns][keymapColns] =
{ {'1', '2', '3', 'A'}, //key of the frist row frist column is '1', frist row second column column is '2'
  {'4', '5', '6', 'B'}, //key of the second row frist column is '4', second row second column column is '5'
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

uint8_t keypadPos = 0;
MatrixKeypad_t *keypad;
char key;

//#define N5110_1_CLK 38
//#define N5110_1_DIN 40
//#define N5110_1_DC 42
//#define N5110_1_CE 44
//#define N5110_1_RST 46
//Adafruit_PCD8544 nokia1 = Adafruit_PCD8544(N5110_1_CLK,N5110_1_DIN,N5110_1_DC,N5110_1_CE,N5110_1_RST);

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial3.begin(115200);

  keypad = MatrixKeypad_create((char*)keymap, rowPins, colPins, keymapRowns, keymapColns); //creates the keypad object


  //  nokia1.begin();
  //  nokia1.setContrast(50);
  //  nokia1.clearDisplay();
  //  nokia1.drawRoundRect(0, 0, 84 , 25, 1, 2);
  //  nokia1.setTextSize(2);
  //  nokia1.setTextColor(BLACK);
  //  nokia1.setCursor(12, 6);
  //  nokia1.println("SNAKE");
  //  nokia1.setTextSize(1);
  //  nokia1.setTextColor(BLACK);
  //  nokia1.setCursor(12, 29);
  //  nokia1.println("FILIPEFLOP");
  //  nokia1.setCursor(13, 29);
  //  nokia1.println("FILIPEFLOP");
  //  nokia1.setCursor(0, 40);
  //  nokia1.println("filipeflop.com");

  //  nokia1.display();
  delay(2000);

  lcd.begin(20, 4);
  lcd.display();
  lcd.setCursor(0, 0);  // col, row
  //         12345678901234567890
  lcd.print("Booting");
  for (int i = 7; i < 20; i++) {
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
  Serial3.println("clear;4226");
  delay(100);
  Serial3.println("title;Ships in Space");
  delay(100);
  Serial3.println("drawCrossHair;200;180;63488");
  delay(100);
  Serial3.println("drawBox;50;180;10;2016");
}

void loop() {
  //  lcd.clear();

  if (Serial2.available()) {
    lcd.setCursor(0, 4);
    lcd.print("Receiving BT...");
    Serial.println("Receiving BT...");
    String data = Serial2.readString();
    lcd.setCursor(0, 4);
    lcd.print(data);
    Serial.println(data);
  }

  int joy1 = analogRead(A0);
  int joy2 = analogRead(A1);
  int joy3 = analogRead(A2);

  lcd.setCursor(0, 1);
  lcd.print("                    ");

  lcd.setCursor(0, 1);
  lcd.print(joy1);

  lcd.setCursor(6, 1);
  lcd.print(joy2);

  lcd.setCursor(12, 1);
  lcd.print(joy3);

  //  nokia1.clearDisplay();
  //
  //  nokia1.setTextSize(1);                 //Tamanho da fonte
  //  nokia1.setTextColor(BLACK);            //Texto Preto
  //  nokia1.setTextColor(WHITE, BLACK);     //Texto Branco com fundo Preto
  //
  //  nokia1.setCursor(0,0);                 //Posição do Cursor
  //
  //  nokia1.print("BOOT");                 //Escreve o Texto
  //
  //  nokia1.setCursor(0,20);
  //  nokia1.setTextSize(1);
  //  nokia1.print("Display Nokia 5110");
  //
  //  nokia1.display();

  MatrixKeypad_scan(keypad); //scans for a key press event
  if (MatrixKeypad_hasKey(keypad)) { //if a key was pressed
    key = MatrixKeypad_getKey(keypad); //get the key
    lcd.setCursor(keypadPos, 2);
    keypadPos ++;
    lcd.print(key);

    if (key == '*') {
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      keypadPos = 0;
    }
  }

  delay(100);
}



// https://alberand.com/hc-05-linux.html
//  sudo minicom -D /dev/rfcomm0
