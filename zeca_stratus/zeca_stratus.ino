void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // open the serial port at 9600 bps:
  Serial.println("inicio");
  pinMode(2, INPUT); // st (ch 1)
  pinMode(3, INPUT); // th (ch 2)
  pinMode(4, INPUT); // ch 3

  pinMode(5, OUTPUT); // h bridge motor A side 1
  pinMode(6, OUTPUT); // h bridge motor A side 2

  pinMode(7, OUTPUT); // output channel 3

  pinMode(9, OUTPUT); // h bridge motor B side 1
  pinMode(10, OUTPUT); // h bridge motor B side 2

}

int min_st = 990;
int max_st = 1460;

int min_th = 990;
int max_th = 1460;

void loop() {
  unsigned long st = pulseIn(2, HIGH); // 985 - 1470 - 1890
  unsigned long th = pulseIn(3, HIGH); // 986 - 1470 - 1970;
  unsigned long ch3 = pulseIn(4, HIGH);

  min_st = min(min_st, st);
  max_st = max(max_st, st);

  min_th = min(min_th, th);
  max_th = max(max_th, th);


  //Serial.println(st);
  //Serial.println(th);
  //Serial.println(ch3);

  if (st < 1460) {
    int pwm_A_1 = map(st, min_st, 1460, 255, 0);
    analogWrite(5, pwm_A_1);
    analogWrite(6, 0);

    //    digitalWrite(5, HIGH);
    //    digitalWrite(6, LOW);

  } else if (st > 1480) {
    analogWrite(5, 0);
    int pwm_A_2 = map(st, 1480, max_st, 0, 255);
    analogWrite(6, pwm_A_2);

    //    digitalWrite(5, LOW);
    //    digitalWrite(6, HIGH);
  } else {
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  }

  if (th < 1460) {
    int pwm_A_1 = map(th, min_th, 1460, 255, 0);
    analogWrite(9, pwm_A_1);
    analogWrite(10, 0);
    //    digitalWrite(9, HIGH;
    //    digitalWrite(10, LOW);
  } else if (th > 1480) {
    analogWrite(9, 0);
    int pwm_A_2 = map(th, 1480, max_th, 0, 255);
    analogWrite(10, pwm_A_2);
    //    digitalWrite(9, LOW);
    //    digitalWrite(10, HIGH);
  } else {
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
  }

  if (ch3 < 1480) {
    digitalWrite(7, LOW);
  } else {
    digitalWrite(7, HIGH);
  }

  delay(50);

}
