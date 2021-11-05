#include <TM1638lite.h>

//Conexao dos pinos STB, CLK e DO
TM1638lite tm(12, 11, 10);

void setup() {

  pinMode(13, INPUT_PULLUP); // RUN / MANUAL

  tm.reset();
  tm.displayText("INICIO");
}


void loop() {

  uint8_t buttons = tm.readButtons();
  if (buttons != 0) {
    doLEDs(buttons);
    displayNumber(buttons);
  }


  displayStepper(123);
  
}


void doLEDs(uint8_t value)
{
  for (uint8_t position = 0; position < 8; position++)
  {
    tm.setLED(position, value & 1);
    value = value >> 1;
  }
}

void displayNumber(uint8_t value)
{
  int digito7 = (value % 10);
  int digito6 = ((value / 10) % 10);
  int digito5 = ((value / 100) % 10);
  int digito4 = ((value / 1000) % 10);

  //Mostra os digitos no display
  tm.displayHex(7, digito7);
  tm.displayHex(6, digito6);
  tm.displayHex(5, digito5);
  tm.displayHex(4, digito4);
}


void displayStepper(uint8_t value)
{
  int digito3 = (value % 10);
  int digito2 = ((value / 10) % 10);
  int digito1 = ((value / 100) % 10);
  int digito0 = ((value / 1000) % 10);

  //Mostra os digitos no display
  tm.displayHex(3, digito3);
  tm.displayHex(2, digito2);
  tm.displayHex(1, digito1);
  tm.displayHex(0, digito0);
}
