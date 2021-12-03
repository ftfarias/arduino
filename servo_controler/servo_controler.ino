#include <Servo.h>
#include <TM1638lite.h>
#include <RotaryEncoder.h>

//Conexao dos pinos STB, CLK e DO
TM1638lite tm(12, 11, 10);
RotaryEncoder encoder(7, 8);

#define NUM_SERVOS 8

#define BUTTON_SAVE 6

int servoPins[NUM_SERVOS] = {A0, A1, A2, A3, A4, A5, 3, 4};
Servo servo[NUM_SERVOS];
byte selectedServo = 0;
int degrees[NUM_SERVOS];
bool rotaryButton;
int pos = 0;
byte prev_state = 0;

void setup() {
  pinMode(9, INPUT_PULLUP); // rotary enconder button INPUT_PULLUP
  pinMode(2, INPUT_PULLUP); //
  pinMode(BUTTON_SAVE, INPUT_PULLUP); //
  pinMode(5, INPUT_PULLUP); //
  pinMode(13, INPUT_PULLUP); // RUN / MANUAL

  tm.reset();
  tm.displayText("Servo");

  for (int i = 0; i < NUM_SERVOS; i++) {
    tm.displayHex(7, i);
    servo[i] = Servo();
    servo[i].attach(servoPins[i]);
    degrees[i] = 90;
    servo[i].write(degrees[selectedServo]);
  }

  tm.displayText("Serial");
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps

  delay(1000);
}


void loop() {
  if (digitalRead(13) == HIGH) {
    if (prev_state != 1) {
      tm.displayText("PLAY");
      prev_state = 1;
    }
    play();
  } else {
    if (prev_state != 2) {
      tm.displayText("SAVE");
      prev_state = 2;
    }
    manual();
  }
}

void play() {
  if (Serial.available() > 0) {
    //    for (int i = 0; i < NUM_SERVOS; i++) {
    while (Serial.parseInt() != -1);
    Serial.read();
    int s = Serial.parseInt();
    Serial.read();
    int pos = Serial.parseInt();
    if (pos != 0) {
      servo[s].write(pos);
      Serial.println(s);
      Serial.println(pos);
    }
  }
}

void manual() {
  uint8_t buttons = tm.readButtons();
  if (buttons != 0) {
    selectedServo = getSelected(buttons);

    for (uint8_t position = 0; position < 8; position++)
    {
      tm.setLED(position, selectedServo == position);
    }
  }


  encoder.tick();
  rotaryButton = (digitalRead(9) == HIGH);

  RotaryEncoder::Direction dir = encoder.getDirection();
  if (dir == RotaryEncoder::Direction::CLOCKWISE && degrees[selectedServo] < 180) {
    if (rotaryButton) {
      degrees[selectedServo] += 10;
    } else {
      degrees[selectedServo] += 1;
    }
    if (degrees[selectedServo] > 179) {
      degrees[selectedServo] = 179;
    }
    servo[selectedServo].write(degrees[selectedServo]);
  }

  if (dir == RotaryEncoder::Direction::COUNTERCLOCKWISE && degrees[selectedServo] > 0) {
    if (rotaryButton) {
      degrees[selectedServo] -= 10;
    } else {
      degrees[selectedServo] -= 1;
    }
    if (degrees[selectedServo] < 1) {
      degrees[selectedServo] = 1;
    }
    servo[selectedServo].write(degrees[selectedServo]);
  }

  displayNumber(degrees[selectedServo]);

  if (digitalRead(BUTTON_SAVE) == HIGH) {
    for (uint8_t position = 0; position < 8; position++)
    {
      tm.setLED(position, 1);
    }
    sendPos();
    delay(500);
    for (uint8_t position = 0; position < 8; position++)
    {
      tm.setLED(position, selectedServo == position);
    }
  }


}

void sendPos() {
  for (int i = 0; i < NUM_SERVOS; i++) {
    if (i > 0) {
      Serial.print(',');
    }
    Serial.print(degrees[i]);
  }
  Serial.println(';'); 
}


byte getSelected(uint8_t value) {
  for (byte i = 0; i < 8; i++)
  {
    if (value & (1 << i)) {
      return i;
    }
  }
  return -1;
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
